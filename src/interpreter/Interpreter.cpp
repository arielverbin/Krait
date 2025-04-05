#include "Interpreter.hpp"
using namespace interpreter;

runtime::GlobalState& Interpreter::interpret(runtime::GlobalState& state,
                                std::vector<std::shared_ptr<semantics::ASTNode>> code) {
                                    
    for (std::shared_ptr<semantics::ASTNode> command : code) {
        command->evaluate(state);
    }

    return state;
}