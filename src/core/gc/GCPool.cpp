#include "GCPool.hpp"
#include "GarbageCollector.hpp"
using namespace gc;

GCPool::GCPool() {
    // GC pools are always roots. That's their main purpose
    GarbageCollector::instance().defineRoot(this);
}

GCPool& GCPool::instance() {
    static GCPool pool;
    return pool;
}

void GCPool::add(GCTrackable* trackable) {
    elements_.insert(trackable);
}

void GCPool::remove(GCTrackable* trackable) {
    elements_.erase(std::remove(elements_.begin(), elements_.end(), trackable), elements_.end());
}

std::vector<gc::GCTrackable*> GCPool::referencees() {
    return std::vector<gc::GCTrackable*>(elements_.begin(), elements_.end());
}

GCPool::~GCPool() {
    elements_.clear();
}