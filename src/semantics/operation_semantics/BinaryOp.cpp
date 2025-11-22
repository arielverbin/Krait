#include "BinaryOp.hpp"
#include "core/Object.hpp"
#include "core/TypeObject.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "exceptions/exceptions.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "semantics/signal_semantics/Signal.hpp"
using namespace semantics;

std::map<BinaryOpType, BinaryOp::Method> BinaryOp::functionTypeMap_ = {
    { BinaryOpType::Sum, &BinaryOp::add },
    { BinaryOpType::Sub, &BinaryOp::subtract },
    { BinaryOpType::Mult, &BinaryOp::multiply },
    { BinaryOpType::Div, &BinaryOp::divide },
    { BinaryOpType::Mod, &BinaryOp::modulu },
    { BinaryOpType::GreaterEqual, &BinaryOp::greaterEqual },
    { BinaryOpType::GreaterThan, &BinaryOp::greater },
    { BinaryOpType::LesserEqual, &BinaryOp::lesserEqual },
    { BinaryOpType::LesserThan, &BinaryOp::lesser },
    { BinaryOpType::Equal, &BinaryOp::equal },
    { BinaryOpType::NotEqual, &BinaryOp::notEqual },
    { BinaryOpType::And, &BinaryOp::logicalAnd },
    { BinaryOpType::Or, &BinaryOp::logicalOr },
};

BinaryOp::BinaryOp(BinaryOpType type, std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp)
    : type_(type), firstExp_(std::move(firstExp)), secExp_(std::move(secExp)) {}

core::Object* BinaryOp::evaluate(runtime::Frame& state) const {
    // Retrieve the current implementation of the operation.
    auto method = BinaryOp::functionTypeMap_.at(type_);
    return (this->*method)(state);
}

#define TRY_AND_RETURN(expression) \
try { \
    return (expression); \
} \
catch(const except::NotImplementedException& e) {} \

#define OPERANDS_TYPE_UNSUPPORTED_EXCEPTION(op, type1, type2) \
    except::TypeError(std::string("unsupported operand type(s) for ") + op + ": '" + (type1) + "' and '" + (type2) + "'")

#define OPERAND_TYPE_UNSUPPORTED_EXCEPTION(op, type) \
    except::TypeError(std::string("unsupported operand type(s) for ") + op + ": '" + (type) + "'")

/**
 * This files unifies the main logic of the language. for example, __rXXX__ mechanism
 * (fix this explanation)
 */

// Binary operations
core::Object* BinaryOp::add(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->add(o2));
    TRY_AND_RETURN(o2->reversedAdd(o1));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION("+", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::subtract(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->subtract(o2));
    TRY_AND_RETURN(o2->reversedSubtract(o1));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION("-", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::multiply(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->multiply(o2));
    TRY_AND_RETURN(o2->reversedMultiply(o1));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION("*", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::divide(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->divide(o2));
    TRY_AND_RETURN(o2->reversedDivide(o1));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION("/", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::modulu(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->modulu(o2));
    TRY_AND_RETURN(o2->reversedModulu(o1));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION("%", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::greaterEqual(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->greaterEqual(o2));
    TRY_AND_RETURN(o2->lesserEqual(o1));
    TRY_AND_RETURN(core::Boolean::get(!(*(o1->lesser(o2)->toBool()))));
    TRY_AND_RETURN(core::Boolean::get(!(*(o2->greater(o1)->toBool()))));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION(">=", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::greater(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->greater(o2));
    TRY_AND_RETURN(o2->lesser(o1));
    TRY_AND_RETURN(core::Boolean::get(!(*(o1->lesserEqual(o2)->toBool()))));
    TRY_AND_RETURN(core::Boolean::get(!(*(o2->greaterEqual(o1)->toBool()))));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION(">", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::lesserEqual(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->lesserEqual(o2));
    TRY_AND_RETURN(o2->greaterEqual(o1));
    TRY_AND_RETURN(core::Boolean::get(!(*(o1->greater(o2)->toBool()))));
    TRY_AND_RETURN(core::Boolean::get(!(*(o2->lesser(o1)->toBool()))));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION("<=", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::lesser(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->lesser(o2));
    TRY_AND_RETURN(o2->greater(o1));
    TRY_AND_RETURN(core::Boolean::get(!(*(o1->greaterEqual(o2)->toBool()))));
    TRY_AND_RETURN(core::Boolean::get(!(*(o2->lesserEqual(o1)->toBool()))));

    throw OPERANDS_TYPE_UNSUPPORTED_EXCEPTION("<", o1->type()->name(), o2->type()->name());
}

core::Object* BinaryOp::equal(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->equal(o2));
    TRY_AND_RETURN(o2->equal(o1));
    TRY_AND_RETURN(core::Boolean::get(!(*(o1->notEqual(o2)->toBool()))));
    TRY_AND_RETURN(core::Boolean::get(!(*(o2->notEqual(o1)->toBool()))));

    // '==' operation fallbacks to id check
    return core::Boolean::get(o1 == o2);
}

core::Object* BinaryOp::notEqual(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    core::Object* o2 = secExp_->evaluate(state);
    TRY_AND_RETURN(o1->notEqual(o2));
    TRY_AND_RETURN(o2->notEqual(o1));
    TRY_AND_RETURN(core::Boolean::get(!(*(o1->equal(o2)->toBool()))));
    TRY_AND_RETURN(core::Boolean::get(!(*(o2->equal(o1)->toBool()))));

    // '!=' operation fallbacks to id check
    return core::Boolean::get(o1 != o2);
}

core::Object* BinaryOp::logicalAnd(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    if (! (*(o1->toBool()))) return o1;

    return secExp_->evaluate(state);
}

core::Object* BinaryOp::logicalOr(runtime::Frame& state) const {
    core::Object* o1 = firstExp_->evaluate(state);
    if ((*(o1->toBool()))) return o1;

    return secExp_->evaluate(state);
}