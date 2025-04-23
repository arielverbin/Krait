#include "BinaryOp.hpp"
#include "core/None.hpp"
#include "semantics/signal_semantics/Signal.hpp"
using namespace semantics;

std::map<BinaryOpType, std::string> BinaryOp::functionTypeMap_ = {
    { BinaryOpType::Sum,  "_add_"  },
    { BinaryOpType::Sub,  "_sub_"  },
    { BinaryOpType::Mult, "_mult_" },
    { BinaryOpType::Div,  "_div_"  },
    { BinaryOpType::Mod,  "_mod_" },
    { BinaryOpType::GreaterEqual, "_ge_" },
    { BinaryOpType::GreaterThan, "_gt_" },
    { BinaryOpType::LesserEqual, "_le_" },
    { BinaryOpType::LesserThan, "_lt_" },
    { BinaryOpType::Equal, "_eq_" },
    { BinaryOpType::NotEqual, "_neq_" },
    { BinaryOpType::And, "_and_" },
    { BinaryOpType::Or, "_or_" },
};

BinaryOp::BinaryOp(BinaryOpType type, std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp)
    : type_(type), firstExp_(std::move(firstExp)), secExp_(std::move(secExp)) {}

std::shared_ptr<core::Object> BinaryOp::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> firstValue = firstExp_->evaluate(state);
    std::shared_ptr<core::Object> secondValue = secExp_->evaluate(state);

    // Retrieve the current object's implementation of the operation.
    std::shared_ptr<core::Object> operation = firstValue->_att_(BinaryOp::functionTypeMap_[type_]);
    try {
        operation->_call_(std::vector<std::shared_ptr<core::Object>>{ secondValue });
    } catch (const ReturnSignal& returnSignal) {
        return returnSignal.value();
    }
    return core::None::getNone();
}
