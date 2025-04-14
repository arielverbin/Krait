#include "Call.hpp"
using namespace semantics;

Call::Call(std::shared_ptr<ASTNode> callee, std::vector<std::shared_ptr<ASTNode>> args)
    : callee_(std::move(callee)), args_(std::move(args)) {}

std::shared_ptr<core::Object> Call::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> callee = callee_->evaluate(state);
    std::vector<std::shared_ptr<core::Object>> args;

    // Evaluate all arguments
    for (const auto& arg : args_) {
        args.push_back(arg->evaluate(state));
    }

    return callee->_call_(args);
}