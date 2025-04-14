#include "Function.hpp"
#include "core/Function.hpp"
#include "core/None.hpp"
using namespace semantics;

Function::Function(std::string funcName, std::vector<std::string> params, std::shared_ptr<ASTNode> code)
    : funcName_(std::move(funcName)), params_(std::move(params)), code_(code) {}

std::shared_ptr<core::Object> Function::evaluate(runtime::Environment& state) {
    // Create a duplicated environment for the function
    runtime::Environment funcEnv(state);
    std::shared_ptr<core::Function> func = std::make_shared<core::Function>(code_, params_, funcEnv);

    // Store the function in the environment
    state.setVariable(funcName_, func);

    return core::None::getNone();
}