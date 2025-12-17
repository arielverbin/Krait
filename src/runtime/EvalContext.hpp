#ifndef RUNTIME_EVALUATION_STACK_HPP
#define RUNTIME_EVALUATION_STACK_HPP

#include "core/gc/GCTrackable.hpp"
#include "core/Object.hpp"
#include <vector>
#include <stack>

namespace runtime {

/**
 * EvalContext is initialized for each Frame (of a function) during the execution.
 * At any moment, the current EvalContext belongs to the currently evaluated function, including the "global" function.
 * 
 * The only exception to this statement is when the interpreter is booting:
 * The 'Frame' object can't be initialized yet (for example because the concept of 'type' is still not initialized),
 * but the builtins need evaluation context to run with.
 * 
 * SO - EvalContext holds a global evaluation context that always lives (and does not belong to a Frame).
 * Therefore it is important to note that all objects that are initialized before the first Frame is created -
 * belong to the global evaluation context (which differs from the evaluation context of the global Frame).
 * 
 * The interpreter is responsible for initializing the global evaluation context on boot, and releasing it when it termintes.
 */

class EvalContext : public gc::GCTrackable {
private:
    std::vector<gc::GCTrackable*> evalStack_;
    static std::stack<EvalContext*> evalContexts_;

public:

    /**
     * EvalGuard is a crutial part of the evaluation stack and the garbage collector.
     * Builtin functions should ignore memory management and use make_guarded to initialize krait objects.
     * Guards MUST be initialized on the stack, and once they are destructed, they ensure to restore the evaluation stack 
     * to how it was when they were initialized. That way, all objects that were pushed to the evaluation stack since the guard
     * was declared, will be popped once the guard is destroyed.
     * 
     * This behaviour helps prevent the garbage collector from deleting objects that were just declared but are yet to be referenced.
     * For example,
     * auto obj1 = gc::make_tracked<Integer>(1);
     * auto obj2 = gc::make_tracked<Integer>(2); // the garbage collector could have delete obj1 at this point
     * 
     * // the objects are just now being added to the reference tree, but there is a race with the garbage collector.
     * complex.setAttribute("real", obj1);
     * complex.setAttribute("img", obj2);
     * 
     * Or, using Guard (safe version),
     * audo guard = runtime::EvalContext::current().Guard();
     * auto obj1 = gc::make_tracked<Integer>(1);
     * guard.protect(obj1);
     * auto obj2 = gc::make_tracked<Integer>(2);
     * guard.protect(obj2);
     * 
     * // the objects are just now being added to the reference tree, but there is a race with the garbage collector.
     * complex.setAttribute("real", obj);
     * complex.setAttribute("img", obj2);
     * 
     * // when the function exists, the evaluation stack will restore itself thanks to the guard, and obj1, obj2 will be
     * popped out, but since complex alreay references them, they are safe from the GC.
     */

    class EvalGuard {
    public:
        EvalGuard(EvalContext& context);
        void protect(gc::GCTrackable* obj);
        ~EvalGuard();
        EvalGuard(const EvalGuard&) = delete;
        EvalGuard& operator=(const EvalGuard&) = delete;
    private:
        EvalContext& context_;
        size_t stackSize_;
    };

    EvalGuard Guard();
    void push(gc::GCTrackable* obj);
    void pop();

    static EvalContext* initGlobalContext();
    static void pushContext(EvalContext* ec);
    static void popContext();
    static EvalContext& current();
    
    virtual size_t size() override { return sizeof(EvalContext); }
    virtual std::vector<gc::GCTrackable*> referencees() override;
};

} // namespace runtime

#endif // RUNTIME_EVALUATION_STACK_HPP