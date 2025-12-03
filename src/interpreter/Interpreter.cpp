#include <iostream>
#include "Interpreter.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/Function.hpp"

using namespace interpreter;


Interpreter::Interpreter() {
    // Initialize builtin types (int, bool, function, etc...)
    core::KraitBuiltins::initializeBuiltins();

    // Define the global environment as its mark&sweep root
    state_ = new runtime::Frame();
    gc::GarbageCollector::instance().defineRoot(state_);
    
    // Initialize the global scope.
    state_->pushNewScope();

    // Expose buildin types.
    state_->defineVariable("none", core::KraitBuiltins::noneType);
    state_->defineVariable("bool", core::KraitBuiltins::boolType);
    state_->defineVariable("float", core::KraitBuiltins::floatType);
    state_->defineVariable("str", core::KraitBuiltins::stringType);
    state_->defineVariable("function", core::KraitBuiltins::functionType);
    state_->defineVariable("type", core::KraitBuiltins::typeType);
    state_->defineVariable("method", core::KraitBuiltins::methodType);
    state_->defineVariable("classmethod", core::KraitBuiltins::classMethodType);
    state_->defineVariable("int", core::KraitBuiltins::intType);

    state_->defineVariable("collect_garbage", gc::make_tracked<core::Function>(gc::GarbageCollector::collect_garbage));
}

runtime::Frame& Interpreter::interpret(std::shared_ptr<semantics::ASTNode> command) {
    command->evaluate(*state_);
    return *state_;
}

Interpreter::~Interpreter() {
    delete state_;
}
