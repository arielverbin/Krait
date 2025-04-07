#include "Variable.hpp"
using namespace semantics;

Variable::Variable(std::string targetVar)
    : targetVar_(targetVar) {}


std::shared_ptr<core::Object> Variable::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> var = state.getVariable(targetVar_);

    return (var != nullptr)
        ? var
        : throw std::runtime_error("[EXCEPTION] Could not find " + targetVar_ + ".");

}