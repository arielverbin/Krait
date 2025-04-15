#include "If.hpp"
#include "core/Boolean.hpp"
#include "core/None.hpp"
using namespace semantics;

If::If(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> trueBlock, std::unique_ptr<ASTNode> falseBlock)
    : condition_(std::move(cond)), trueBlock_(std::move(trueBlock)), falseBlock_(std::move(falseBlock)) {}


std::shared_ptr<core::Object> If::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> computed_condition = condition_->evaluate(state)->_bool_();

    // state.pushNewScope();
    if (*computed_condition) {
        trueBlock_->evaluate(state);
    } else {
        falseBlock_->evaluate(state);
    }
    // state.popLastScope();

    return core::None::getNone();
}
