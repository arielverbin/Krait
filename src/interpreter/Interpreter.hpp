#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>
#include "../semantics/ASTNode.hpp"
#include "../runtime/GlobalState.hpp"

namespace interpreter {
class Interpreter {
public:
    static runtime::GlobalState& interpret(runtime::GlobalState& state, std::vector<std::shared_ptr<semantics::ASTNode>> code);
};

} // namespace interpreter

#endif // INTERPRETER_HPP