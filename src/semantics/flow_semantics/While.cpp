#include "While.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "semantics/signal_semantics/Signal.hpp"
using namespace semantics;

While::While(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> block)
    : condition_(std::move(cond)), block_(std::move(block)) {}


core::Object* While::evaluate(runtime::Frame& state) const {    
    while (true) {
        // we can throw the condition object once we evaluate it. Therefore, the guard is defined inside the while scope
        // to flush the condition object (as we no longer need to guard it).
        runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();
        core::Object* condition = condition_->evaluate(state);
        guard.protect(condition);

        if (*condition->toBool()) {
            try {
                block_->evaluate(state);
            } catch (BreakSignal& breakSignal) {
                return core::None::getNone();
            } catch (ContinueSignal& contineSignal) {
                continue;
            }

        } else break;
    }

    return core::None::getNone();
}
