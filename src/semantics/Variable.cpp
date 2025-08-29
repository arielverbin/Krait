#include "Variable.hpp"
#include "exceptions/exceptions.hpp"
using namespace semantics;

Variable::Variable(std::string targetVar)
    : targetVar_(targetVar) {}

core::Object* Variable::evaluate(runtime::Environment& state) const {
    core::Object* var = state.getVariable(targetVar_);

    return (var != nullptr) ? var : throw except::VariableNotFoundException(targetVar_);
 }

void Variable::assign(runtime::Environment& state, core::Object* value) {
    state.setVariable(targetVar_, value);
}
