#include "Object.hpp"

#include <sstream>
#include <iostream>
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "core/builtins/builtin_types/BoundMethod.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "exceptions/exceptions.hpp"
#include "core/TypeObject.hpp"
#include "utils/utils.hpp"

using namespace core;

Object::Object(std::shared_ptr<TypeObject> type) : type_(type) {}

std::string Object::_type_() {
    return "object";
}

std::shared_ptr<TypeObject> Object::type() {
    return type_;
}

std::shared_ptr<String> Object::toString() {
    if (!type_->hasAttribute("__str__")) {
        // all objects must have a string representation
        std::ostringstream oss;
        oss <<"<'" + type_->name() + "' object at " <<  this << + ">";
        return std::make_shared<String>(oss.str());
    }

    std::shared_ptr<Object> string = getTypeAttribute("__str__")->call({});
    if (std::shared_ptr<core::String> s = std::dynamic_pointer_cast<core::String>(string)) {
        return s;
    }
    throw except::TypeError("'__str__' returned non-string (type '" + string->type()->name() + "')");
}

std::shared_ptr<Boolean> Object::toBool() {
    std::shared_ptr<Object> boolean = getTypeAttribute("__bool__")->call({});
    if (std::shared_ptr<core::Boolean> b = std::dynamic_pointer_cast<core::Boolean>(boolean)) {
        return b;
    }
    throw except::TypeError("'__bool__' returned non-bool (type '" + boolean->type()->name() + "')");
}

std::shared_ptr<Object> Object::add(std::shared_ptr<Object> another) {
    return getTypeAttribute("__add__")->call({ another });
}

std::shared_ptr<Object> Object::subtract(std::shared_ptr<Object> another) {
    return getTypeAttribute("__sub__")->call({ another });
}

std::shared_ptr<Object> Object::multiply(std::shared_ptr<Object> another) {
    return getTypeAttribute("__mult__")->call({ another });
}

std::shared_ptr<Object> Object::divide(std::shared_ptr<Object> another) {
    return getTypeAttribute("__div__")->call({ another });
}

std::shared_ptr<Object> Object::modulu(std::shared_ptr<Object> another) {
    return getTypeAttribute("__mod__")->call({ another });
}

std::shared_ptr<Object> Object::negate() {
    return getTypeAttribute("__neg__")->call({});
}

std::shared_ptr<Object> Object::greaterEqual(std::shared_ptr<Object> another) {
    return getTypeAttribute("__ge__")->call({ another });
}

std::shared_ptr<Object> Object::greater(std::shared_ptr<Object> another) {
    return getTypeAttribute("__gt__")->call({ another });
}

std::shared_ptr<Object> Object::lesserEqual(std::shared_ptr<Object> another) {
    return getTypeAttribute("__le__")->call({ another });
}

std::shared_ptr<Object> Object::lesser(std::shared_ptr<Object> another) {
    return getTypeAttribute("__lt__")->call({ another });
}

std::shared_ptr<Object> Object::equal(std::shared_ptr<Object> another) {
    return getTypeAttribute("__eq__")->call({ another });
}

std::shared_ptr<Object> Object::notEqual(std::shared_ptr<Object> another) {
    return getTypeAttribute("__neq__")->call({ another });
}

std::shared_ptr<Object> Object::logicalOr(std::shared_ptr<Object> another) {
    return getTypeAttribute("__or__")->call({ another });
}

std::shared_ptr<Object> Object::logicalAnd(std::shared_ptr<Object> another) {
    return getTypeAttribute("__and__")->call({ another });
}

std::shared_ptr<Object> Object::logicalNot() {
    return getTypeAttribute("__not__")->call({ });
}

std::shared_ptr<Object> Object::call(const CallArgs& args) {
    try {
        return getTypeAttribute("__call__")->call({ args });
    } catch (const semantics::ReturnSignal& ret) {
        return ret.value();
    }
    return None::getNone();
}

std::shared_ptr<Object> Object::get(std::shared_ptr<Object> instance) {
    auto fnRaw = type_->getAttributeRaw("__get__");
    if (!fnRaw)
        throw except::AttributeError("__get__ not found on descriptor");

    return fnRaw->call({ _shared_from_this(), instance });
}

bool Object::hasAttribute(const std::string& varName) {
    return members_.find(varName) != members_.end();
}

std::shared_ptr<Object> Object::getAttributeRaw(const std::string& varName) {
    auto member = members_.find(varName);
    if (member == members_.end()) throw except::AttributeError(
        "'" + type_->name() + "' object has no attribute '" + varName + "'");
    
    if (std::holds_alternative<std::shared_ptr<Object>>(member->second)) {
        return std::get<std::shared_ptr<Object>>(member->second);
    }

     // It's a LazyValue — resolve and cache
     utils::LazyValue& lazy = std::get<utils::LazyValue>(member->second);
     std::shared_ptr<Object> resolved = lazy.creator();
     member->second = resolved;
     return resolved;
}

std::shared_ptr<Object> Object::getTypeAttribute(const std::string& varName) {
    if (type_->hasAttribute(varName)) {
        std::shared_ptr<Object> descriptor = type_->getAttributeRaw(varName);
        if (descriptor->type()->hasAttribute("__get__")) {
                std::shared_ptr<Object> bound = descriptor->get(_shared_from_this());

                // cache and return
                setAttribute(varName, bound);
                return bound;
        }
    }
    throw except::AttributeError("'" + type_->name() + "' object has no attribute '" + varName + "'");
}

std::shared_ptr<Object> Object::getAttribute(const std::string& varName) {
    if (hasAttribute(varName)) return getAttributeRaw(varName);
    return getTypeAttribute(varName);
}

void Object::setAttribute(const std::string& varName, MemberEntry value) {
    members_[varName] = value;
}
