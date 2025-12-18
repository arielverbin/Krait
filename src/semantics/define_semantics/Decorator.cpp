#include "Decorator.hpp"
using namespace semantics;

Decorator::Decorator(std::shared_ptr<ASTNode> decorator, std::shared_ptr<BindableASTNode> decorated)
    : decorator_(std::move(decorator)), decorated_(std::move(decorated)) {}

std::string Decorator::name() const {
    return decorated_->name();
}

core::Object* Decorator::compute(runtime::Frame& state) const {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* decorator = decorator_->evaluate(state);
    guard.protect(decorator);
    core::Object* decorated = decorated_->evaluate(state);
    guard.protect(decorated);

    return decorator->call({decorated});
}
