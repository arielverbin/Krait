#include "AccessProperty.hpp"
using namespace semantics;

AccessProperty::AccessProperty(std::shared_ptr<ASTNode> expression, std::string memberName)
    : exp_(expression), memberName_(memberName) {}

core::Object* AccessProperty::evaluate(runtime::Frame& state) const {
    core::Object* object = exp_->evaluate(state);

    // Retrieve the object's attribute
    return object->getAttribute(memberName_);
}

void AccessProperty::assign(runtime::Frame& state, core::Object* value)  {
    core::Object* object = exp_->evaluate(state);
    object->setAttribute(memberName_, value);
}
