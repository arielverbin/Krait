#include "FunctionDef.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

FunctionDef::FunctionDef(std::string funcName, std::vector<std::string> params, std::shared_ptr<ASTNode> code)
    : funcName_(std::move(funcName)), params_(std::move(params)), code_(std::move(code)) {}

core::Object* FunctionDef::evaluate(runtime::Frame& state) const {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    // Create a duplicated frame for the function
    // because the original 'state' object could have its outer scopes popped out
    // although they are part of the currently defined function's closure.
    runtime::Frame* funcEnv = gc::make_tracked<runtime::Frame>(state);
    guard.protect(funcEnv);
    
    core::Function* func = gc::make_tracked<core::Function>(code_, params_, funcEnv);

    // Store the function in the environment
    state.defineVariable(funcName_, func);

    // return None as function definitions do not return a value
    return core::None::getNone();
}