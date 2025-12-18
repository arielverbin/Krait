#ifndef CORE_GC_GARBAGE_COLLECTOR_HPP
#define CORE_GC_GARBAGE_COLLECTOR_HPP

#include "runtime/Frame.hpp"
#include "runtime/EvalContext.hpp"
#include "core/Object.hpp"
#include <unordered_set>

namespace gc {

#define INITIAL_BYTES_THRESHOLD (300 * 1024)  // 300 KB

class GarbageCollector {
private:
    std::unordered_set<gc::GCTrackable*> trackedObjects_;
    std::vector<gc::GCTrackable*> roots_;
    
    size_t objectCount_;
    size_t bytesAllocated_;
    size_t byteThreshold_;

    GarbageCollector();
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
    static core::Object* gc_info(const core::CallArgs& args);

    void flush();
    ~GarbageCollector();
};

template <typename T, typename... Args>
#ifdef KRAIT_DEBUGGING
T* make_tracked(const char* file, int line, Args&&... args) {
#else
T* make_tracked(Args&&... args) {
#endif // KRAIT_DEBUGGING
    T* instance = new T(std::forward<Args>(args)...);
    instance->unmark();

    #ifdef KRAIT_DEBUGGING
    instance->file = file;
    instance->line = line;
    #endif // KRAIT_DEBUGGING

    gc::GarbageCollector::instance().call();
    gc::GarbageCollector::instance().trackObject(instance);
    return instance;
}

template <typename T, typename... Args>
#ifdef KRAIT_DEBUGGING
T* make_guarded(const char* file, int line, Args&&... args) {
    T* t = make_tracked<T>(file, line, std::forward<Args>(args)...);
#else
T* make_guarded(Args&&... args) {
    T* t = make_tracked<T>(std::forward<Args>(args)...);
#endif // KRAIT_DEBUGGING
    runtime::EvalContext::current().push(t);
    return t;
}

} // namespace gc

#endif // CORE_GC_GARBAGE_COLLECTOR_HPPn