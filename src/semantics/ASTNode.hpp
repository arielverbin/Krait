#ifndef SEMANTICS_AST_NODE_HPP
#define SEMANTICS_AST_NODE_HPP

#include "../runtime/GlobalState.hpp"
#include "../core/Object.hpp"
#include "../utils/utils.hpp"
#include <iostream>

namespace semantics {
class ASTNode {
public:
    virtual std::shared_ptr<core::Object> evaluate(runtime::GlobalState& state) = 0;
};

} // namespace semantics

#endif // SEMANTICS_AST_NODE_HPP