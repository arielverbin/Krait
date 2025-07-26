#include <iostream>
#include "Interpreter.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include "core/builtins/builtin_types/Function.hpp"
using namespace interpreter;

Interpreter::Interpreter() : state_(std::make_shared<runtime::Environment>()) {
    core::KraitBuiltins::initializeBuiltins();
    
    // Initialize the global scope.
    state_->pushNewScope();

    // Initialize built-in types.
    state_->defineVariable("none", core::KraitBuiltins::noneType);
    state_->defineVariable("bool", core::KraitBuiltins::boolType);
    state_->defineVariable("float", core::KraitBuiltins::floatType);
    state_->defineVariable("str", core::KraitBuiltins::stringType);
    state_->defineVariable("function", core::KraitBuiltins::functionType);
    state_->defineVariable("type", core::TypeObject::typeType);
    state_->defineVariable("method", core::KraitBuiltins::methodType);
    state_->defineVariable("classmethod", core::KraitBuiltins::classMethodType);
    state_->defineVariable("int", core::KraitBuiltins::intType);
}

runtime::Environment& Interpreter::interpret(std::shared_ptr<semantics::ASTNode> command) {
    command->evaluate(*state_);
    return *state_;
}
