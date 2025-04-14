#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>
#include "semantics/ASTNode.hpp"
#include "semantics/flow_semantics/Code.hpp"
#include "runtime/Environment.hpp"

namespace interpreter {
class Interpreter {
public:
    Interpreter(std::vector<std::shared_ptr<semantics::ASTNode>> commands);
    runtime::Environment& interpret();

private:
    std::shared_ptr<runtime::Environment> state_;
    std::vector<std::shared_ptr<semantics::ASTNode>> commands_;
};

} // namespace interpreter

#endif // INTERPRETER_HPP