#ifndef CORE_GC_GARBAGE_COLLECTOR_HPP
#define CORE_GC_GARBAGE_COLLECTOR_HPP

#include "runtime/Frame.hpp"
#include "core/Object.hpp"
#include <unordered_set>

#ifdef KRAIT_TESTING
struct source_location {
    int line; const char* file;
};
#endif // KRAIT_TESTING

namespace gc {

class GarbageCollector {
private:

    std::unordered_set<gc::GCTrackable*> trackedObjects_;
    std::vector<gc::GCTrackable*> roots_;

    GarbageCollector() = default;
    GarbageCollector(const GarbageCollector&) = delete;
    GarbageCollector& operator=(const GarbageCollector&) = delete;
    GarbageCollector(GarbageCollector&&) = delete;
    GarbageCollector& operator=(GarbageCollector&&) = delete;

    void mark(gc::GCTrackable* root);
    size_t sweep();

public:
    static GarbageCollector& instance();

    // root objects are not managed by the GC, it does not delete them.
    void defineRoot(gc::GCTrackable *root);
    void undefineRoot(gc::GCTrackable *root);

    // GC takes ownership of trackable objects, and can delete them.
    void trackObject(gc::GCTrackable *obj);

    size_t mark_and_sweep();

    static core::Object* collect_garbage(const core::CallArgs& args);

    ~GarbageCollector();
};

#ifdef KRAIT_TESTING
template <typename T, typename... Args>
T* make_tracked(Args&&... args) {
    T* instance = new T(std::forward<Args>(args)...);
    instance->unmark();

    instance->file = "";
    instance->line = 0;

    gc::GarbageCollector::instance().trackObject(instance);
    return instance;
}
#else
template <typename T, typename... Args>
T* make_tracked(Args&&... args) {
    T* instance = new T(std::forward<Args>(args)...);
    instance->unmark();
    gc::GarbageCollector::instance().trackObject(instance);
    return instance;
}
#endif // KRAIT_TESTING

} // namespace gc

#endif // CORE_GC_GARBAGE_COLLECTOR_HPP