#include "GarbageCollector.hpp"
#include <iostream>
using namespace gc;

GarbageCollector GarbageCollector::instance_;

GarbageCollector& GarbageCollector::instance() {
    return instance_;
}

void GarbageCollector::trackObject(gc::GCTrackable *obj) {
    trackedObjects_.insert(obj);
}

void GarbageCollector::scan_reachable(gc::GCTrackable* root) {
    if (root->isMarked()) {
        // avoid re-visiting the same node, to prevent infinite loop
        return;
    }

    root->mark();
    for (auto& ref : root->referencees()) {
        GarbageCollector::scan_reachable(ref);
    }
}

void GarbageCollector::defineRoot(gc::GCTrackable *root) {
    roots_.push_back(root);
}

void GarbageCollector::mark_and_sweep() {
    GarbageCollector::scan_reachable(nullptr);

    for (auto it = trackedObjects_.begin(); it != trackedObjects_.end(); ) {
        gc::GCTrackable* obj = *it;
        if (!obj->isMarked()) {
            it = trackedObjects_.erase(it); // erase returns the next iterator
            delete obj;
        } else {
            obj->unmark();  // prepare for next GC cycle
            ++it;
        }
    }
}

GarbageCollector::~GarbageCollector() {
    for (auto obj : trackedObjects_) {
        delete obj;
    }
}
