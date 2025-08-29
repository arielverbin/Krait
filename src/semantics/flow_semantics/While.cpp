#include "While.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "semantics/signal_semantics/Signal.hpp"
using namespace semantics;

While::While(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> block)
    : condition_(std::move(cond)), block_(std::move(block)) {}


core::Object* While::evaluate(runtime::Environment& state) const {
    while (*condition_->evaluate(state)->toBool()) {
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
