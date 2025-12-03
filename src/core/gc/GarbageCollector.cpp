#include "GarbageCollector.hpp"
#include "exceptions/exceptions.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/TypeObject.hpp"
#include "core/Object.hpp"
#include <iostream>
using namespace gc;

GarbageCollector& GarbageCollector::instance() {
    static GarbageCollector instance_;
    return instance_;
}

void GarbageCollector::trackObject(gc::GCTrackable *obj) {
    trackedObjects_.insert(obj);
}

void GarbageCollector::mark(GCTrackable* root) {
    if (!root) return;

    std::vector<GCTrackable*> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        GCTrackable* obj = stack.back();
        stack.pop_back();

        if (!obj || obj->isMarked()) continue;
        
        obj->mark();

        for (auto child : obj->referencees()) {
            if (!child->isMarked())
                stack.push_back(child);
        }
    }
}

size_t GarbageCollector::sweep() {
    size_t deleteCounter = 0;

    for (auto it = trackedObjects_.begin(); it != trackedObjects_.end(); ) {
        GCTrackable* obj = *it;
        if (!obj->isMarked()) {
            core::Object* asObj = dynamic_cast<core::Object*>(obj);
            if (asObj) { std::cout << " - Deleting object '" << asObj->type()->name() << "'..." << std::endl; fflush(stdout); }
            else { std::cout << " - Deleting Trackable..." << std::endl; }
            delete obj;
            deleteCounter += 1;
            it = trackedObjects_.erase(it);
        } else {
            obj->unmark();
            ++it;
        }
    }
    return deleteCounter;
}

void GarbageCollector::defineRoot(gc::GCTrackable *root) {
    roots_.push_back(root);
}
void GarbageCollector::undefineRoot(gc::GCTrackable *root) {
    roots_.erase(std::remove(roots_.begin(), roots_.end(), root), roots_.end());
}

size_t GarbageCollector::mark_and_sweep() {
    for (const auto& root : roots_) {
        GarbageCollector::mark(root);
    }
    return sweep();
}

GarbageCollector::~GarbageCollector() {
    for (auto obj : trackedObjects_) {
        delete obj;
    }
    trackedObjects_.clear();
}

core::Object* GarbageCollector::collect_garbage(const core::CallArgs& args) {
    if (args.size() != 1) 
        throw except::InvalidArgumentException(
            "garbage collection requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto verbose = dynamic_cast<core::Boolean*>(args[0]);
    if (!verbose) throw except::TypeError("argument to garbage collection must be a boolean");
    
    size_t deleteCounter = GarbageCollector::instance().mark_and_sweep();
    
    std::cout << "Successfully cleaned " << deleteCounter << " objects." << std::endl; fflush(stdout);

    return core::None::getNone();
}