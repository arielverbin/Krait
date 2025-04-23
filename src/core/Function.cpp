#include "Function.hpp"
#include "String.hpp"
#include "exceptions/exceptions.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "semantics/Const.hpp"
#include <iostream>
using namespace core;

Function::Function(NativeFunc nativeFunc, size_t numArgs) 
        : isBuiltIn_(true), nativeFunc_(nativeFunc), numArgs_(numArgs) {}

Function::Function(std::shared_ptr<semantics::ASTNode> body, std::vector<std::string> params, runtime::Environment closure) 
    : isBuiltIn_(false), body_(body), params_(params), closure_(std::move(closure)) {}

std::shared_ptr<Object> Function::_call_(std::vector<std::shared_ptr<Object>> args) {
    UNREFERENCED(body_);

    size_t expectedNumArgs = isBuiltIn_ ? numArgs_ : params_.size();

    if (expectedNumArgs != args.size()) {
        throw except::InvalidArgumentException("Function call with incorrect number of arguments (expected " +
            std::to_string(expectedNumArgs) + ", got " + std::to_string(args.size()) + ").");
    }

    if (isBuiltIn_) {
        throw semantics::ReturnSignal(nativeFunc_(args));
    }

    runtime::Environment evalScope = closure_.createChildEnvironment();
    for (size_t i = 0; i < params_.size(); ++i) {
        evalScope.defineVariable(params_[i], args[i]);
    }

    // Here evaluate is responsible for throwing the return value, we don't need to 'return' anything.
    // If it doesnt throw anything, we will return its evaluation, but FunctionCall semantics ignore that, and 
    // evaluates to None.
    return body_->evaluate(evalScope);
}

size_t Function::numArgs() {
    return isBuiltIn_ ? numArgs_ : params_.size();
}

std::string Function::_type_() {
    return "Function";
}

std::shared_ptr<Object> Function::_str_() {
    std::ostringstream oss;
    oss << "<Function (" << (isBuiltIn_ ? "" : "non-") << "built-in)>";
    return std::make_shared<String>(oss.str());
 }
 