#ifndef SEMANTICS_WHILE_HPP
#define SEMANTICS_WHILE_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class While : public ASTNode {
public:
    While(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> block);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~While() = default;

private:
    std::shared_ptr<ASTNode> condition_;
    std::shared_ptr<ASTNode> block_;
};
}

#endif // SEMANTICS_WHILE_HPP
