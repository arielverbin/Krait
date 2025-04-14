#include "While.hpp"
#include "core/Boolean.hpp"
#include "core/None.hpp"
#include "semantics/signal_semantics/Signal.hpp"
using namespace semantics;

While::While(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> block)
    : condition_(cond), block_(block) {}


std::shared_ptr<core::Object> While::evaluate(runtime::Environment& state) {
    while (*condition_->evaluate(state)->_bool_()) {
        try {
            block_->evaluate(state);
        } catch (BreakSignal& breakSignal) {
            return core::None::getNone();
        } catch (ContinueSignal& contineSignal) {
            continue;
        }
    }

    return core::None::getNone();
}
