#ifndef SEMANTICS_BINARY_OP_HPP
#define SEMANTICS_BINARY_OP_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

enum BinaryOpType {
    Sum,
    Sub,
    Mult,
    Div,
    Mod,
    GreaterEqual,
    GreaterThan,
    LesserEqual,
    LesserThan,
    Equal,
    NotEqual,
    And,
    Or,
};

class BinaryOp : public ASTNode {
public:
    BinaryOp(BinaryOpType type, std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "BinaryOp(" + functionTypeMap_[type_] + ", " + firstExp_->stringify() + ", " + secExp_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~BinaryOp() = default;
    
private:
    BinaryOpType type_;
    std::shared_ptr<ASTNode> firstExp_;
    std::shared_ptr<ASTNode> secExp_;

    static std::map<BinaryOpType, std::string> functionTypeMap_;
};

} // namespace semantics

#endif // SEMANTICS_BINARY_OP_HPP
