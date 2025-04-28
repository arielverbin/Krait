#include "BinaryOp.hpp"
#include "core/Object.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "semantics/signal_semantics/Signal.hpp"
using namespace semantics;

std::map<BinaryOpType, BinaryOp::Method> BinaryOp::functionTypeMap_ = {
    { BinaryOpType::Sum, &core::Object::add },
    { BinaryOpType::Sub, &core::Object::subtract },
    { BinaryOpType::Mult, &core::Object::multiply },
    { BinaryOpType::Div, &core::Object::divide },
    { BinaryOpType::Mod, &core::Object::modulu },
    { BinaryOpType::GreaterEqual, &core::Object::greaterEqual },
    { BinaryOpType::GreaterThan, &core::Object::greater },
    { BinaryOpType::LesserEqual, &core::Object::lesserEqual },
    { BinaryOpType::LesserThan, &core::Object::lesser },
    { BinaryOpType::Equal, &core::Object::equal },
    { BinaryOpType::NotEqual, &core::Object::notEqual },
    { BinaryOpType::And, &core::Object::logicalAnd },
    { BinaryOpType::Or, &core::Object::logicalOr },
};

BinaryOp::BinaryOp(BinaryOpType type, std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp)
    : type_(type), firstExp_(std::move(firstExp)), secExp_(std::move(secExp)) {}

std::shared_ptr<core::Object> BinaryOp::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> firstValue = firstExp_->evaluate(state);
    std::shared_ptr<core::Object> secondValue = secExp_->evaluate(state);

    // Retrieve the current object's implementation of the operation.
    auto method = BinaryOp::functionTypeMap_.at(type_);
    return (firstValue.get()->*method)(secondValue);
}
