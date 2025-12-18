#include "Assign.hpp"
#include <iostream>

using namespace semantics;

Assign::Assign(std::shared_ptr<AssignableASTNode> target, std::shared_ptr<ASTNode> source)
    : target_(std::move(target)), source_(std::move(source)) {}

core::Object* Assign::evaluate(runtime::Frame& state) const {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* source = source_->evaluate(state);
    guard.protect(source);
    
    target_->assign(state, source);

    // Assignment returns the assigned value as a return value.
    return source;
}
