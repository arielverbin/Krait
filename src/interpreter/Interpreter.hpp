#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>
#include "semantics/ASTNode.hpp"
#include "semantics/flow_semantics/Code.hpp"
#include "runtime/Frame.hpp"

namespace interpreter {
class Interpreter {
public:
    Interpreter();
    runtime::Frame& interpret(std::shared_ptr<semantics::ASTNode> command);

    ~Interpreter();

private:
    runtime::Frame* state_;
    std::vector<std::shared_ptr<semantics::ASTNode>> commands_;
};

} // namespace interpreter

#endif // INTERPRETER_HPP