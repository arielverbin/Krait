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
    using Method = core::Object* (BinaryOp::*)(runtime::Frame& state) const;
    BinaryOp(BinaryOpType type, std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp);

    virtual core::Object* evaluate(runtime::Frame& state) const override;

    core::Object* add(runtime::Frame& state) const;
    core::Object* subtract(runtime::Frame& state) const;
    core::Object* multiply(runtime::Frame& state) const;
    core::Object* divide(runtime::Frame& state) const;
    core::Object* modulu(runtime::Frame& state) const;
    core::Object* greaterEqual(runtime::Frame& state) const;
    core::Object* greater(runtime::Frame& state) const;
    core::Object* lesserEqual(runtime::Frame& state) const;
    core::Object* lesser(runtime::Frame& state) const;
    core::Object* equal(runtime::Frame& state) const;
    core::Object* notEqual(runtime::Frame& state) const;
    core::Object* logicalAnd(runtime::Frame& state) const;
    core::Object* logicalOr(runtime::Frame& state) const;

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
