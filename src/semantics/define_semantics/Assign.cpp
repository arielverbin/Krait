#include "Assign.hpp"
#include <iostream>

using namespace semantics;

Assign::Assign(std::shared_ptr<AssignableASTNode> target, std::shared_ptr<ASTNode> source)
    : target_(std::move(target)), source_(std::move(source)) {}

std::shared_ptr<core::Object> Assign::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> source = source_->evaluate(state);
    target_->assign(state, source);

    // Assignment returns the assigned value as a return value.
    return source;
}
