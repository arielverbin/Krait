#include "Variable.hpp"
#include "exceptions/exceptions.hpp"
using namespace semantics;

Variable::Variable(std::string targetVar)
    : targetVar_(targetVar) {}

std::shared_ptr<core::Object> Variable::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> var = state.getVariable(targetVar_);

    return (var != nullptr)
        ? var
        : throw except::VariableNotFoundException(targetVar_);

}

void Variable::assign(runtime::Environment& state, std::shared_ptr<core::Object> value) {
    state.setVariable(targetVar_, value);
}
