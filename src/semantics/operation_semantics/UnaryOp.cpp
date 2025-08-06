#include "UnaryOp.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
using namespace semantics;

std::map<UnaryOpType, UnaryOp::Method> UnaryOp::functionTypeMap_ = {
    { UnaryOpType::Not,  &UnaryOp::logicalNot },
    { UnaryOpType::Neg,  &UnaryOp::negate }
};

UnaryOp::UnaryOp(UnaryOpType type, std::shared_ptr<ASTNode> exp)
    : type_(type), exp_(std::move(exp)) {}

std::shared_ptr<core::Object> UnaryOp::evaluate(runtime::Environment& state) const {
    // Retrieve the current implementation of the operation.
    auto method = UnaryOp::functionTypeMap_.at(type_);
    return (this->*method)(state);
}

std::shared_ptr<core::Object> UnaryOp::negate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> o = exp_->evaluate(state);
    return o->negate();
}

std::shared_ptr<core::Object> UnaryOp::logicalNot(runtime::Environment& state) const {
    std::shared_ptr<core::Object> o = exp_->evaluate(state);
    return core::Boolean::get(!(*(o->toBool())));
}