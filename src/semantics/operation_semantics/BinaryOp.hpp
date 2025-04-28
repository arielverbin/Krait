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
    using Method = std::shared_ptr<core::Object> (core::Object::*)(std::shared_ptr<core::Object>);
    BinaryOp(BinaryOpType type, std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "BinaryOp(op," + firstExp_->stringify() + ", " + secExp_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~BinaryOp() = default;
    
private:
    BinaryOpType type_;
    std::shared_ptr<ASTNode> firstExp_;
    std::shared_ptr<ASTNode> secExp_;

    static std::map<BinaryOpType, Method> functionTypeMap_;
};

} // namespace semantics

#endif // SEMANTICS_BINARY_OP_HPP
