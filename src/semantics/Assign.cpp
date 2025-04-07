#include "Assign.hpp"
#include <iostream>

using namespace semantics;

Assign::Assign(std::string targetVar, std::shared_ptr<ASTNode> source)
    : targetVar_(targetVar), source_(source) {}

std::shared_ptr<core::Object> Assign::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> source = source_->evaluate(state);

    state.setVariable(targetVar_, source);

    // Assignment return the assigned value as a return value.
    return source;
}