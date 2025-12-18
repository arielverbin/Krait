#include "Variable.hpp"
#include "exceptions/exceptions.hpp"
using namespace semantics;

Variable::Variable(std::string targetVar)
    : targetVar_(targetVar) {}

core::Object* Variable::evaluate(runtime::Frame& state) const {
    // runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* var = state.getVariable(targetVar_);

    return (var != nullptr) ? var : throw except::VariableNotFoundException(targetVar_);
 }

void Variable::assign(runtime::Frame& state, core::Object* value) {
    // runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    state.defineVariable(targetVar_, value); // ??, why not setVariable?
}
