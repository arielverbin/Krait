#include "Function.hpp"
#include "String.hpp"
using namespace core;

Function::Function(NativeFunc nativeFunc) 
        : isBuiltIn_(true), nativeFunc_(nativeFunc) {}

Function::Function(semantics::ASTNode* body, std::vector<std::string> params, runtime::Environment closure) 
    : isBuiltIn_(false), body_(body), params_(params), closure_(std::move(closure)) {}

std::shared_ptr<Object> Function::_call_(std::vector<std::shared_ptr<Object>> args) {
    
    UNREFERENCED(body_);
    // TODO: argument check here.

    if (isBuiltIn_) {
        return nativeFunc_(args);
    }
    // Otherwise, set up a new local scope, bind parameters, and evaluate the body.
    // This is where you’d save the current state/return address if using a call stack.
    // ...
    return nullptr; // placeholder

}

std::shared_ptr<Object> Function::_str_() {
    std::ostringstream oss;
    oss << "<Function (" << (isBuiltIn_ ? "" : "non-") << "built-in)>";
    return std::make_shared<String>(oss.str());
 }
 