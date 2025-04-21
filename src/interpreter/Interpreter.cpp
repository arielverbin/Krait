#include <iostream>
#include "Interpreter.hpp"
using namespace interpreter;

Interpreter::Interpreter(std::vector<std::shared_ptr<semantics::ASTNode>> commands) 
    : state_(std::make_shared<runtime::Environment>()), commands_(commands) {

    // Initialize the global scope.
    state_->pushNewScope();
}

runtime::Environment& Interpreter::interpret() {
    for (auto& command : commands_) {
        command->evaluate(*state_);
    }

    return *state_;
}
