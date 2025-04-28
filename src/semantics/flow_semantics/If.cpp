#include "If.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

If::If(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> trueBlock, std::shared_ptr<ASTNode> falseBlock)
    : condition_(std::move(cond)), trueBlock_(std::move(trueBlock)), falseBlock_(std::move(falseBlock)) {}


std::shared_ptr<core::Object> If::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Boolean> computed_condition = condition_->evaluate(state)->toBool();

    // state.pushNewScope();
    if (*computed_condition) {
        trueBlock_->evaluate(state);
    } else {
        falseBlock_->evaluate(state);
    }
    // state.popLastScope();

    return core::None::getNone();
}
