#ifndef CORE_GC_GARBAGE_GCPOOL_HPP
#define CORE_GC_GARBAGE_GCPOOL_HPP

#include "core/gc/GCTrackable.hpp"
#include <unordered_set>

namespace gc {

/**
 * GC-pools are a way to organize trackables into protected groups, preventing them from being destroyed even though
 * nothing is yet to reference them.
 * A usage example is for a parser to save the script's constants ahead of evaluation time,
 * or pre-initialize all integers up to a reasonable limit.
 * 
 * The pool is defined as a single root for the GC, instead of defining all of its elemenets as 'tiny' roots.
 */
class GCPool : public GCTrackable {
private:
    std::unordered_set<GCTrackable*> elements_;

public:
    GCPool();
    void add(GCTrackable* trackable);
    void remove(GCTrackable* trackable);

    std::vector<gc::GCTrackable*> referencees() override;

    virtual size_t size() override { return sizeof(GCPool); }
    virtual ~GCPool();

    static GCPool& instance();
};

}  // namespace gc

#endif  //CORE_GC_GARBAGE_GCPOOL_HPP