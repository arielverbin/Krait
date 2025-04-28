#include <iostream>
#include "Interpreter.hpp"
#include "core/builtins/KraitBuiltins.hpp"
using namespace interpreter;

Interpreter::Interpreter() : state_(std::make_shared<runtime::Environment>()) {
    core::KraitBuiltins::initializeBuiltins();
    // Initialize the global scope.
    state_->pushNewScope();
}

runtime::Environment& Interpreter::interpret(std::shared_ptr<semantics::ASTNode> command) {
    command->evaluate(*state_);
    return *state_;
}
