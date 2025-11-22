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

void GarbageCollector::defineRoot(gc::GCTrackable *root) {
    roots_.push_back(root);
}

void GarbageCollector::mark_and_sweep() {
    /** TODO: */
}

GarbageCollector::~GarbageCollector() {
    for (auto obj : trackedObjects_) {
        delete obj;
    }
}