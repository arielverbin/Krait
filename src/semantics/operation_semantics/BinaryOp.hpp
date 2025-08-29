#ifndef SEMANTICS_BINARY_OP_HPP
#define SEMANTICS_BINARY_OP_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

enum BinaryOpType {
    Sum, Sub, Mult, Div, Mod,
    GreaterEqual, GreaterThan, LesserEqual, LesserThan,
    Equal, NotEqual,
    And, Or,
};

class BinaryOp : public ASTNode {
public:
    using Method = core::Object* (BinaryOp::*)(runtime::Environment& state) const;
    BinaryOp(BinaryOpType type, std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp);

    virtual core::Object* evaluate(runtime::Environment& state) const override;

    core::Object* add(runtime::Environment& state) const;
    core::Object* subtract(runtime::Environment& state) const;
    core::Object* multiply(runtime::Environment& state) const;
    core::Object* divide(runtime::Environment& state) const;
    core::Object* modulu(runtime::Environment& state) const;
    core::Object* greaterEqual(runtime::Environment& state) const;
    core::Object* greater(runtime::Environment& state) const;
    core::Object* lesserEqual(runtime::Environment& state) const;
    core::Object* lesser(runtime::Environment& state) const;
    core::Object* equal(runtime::Environment& state) const;
    core::Object* notEqual(runtime::Environment& state) const;
    core::Object* logicalAnd(runtime::Environment& state) const;
    core::Object* logicalOr(runtime::Environment& state) const;

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
