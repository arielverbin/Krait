#include "ASTNode.hpp"
using namespace semantics;

core::Object* BindableASTNode::evaluate(runtime::Frame& state) const {
    // runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* bindable = compute(state);
    // guard.protect(bindable);

    // Store the class in the environment
    state.defineVariable(name(), bindable);

    return bindable;
}