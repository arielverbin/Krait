#ifndef SEMANTICS_UNARY_OP_HPP
#define SEMANTICS_UNARY_OP_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

enum UnaryOpType {
    Neg,
    Not,
};

class UnaryOp : public ASTNode {
public:
    using Method = std::shared_ptr<core::Object> (core::Object::*)();
    UnaryOp(UnaryOpType type, std::shared_ptr<ASTNode> exp);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

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
