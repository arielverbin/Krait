#ifndef CORE_GC_GARBAGE_COLLECTOR_HPP
#define CORE_GC_GARBAGE_COLLECTOR_HPP

#include "runtime/Environment.hpp"
#include "core/Object.hpp"
#include <unordered_set>

namespace gc {

class GarbageCollector {
private:
    runtime::Environment *env_;
    std::unordered_set<gc::GCTrackable*> trackedObjects_;

    GarbageCollector() = default;
    GarbageCollector(const GarbageCollector&) = delete;
    GarbageCollector& operator=(const GarbageCollector&) = delete;
    GarbageCollector(GarbageCollector&&) = delete;
    GarbageCollector& operator=(GarbageCollector&&) = delete;

    // Static singleton instance
    static GarbageCollector instance_;

public:
    static void initialize(runtime::Environment *env);
    static GarbageCollector& instance();

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