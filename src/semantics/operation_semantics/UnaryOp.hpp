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
    UnaryOp(UnaryOpType type, std::unique_ptr<ASTNode> exp);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~UnaryOp() = default;
private:
    UnaryOpType type_;
    std::unique_ptr<ASTNode> exp_;

    static std::map<UnaryOpType, std::string> functionTypeMap_;
};

} // namespace semantics

#endif // SEMANTICS_UNARY_OP_HPP
