#include <iostream>
#include "Interpreter.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/Function.hpp"
using namespace interpreter;

Interpreter::Interpreter() : state_(new runtime::Environment()) {
    // Initialize the grabage collector
    gc::GarbageCollector::initialize(state_);
    gc::GarbageCollector::instance().trackObject(state_);

    // Initialize 'type' type.
    core::TypeObject::typeType = core::TypeObject::initType();

    // Initialize builtin types (int, bool, function, etc...)
    core::KraitBuiltins::initializeBuiltins();
    
    // Initialize the global scope.
    state_->pushNewScope();

    // Expose buildin types.
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
