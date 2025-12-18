#ifndef SEMANTICS_IF_HPP
#define SEMANTICS_IF_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class If : public ASTNode {
public:
    If(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> trueBlock, std::shared_ptr<ASTNode> falseBlock);

    virtual core::Object* evaluate(runtime::Frame& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "If(" + condition_->stringify() + ", " + trueBlock_->stringify() + ", " + falseBlock_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~If() = default;

private:
    std::shared_ptr<ASTNode> condition_;
    std::shared_ptr<ASTNode> trueBlock_;
    std::shared_ptr<ASTNode> falseBlock_;
};
}

#endif // SEMANTICS_IF_HPP
