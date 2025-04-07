#include "Function.hpp"
using namespace core;

Function::Function(NativeFunc nativeFunc) 
        : isBuiltIn_(true), nativeFunc_(nativeFunc) {}

Function::Function(semantics::ASTNode* body, std::vector<std::string> params, std::shared_ptr<runtime::Environment> closure) 
    : isBuiltIn_(false), body_(body), params_(params), closure_(closure) {}

std::shared_ptr<Object> Function::_call_(runtime::Environment& state, 
                                 const std::vector<std::shared_ptr<Object>>& args) {
    
    UNREFERENCED(body_);

    if (isBuiltIn_) {
        return nativeFunc_(state, args);
    }
    // Otherwise, set up a new local scope, bind parameters, and evaluate the body.
    // This is where you’d save the current state/return address if using a call stack.
    // ...
    return nullptr; // placeholder

}

std::string Function::_str_() {
        std::ostringstream oss;
        oss << "<Function (" << (isBuiltIn_ ? "" : "non-") << "built-in)>";
        return oss.str();
    }