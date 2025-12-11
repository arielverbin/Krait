#ifndef RUNTIME_EVALUATION_STACK_HPP
#define RUNTIME_EVALUATION_STACK_HPP

#include "core/gc/GCTrackable.hpp"
#include "core/Object.hpp"
#include <vector>
#include <stack>

namespace runtime {

class EvalContext : public gc::GCTrackable {
private:
    std::vector<core::Object*> evalStack_;

    static std::stack<EvalContext*> evalContexts_;
public:
    class EvalGuard {
    public:
        EvalGuard(EvalContext& context);
        void protect(core::Object* obj);
        ~EvalGuard() noexcept;
        EvalGuard(const EvalGuard&) = delete;
        EvalGuard& operator=(const EvalGuard&) = delete;
    private:
        EvalContext& context_;
        size_t stackSize_;
    };

    EvalGuard Guard();

    static void push(EvalContext* ec);
    static void pop();
    static EvalContext& current();
    
    virtual size_t size() override { return sizeof(EvalContext); }
    virtual std::vector<gc::GCTrackable*> referencees() override;
};

} // namespace runtime

#endif // RUNTIME_EVALUATION_STACK_HPP