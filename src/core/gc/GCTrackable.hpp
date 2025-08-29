#ifndef CORE_GC_TRACKABLE_HPP
#define CORE_GC_TRACKABLE_HPP

#include <vector>

namespace gc {

class GCTrackable {
private:
    bool mark_;

public:
    void mark() { mark_ = true; }
    void unmark() { mark_ = false; }
    bool isMarked() { return mark_; }

    virtual ~GCTrackable() = default;

    /** TODO: use an iterator approach to avoid vector allocation */
    virtual std::vector<GCTrackable*> referencees() = 0;
};

} // namespace gc

#endif // CORE_GC_TRACKABLE_HPP