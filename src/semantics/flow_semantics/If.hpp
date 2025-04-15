#ifndef SEMANTICS_IF_HPP
#define SEMANTICS_IF_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class If : public ASTNode {
public:
    If(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> trueBlock, std::unique_ptr<ASTNode> falseBlock);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~If() = default;

private:
    std::unique_ptr<ASTNode> condition_;
    std::unique_ptr<ASTNode> trueBlock_;
    std::unique_ptr<ASTNode> falseBlock_;
};
}

#endif // SEMANTICS_IF_HPP
