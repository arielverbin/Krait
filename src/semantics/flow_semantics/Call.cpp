#include "Call.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "runtime/EvalContext.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "semantics/Print.hpp"
#include "semantics/Const.hpp"
using namespace semantics;

Call::Call(std::shared_ptr<ASTNode> callee, std::vector<std::shared_ptr<ASTNode>> args)
    : callee_(std::move(callee)), args_(std::move(args)) {}

core::Object* Call::evaluate(runtime::Frame& state) const {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* callee = callee_->evaluate(state);
    guard.protect(callee);

    std::vector<core::Object*> args;

    // Evaluate all arguments
    for (const auto& arg : args_) {
        core::Object* a = arg->evaluate(state);
        args.push_back(a);
        guard.protect(a);
    }
    
    return callee->call(args);
}

core::Object* Call::makeCall(core::Function* func, core::CallArgs args) {
    runtime::EvalContext::push(func->closure_->context());
    func->closure_->pushNewScope();

    for (size_t i = 0; i < func->params_.size(); ++i) {
        func->closure_->defineVariable(func->params_[i], args[i + 1]);
    }

    core::Object* returnValue = core::None::getNone();
    
    try { func->body_->evaluate(*func->closure_); }  // execute the function
    catch (const semantics::ReturnSignal& ret) {  returnValue = ret.value(); }

    func->closure_->popLastScope();
    runtime::EvalContext::pop();

    return returnValue;
}