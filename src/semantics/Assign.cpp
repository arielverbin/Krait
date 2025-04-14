#include "Assign.hpp"
#include <iostream>

using namespace semantics;

Assign::Assign(std::shared_ptr<AssignableASTNode> target, std::shared_ptr<ASTNode> source)
    : target_(target), source_(source) {}

std::shared_ptr<core::Object> Assign::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> source = source_->evaluate(state);
    target_->assign(state, source);

    // Assignment return the assigned value as a return value.
    return source;
}
