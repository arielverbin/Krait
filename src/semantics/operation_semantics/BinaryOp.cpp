#include "BinaryOp.hpp"
using namespace semantics;

std::map<BinaryOpType, std::string> BinaryOp::functionTypeMap_ = {
    { BinaryOpType::Sum,  "_add_"  },
    { BinaryOpType::Sub,  "_sub_"  },
    { BinaryOpType::Mult, "_mult_" },
    { BinaryOpType::Div,  "_div_"  },
    { BinaryOpType::GreaterEqual, "_ge_" },
    { BinaryOpType::LesserEqual, "_le_" },
    { BinaryOpType::Equal, "_eq_" },
    { BinaryOpType::And, "_and_" },
    { BinaryOpType::Or, "_or_" },
};

BinaryOp::BinaryOp(BinaryOpType type, std::unique_ptr<ASTNode> firstExp, std::unique_ptr<ASTNode> secExp)
    : type_(type), firstExp_(std::move(firstExp)), secExp_(std::move(secExp)) {}

std::shared_ptr<core::Object> BinaryOp::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> firstValue = firstExp_->evaluate(state);
    std::shared_ptr<core::Object> secondValue = secExp_->evaluate(state);

    // Retrieve the current object's implementation of the operation.
    std::shared_ptr<core::Object> operation = firstValue->_att_(BinaryOp::functionTypeMap_[type_]);
    return operation->_call_(std::vector<std::shared_ptr<core::Object>>{ secondValue });
}
