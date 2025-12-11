#ifndef CORE_GC_GARBAGE_COLLECTOR_HPP
#define CORE_GC_GARBAGE_COLLECTOR_HPP

#include "runtime/Frame.hpp"
#include "core/Object.hpp"
#include <unordered_set>

namespace gc {

class GarbageCollector {
private:

    std::unordered_set<gc::GCTrackable*> trackedObjects_;
    std::vector<gc::GCTrackable*> roots_;
    size_t bytesAllocated;

    GarbageCollector() = default;
    GarbageCollector(const GarbageCollector&) = delete;
    GarbageCollector& operator=(const GarbageCollector&) = delete;
    GarbageCollector(GarbageCollector&&) = delete;
    GarbageCollector& operator=(GarbageCollector&&) = delete;

    void mark(gc::GCTrackable* root);
    size_t sweep(bool verbose);

public:
    static GarbageCollector& instance();

    // root objects are not managed by the GC, it does not delete them.
    void defineRoot(gc::GCTrackable *root);
    void undefineRoot(gc::GCTrackable *root);

    // GC takes ownership of trackable objects, and can delete them.
    void trackObject(gc::GCTrackable *obj);

    // Marks & Sweeps, returns amount of deleted objects
    size_t mark_and_sweep(bool verbose);

    // Calls the garbage collector to give it a chance the collect garbage
    void call();

    static core::Object* collect_garbage(const core::CallArgs& args);

    ~GarbageCollector();
};

template <typename T, typename... Args>
#ifdef KRAIT_TESTING
T* make_tracked(const char* file, int line, Args&&... args) {
#else
T* make_tracked(Args&&... args) {
#endif
    T* instance = new T(std::forward<Args>(args)...);
    instance->unmark();

    #ifdef KRAIT_TESTING
    instance->file = file;
    instance->line = line;
    #endif

    gc::GarbageCollector::instance().call();
    gc::GarbageCollector::instance().trackObject(instance);
    return instance;
}

} // namespace gc

#endif // CORE_GC_GARBAGE_COLLECTOR_HPPn