#include "GarbageCollector.hpp"
#include <iostream>
using namespace gc;

GarbageCollector GarbageCollector::instance_;

void GarbageCollector::initialize(runtime::Environment *env) {
    if (instance_.env_) {
        throw std::runtime_error("GarbageCollector already initialized");
    }
    instance_.env_ = env;
}

GarbageCollector& GarbageCollector::instance() {
    if (!instance_.env_) {
        throw std::runtime_error("GarbageCollector not initialized");
    }
    return instance_;
}

void GarbageCollector::trackObject(gc::GCTrackable *obj) {
    trackedObjects_.insert(obj);
}

void GarbageCollector::mark_and_sweep() {
    /** TODO: */
}

GarbageCollector::~GarbageCollector() {
    for (auto obj : trackedObjects_) {
       delete obj;
    }
}
