#ifndef SEMANTICS_UNARY_OP_HPP
#define SEMANTICS_UNARY_OP_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

enum UnaryOpType {
    Neg, Not,
};

class UnaryOp : public ASTNode {
public:
    using Method = core::Object* (UnaryOp::*)(runtime::Frame& state) const;
    UnaryOp(UnaryOpType type, std::shared_ptr<ASTNode> exp);

    virtual core::Object* evaluate(runtime::Frame& state) const override;

    core::Object* negate(runtime::Frame& state) const;
    core::Object* logicalNot(runtime::Frame& state) const;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "UnaryOp(op, " + exp_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~UnaryOp() = default;
    
private:
    UnaryOpType type_;
    std::shared_ptr<ASTNode> exp_;

    static std::map<UnaryOpType, Method> functionTypeMap_;
};

} // namespace semantics

#endif // SEMANTICS_UNARY_OP_HPP
