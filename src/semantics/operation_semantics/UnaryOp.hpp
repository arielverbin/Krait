#ifndef SEMANTICS_UNARY_OP_HPP
#define SEMANTICS_UNARY_OP_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

enum UnaryOpType {
    Neg,
    Not,
    Bool,
};

class UnaryOp : public ASTNode {
public:
    UnaryOp(UnaryOpType type, std::shared_ptr<ASTNode> exp);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "UnaryOp(" + functionTypeMap_[type_] + ", " + exp_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~UnaryOp() = default;
    
private:
    UnaryOpType type_;
    std::shared_ptr<ASTNode> exp_;

    static std::map<UnaryOpType, std::string> functionTypeMap_;
};

} // namespace semantics

#endif // SEMANTICS_UNARY_OP_HPP
