#include "Function.hpp"
#include "String.hpp"
#include "exceptions/exceptions.hpp"
#include <iostream>
using namespace core;

Function::Function(NativeFunc nativeFunc) 
        : isBuiltIn_(true), nativeFunc_(nativeFunc) {}

Function::Function(std::shared_ptr<semantics::ASTNode> body, std::vector<std::string> params, runtime::Environment closure) 
    : isBuiltIn_(false), body_(body), params_(params), closure_(std::move(closure)) {}

std::shared_ptr<Object> Function::_call_(std::vector<std::shared_ptr<Object>> args) {
    UNREFERENCED(body_);

    if (isBuiltIn_) {
        return nativeFunc_(args);
    }

    if (args.size() != params_.size()) {
        throw except::InvalidArgumentException("Function call with incorrect number of arguments (expected " +
            std::to_string(params_.size()) + ", got " + std::to_string(args.size()) + ").");
    }

    runtime::Environment evalScope = closure_.createChildEnvironment();
    for (size_t i = 0; i < params_.size(); ++i) {
        evalScope.defineVariable(params_[i], args[i]);
    }

    return body_->evaluate(evalScope);
}

std::shared_ptr<Object> Function::_str_() {
    std::ostringstream oss;
    oss << "<Function (" << (isBuiltIn_ ? "" : "non-") << "built-in)>";
    return std::make_shared<String>(oss.str());
 }
 