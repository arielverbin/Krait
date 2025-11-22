#ifndef CORE_GC_GARBAGE_COLLECTOR_HPP
#define CORE_GC_GARBAGE_COLLECTOR_HPP

#include "runtime/Environment.hpp"
#include "core/Object.hpp"
#include <unordered_set>

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

    // Static singleton instance
    static GarbageCollector instance_;

public:
    static GarbageCollector& instance();

    // root objects are not managed by the GC, it does not delete them.
    void defineRoot(gc::GCTrackable *root);

    // GC takes ownership of trackable objects, and can delete them.
    void trackObject(gc::GCTrackable *obj);

    void mark_and_sweep();

    ~GarbageCollector();
};

template <typename T, typename... Args>
T* make_tracked(Args&&... args) {
    T* instance = new T(std::forward<Args>(args)...);
    gc::GarbageCollector::instance().trackObject(instance);
    return instance;
}

} // namespace gc

#endif // CORE_GC_GARBAGE_COLLECTOR_HPP