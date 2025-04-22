#include <iostream>
#include "Interpreter.hpp"
using namespace interpreter;

Interpreter::Interpreter() : state_(std::make_shared<runtime::Environment>()) {
    // Initialize the global scope.
    state_->pushNewScope();
}

runtime::Environment& Interpreter::interpret(std::shared_ptr<semantics::ASTNode> command) {
    command->evaluate(*state_);
    return *state_;
}
