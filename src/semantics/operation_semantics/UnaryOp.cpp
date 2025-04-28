#include "UnaryOp.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

std::map<UnaryOpType, UnaryOp::Method> UnaryOp::functionTypeMap_ = {
    { UnaryOpType::Not,  &core::Object::logicalNot },
    { UnaryOpType::Neg,  &core::Object::negate }
};

UnaryOp::UnaryOp(UnaryOpType type, std::shared_ptr<ASTNode> exp)
    : type_(type), exp_(std::move(exp)) {}

std::shared_ptr<core::Object> UnaryOp::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> firstValue = exp_->evaluate(state);

    // Retrieve the current object's implementation of the operation.
    auto method = UnaryOp::functionTypeMap_.at(type_);
    return (firstValue.get()->*method)();
}
