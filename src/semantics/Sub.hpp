#ifndef SEMANTICS_SUB_HPP
#define SEMANTICS_SUB_HPP

#include "ASTNode.hpp"

namespace semantics {
class Sub : public ASTNode {
public:
    Sub(std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp);

    virtual std::shared_ptr<core::Object> evaluate(runtime::GlobalState& state) override;

private:
    std::shared_ptr<ASTNode> firstExp_;
    std::shared_ptr<ASTNode> secExp_;
};

} // namespace semantics

#endif // SEMANTICS_SUB_HPP