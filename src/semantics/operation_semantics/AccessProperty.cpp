#include "AccessProperty.hpp"
using namespace semantics;

AccessProperty::AccessProperty(std::shared_ptr<ASTNode> expression, std::string memberName)
    : exp_(expression), memberName_(memberName) {}

core::Object* AccessProperty::evaluate(runtime::Frame& state) const {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* object = exp_->evaluate(state);
    guard.protect(object);

    // Retrieve the object's attribute
    return object->getAttribute(memberName_);
}

void AccessProperty::assign(runtime::Frame& state, core::Object* value)  {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* object = exp_->evaluate(state);
    guard.protect(object);
    
    object->setAttribute(memberName_, value);
}
