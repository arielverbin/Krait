#ifndef SEMANTICS_BINARY_OP_HPP
#define SEMANTICS_BINARY_OP_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

enum BinaryOpType {
    Sum,
    Sub,
    Mult,
    Div,
    GreaterEqual,
    LesserEqual,
    Equal,
    And,
    Or,
};

class BinaryOp : public ASTNode {
public:
    BinaryOp(BinaryOpType type, std::unique_ptr<ASTNode> firstExp, std::unique_ptr<ASTNode> secExp);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~BinaryOp() = default;
private:
    BinaryOpType type_;
    std::unique_ptr<ASTNode> firstExp_;
    std::unique_ptr<ASTNode> secExp_;

    static std::map<BinaryOpType, std::string> functionTypeMap_;
};

} // namespace semantics

#endif // SEMANTICS_BINARY_OP_HPP
