#include "Call.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

Call::Call(std::shared_ptr<ASTNode> callee, std::vector<std::shared_ptr<ASTNode>> args)
    : callee_(std::move(callee)), args_(std::move(args)) {}

std::shared_ptr<core::Object> Call::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> callee = callee_->evaluate(state);
    std::vector<std::shared_ptr<core::Object>> args;

    // Evaluate all arguments
    for (const auto& arg : args_) {
        args.push_back(arg->evaluate(state));
    }

    return callee->call(args);
}