#ifndef SEMANTICS_WHILE_HPP
#define SEMANTICS_WHILE_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class While : public ASTNode {
public:
    While(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> block);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~While() = default;

private:
    std::unique_ptr<ASTNode> condition_;
    std::unique_ptr<ASTNode> block_;
};
}

#endif // SEMANTICS_WHILE_HPP
