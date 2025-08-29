#ifndef SEMANTICS_AST_NODE_HPP
#define SEMANTICS_AST_NODE_HPP

#include "runtime/Environment.hpp"
#include "core/Object.hpp"
#include "utils/utils.hpp"
#include <iostream>

namespace semantics {
class ASTNode {
public:
    virtual core::Object* evaluate(runtime::Environment& state) const = 0;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const = 0;
    #endif // KRAIT_TESTING

    virtual ~ASTNode() = default;
};

class AssignableASTNode : public ASTNode {
public:
    virtual void assign(runtime::Environment& state, core::Object* value) = 0;

    virtual ~AssignableASTNode() = default;
};

} // namespace semantics

#endif // SEMANTICS_AST_NODE_HPP
