#include "Object.hpp"

#include <sstream>
#include <iostream>
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "exceptions/exceptions.hpp"
#include "core/TypeObject.hpp"
#include "utils/utils.hpp"

using namespace core;

Object::Object(std::shared_ptr<TypeObject> type) : type_(type) {
    // All objects expose their type objects through the '__class__' attribute.
    setAttribute("__class__", type_);
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

    std::shared_ptr<Object> string = getAttribute("__str__")->call({});
    if (std::shared_ptr<core::String> s = std::dynamic_pointer_cast<core::String>(string)) {
        return s;
    }
    throw except::TypeError("'__str__' returned non-string (type '" + string->type()->name() + "')");
}

std::shared_ptr<Boolean> Object::toBool() {
    std::shared_ptr<Object> boolean = getAttribute("__bool__")->call({});
    if (std::shared_ptr<core::Boolean> b = std::dynamic_pointer_cast<core::Boolean>(boolean)) {
        return b;
    }
    throw except::TypeError("'__bool__' returned non-bool (type '" + boolean->type()->name() + "')");
}

std::shared_ptr<Object> Object::add(std::shared_ptr<Object> another) {
    return getAttribute("__add__")->call({ another });
}

std::shared_ptr<Object> Object::subtract(std::shared_ptr<Object> another) {
    return getAttribute("__sub__")->call({ another });
}

std::shared_ptr<Object> Object::multiply(std::shared_ptr<Object> another) {
    return getAttribute("__mult__")->call({ another });
}

std::shared_ptr<Object> Object::divide(std::shared_ptr<Object> another) {
    return getAttribute("__div__")->call({ another });
}

std::shared_ptr<Object> Object::modulu(std::shared_ptr<Object> another) {
    return getAttribute("__mod__")->call({ another });
}

std::shared_ptr<Object> Object::negate() {
    return getAttribute("__neg__")->call({});
}

std::shared_ptr<Object> Object::greaterEqual(std::shared_ptr<Object> another) {
    return getAttribute("__ge__")->call({ another });
}

std::shared_ptr<Object> Object::greater(std::shared_ptr<Object> another) {
    return getAttribute("__gt__")->call({ another });
}

std::shared_ptr<Object> Object::lesserEqual(std::shared_ptr<Object> another) {
    return getAttribute("__le__")->call({ another });
}

std::shared_ptr<Object> Object::lesser(std::shared_ptr<Object> another) {
    return getAttribute("__lt__")->call({ another });
}

std::shared_ptr<Object> Object::equal(std::shared_ptr<Object> another) {
    return getAttribute("__eq__")->call({ another });
}

std::shared_ptr<Object> Object::notEqual(std::shared_ptr<Object> another) {
    return getAttribute("__neq__")->call({ another });
}

std::shared_ptr<Object> Object::logicalOr(std::shared_ptr<Object> another) {
    return getAttribute("__or__")->call({ another });
}

std::shared_ptr<Object> Object::logicalAnd(std::shared_ptr<Object> another) {
    return getAttribute("__and__")->call({ another });
}

std::shared_ptr<Object> Object::logicalNot() {
    return getAttribute("__not__")->call({ });
}

std::shared_ptr<Object> Object::createNew(const CallArgs& args) {
    return getAttribute("__new__")->call(args);
}

std::shared_ptr<Object> Object::initialize(const CallArgs& args) {
    return getAttribute("__init__")->call(args);
}

std::shared_ptr<Object> Object::call(const CallArgs& args) {
    try {
        return getAttribute("__call__")->call({ args });
    } catch (const semantics::ReturnSignal& ret) {
        return ret.value();
    }
    return None::getNone();
}

std::shared_ptr<Object> Object::get(std::shared_ptr<Object> instance, std::shared_ptr<TypeObject> owner) {
    auto fnRaw = type_->getAttributeRaw("__get__");
    if (!fnRaw)
        throw except::AttributeError("__get__ not found on descriptor");

    return fnRaw->call({ _shared_from_this(), instance, owner });
}

bool Object::hasAttribute(const std::string& varName) {
    return members_.find(varName) != members_.end();
}

std::shared_ptr<Object> Object::getAttributeRaw(const std::string& varName) {
    // TODO: even in raw, check if the properly has a __get__ function, if so - call it.
    auto member = members_.find(varName);
    if (member == members_.end()) throw except::AttributeError(
        "'" + type_->name() + "' object has no attribute '" + varName + "'");
    
    if (std::holds_alternative<utils::LazyValue>(member->second)) {
        // It's a LazyValue — resolve and cache
        utils::LazyValue& lazy = std::get<utils::LazyValue>(member->second);
        std::shared_ptr<Object> resolved = lazy.creator();
        member->second = resolved;
    }

    return std::get<std::shared_ptr<Object>>(member->second);
}

std::shared_ptr<Object> Object::getAttribute(const std::string& varName) { 
    auto isClass = [&]() {
        // a 'class' is defined as an object of type 'type'.
        return this->type() == TypeObject::typeType;
    };

    if (!isClass() && hasAttribute(varName)) {
        return getAttributeRaw(varName);
    }

    std::shared_ptr<TypeObject> cls = isClass() ? std::dynamic_pointer_cast<TypeObject>(_shared_from_this()) : this->type();
    if (!cls) throw except::TypeError("class (i.e object of type 'type') must be a 'type'");

    if (cls->hasAttribute(varName)) {
        std::shared_ptr<Object> attribute = cls->getAttributeRaw(varName);
        if (!attribute->type()->hasAttribute("__get__")) {
            return attribute;
        }
        std::shared_ptr<Object> bound = attribute->get(isClass() ? core::None::getNone() : _shared_from_this(), cls);

        // // cache on the instance's own dict and return
        /** TODO: add mechanism to remove the cached result in case the object was changed on the instance's type */
        // setAttribute(varName, bound); 
        return bound;
    }

    /** TODO: fallback to __getattr__ implementation */

    throw except::AttributeError("'" + type_->name() + "' object has no attribute '" + varName + "'");
}

void Object::setAttribute(const std::string& varName, MemberEntry value) {
    members_[varName] = value;
}
