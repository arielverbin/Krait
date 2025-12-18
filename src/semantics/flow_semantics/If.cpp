#include "If.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

If::If(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> trueBlock, std::shared_ptr<ASTNode> falseBlock)
    : condition_(std::move(cond)), trueBlock_(std::move(trueBlock)), falseBlock_(std::move(falseBlock)) {}


core::Object* If::evaluate(runtime::Frame& state) const {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();
    core::Object* condition = condition_->evaluate(state);
    guard.protect(condition);

    core::Boolean* computed_condition = condition->toBool();
    guard.protect(computed_condition);

    // state.pushNewScope();
    if (*computed_condition) {
        trueBlock_->evaluate(state);
    } else {
        falseBlock_->evaluate(state);
    }
    // state.popLastScope();

    return core::None::getNone();
}
