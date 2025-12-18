#include "GarbageCollector.hpp"
#include "exceptions/exceptions.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/TypeObject.hpp"
#include "core/Object.hpp"
#include <iostream>
using namespace gc;

GarbageCollector::GarbageCollector() : objectCount_(0), bytesAllocated_(0), byteThreshold_(INITIAL_BYTES_THRESHOLD) {}

GarbageCollector& GarbageCollector::instance() {
    static GarbageCollector instance_;
    return instance_;
}

void GarbageCollector::trackObject(gc::GCTrackable *obj) {
    if (trackedObjects_.contains(obj)) {
        // should never happen
        throw std::runtime_error("object was tracked twice");
    }

    #ifdef KRAIT_DEBUGGING
    core::Object* asObj = dynamic_cast<core::Object*>(obj);
    if (asObj && asObj->type()) {
        std::cout << "[GC] Tracking object '" << asObj->type()->name() << "'";
    } else if (asObj)  {
        std::cout << "[GC] Tracking object (no type)";
    } else {
        std::cout << "[GC] Tracking trackable";
    }
    std::cout << " (defined in " << obj->file << ":" << obj->line << ")";
    std::cout << std::endl;
    #endif // KRAIT_DEBUGGING

    trackedObjects_.insert(obj);
    bytesAllocated_ += obj->size();
    objectCount_ += 1;
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

size_t GarbageCollector::sweep(bool verbose) {
    size_t deleteCounter = 0;

    for (auto it = trackedObjects_.begin(); it != trackedObjects_.end(); ) {
        GCTrackable* obj = *it;
        if (!obj->isMarked()) {

            if (verbose) {
                core::Object* asObj = dynamic_cast<core::Object*>(obj);
                if (asObj) {
                    std::cout << "[GC] Deleted object '" << asObj->type()->name() << "'";
                } else {
                    std::cout << "[GC] Deleted trackable";
                }
                #ifdef KRAIT_DEBUGGING
                std::cout << " (defined in " << obj->file << ":" << obj->line << ")";
                #endif // KRAIT_DEBUGGING
                std::cout << std::endl;
            }

            deleteCounter += 1;
            bytesAllocated_ -= obj->size();
            objectCount_ -= 1;

            delete obj;

            it = trackedObjects_.erase(it);
        } else {
            obj->unmark();
            ++it;
        }
    }

    for (auto r : roots_) r->unmark();
    return deleteCounter;
}

void GarbageCollector::defineRoot(gc::GCTrackable *root) {
    roots_.push_back(root);
}
void GarbageCollector::undefineRoot(gc::GCTrackable *root) {
    roots_.erase(std::remove(roots_.begin(), roots_.end(), root), roots_.end());
}

size_t GarbageCollector::mark_and_sweep(bool verbose) {
    for (const auto& root : roots_) {
        GarbageCollector::mark(root);
    }
    return sweep(verbose);
}

void GarbageCollector::flush() {
    for (auto obj : trackedObjects_) {
        delete obj;
    }
    trackedObjects_.clear();
    roots_.clear();

    objectCount_ = 0;
    bytesAllocated_ = 0;
    byteThreshold_ = INITIAL_BYTES_THRESHOLD;
}

GarbageCollector::~GarbageCollector() {
    flush();
}

void GarbageCollector::call() {
    #if KRAIT_DEBUGGING // for the debug version, we call the GC each time !
    size_t bytesBefore = bytesAllocated_;
    #else
    if (bytesAllocated_ > byteThreshold_)
    #endif // KRAIT_DEBUGGING
    {
        size_t deleteCounter = mark_and_sweep(/*verbose=*/false);

        #ifdef KRAIT_DEBUGGING
        std::cout << "[GC] Cleaned " << (bytesBefore - bytesAllocated_)
            << " bytes across " << deleteCounter << " deleted objects." << std::endl;
        #else
        UNREFERENCED(deleteCounter);
        #endif // KRAIT_DEBUGGING

        // update threshold
        byteThreshold_ = bytesAllocated_ * 10;
    }
}

core::Object* GarbageCollector::collect_garbage(const core::CallArgs& args) {
    if (args.size() != 1) 
        throw except::InvalidArgumentException(
            "garbage collection requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto verbose = dynamic_cast<core::Boolean*>(args[0]);
    if (!verbose) throw except::TypeError("argument to garbage collection must be a boolean");
    
    size_t deleteCounter = GarbageCollector::instance().mark_and_sweep(*verbose);
    
    std::cout << "[GC] Successfully cleaned " << deleteCounter << " objects." << std::endl;
    
    return core::None::getNone();
}

core::Object* GarbageCollector::gc_info(const core::CallArgs& args) {
    UNREFERENCED(args);

    GarbageCollector& gc = GarbageCollector::instance();
    std::cout << "[GC] BYTES ALLOCATED: " << gc.bytesAllocated_
              << ", CURRENT THRESHOLD: " << gc.byteThreshold_
              << ", OBJECT COUNT: " << gc.objectCount_
              << std::endl;
    
    return core::None::getNone();
}