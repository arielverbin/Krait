#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>
#include "semantics/ASTNode.hpp"
#include "semantics/flow_semantics/Code.hpp"
#include "runtime/Environment.hpp"

namespace interpreter {
class Interpreter {
public:
    Interpreter();
    runtime::Environment& interpret(std::shared_ptr<semantics::ASTNode> command);

private:
    runtime::Environment* state_;
    std::vector<std::shared_ptr<semantics::ASTNode>> commands_;
};

} // namespace interpreter

#endif // INTERPRETER_HPP