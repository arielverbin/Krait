#include "AccessIndex.hpp"
using namespace semantics;

AccessIndex::AccessIndex(std::shared_ptr<ASTNode> expression, std::shared_ptr<ASTNode> index)
    : exp_(expression), index_(index) {}

core::Object* AccessIndex::evaluate(runtime::Frame& state) const {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* object = exp_->evaluate(state);
    guard.protect(object);
    core::Object* index = index_->evaluate(state);
    guard.protect(index);

    return object->getItem(index);
}

void AccessIndex::assign(runtime::Frame& state, core::Object* value)  {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* object = exp_->evaluate(state);
    guard.protect(object);
    core::Object* index = index_->evaluate(state);
    guard.protect(index);

    object->setItem(index, value);
}
