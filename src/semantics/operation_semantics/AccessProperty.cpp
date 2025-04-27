#include "AccessProperty.hpp"
using namespace semantics;

AccessProperty::AccessProperty(std::shared_ptr<ASTNode> expression, std::string memberName)
    : exp_(expression), memberName_(memberName) {}

std::shared_ptr<core::Object> AccessProperty::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> object = exp_->evaluate(state);

    // Retrieve the object's attribute
    return object->getAttribute(memberName_);
}

void AccessProperty::assign(runtime::Environment& state, std::shared_ptr<core::Object> value)  {
    std::shared_ptr<core::Object> object = exp_->evaluate(state);
    object->setAttribute(memberName_, value);
}
