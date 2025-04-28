#include "FunctionDef.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

FunctionDef::FunctionDef(std::string funcName, std::vector<std::string> params, std::shared_ptr<ASTNode> code)
    : funcName_(std::move(funcName)), params_(std::move(params)), code_(std::move(code)) {}

std::shared_ptr<core::Object> FunctionDef::evaluate(runtime::Environment& state) const {
    // Create a duplicated environment for the function
    runtime::Environment funcEnv(state);
    std::shared_ptr<core::Function> func = std::make_shared<core::Function>(code_, params_, funcEnv);

    // Store the function in the environment
    state.setVariable(funcName_, func);

    // return None as function definitions do not return a value
    return core::None::getNone();
}