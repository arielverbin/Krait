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
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"
#include "core/TypeObject.hpp"
#include "utils/utils.hpp"

using namespace core;

Object::Object(std::shared_ptr<TypeObject> type) : type_(type), members_(std::make_shared<utils::Scope>()) {
    // All objects expose their type objects through the '__class__' attribute.
    setAttribute("__class__", type_);
}

std::shared_ptr<TypeObject> Object::type() {
    return type_;
}

std::shared_ptr<utils::Scope> Object::getScope() {
    return members_;
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
    if (type_->hasAttribute("__bool__")) {
        std::shared_ptr<Object> boolean = getAttribute("__bool__")->call({});
        if (std::shared_ptr<core::Boolean> b = std::dynamic_pointer_cast<core::Boolean>(boolean)) {
            return b;
        }
        throw except::TypeError("'__bool__' returned non-bool (type '" + boolean->type()->name() + "')");
    }
    
    // all objects return True by default, except None
    return Boolean::get(type_ != KraitBuiltins::noneType);
}

std::shared_ptr<Object> Object::add(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__add__");
    if (!attribute) throw except::NotImplementedException("__add__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
std::shared_ptr<Object> Object::reversedAdd(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__radd__");
    if (!attribute) throw except::NotImplementedException("__radd__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::subtract(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__sub__");
    if (!attribute) throw except::NotImplementedException("__sub__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
std::shared_ptr<Object> Object::reversedSubtract(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__rsub__");
    if (!attribute) throw except::NotImplementedException("__rsub__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::multiply(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__mult__");
    if (!attribute) throw except::NotImplementedException("__mult__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
std::shared_ptr<Object> Object::reversedMultiply(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__rmult__");
    if (!attribute) throw except::NotImplementedException("__rmult__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::divide(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__div__");
    if (!attribute) throw except::NotImplementedException("__div__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
std::shared_ptr<Object> Object::reversedDivide(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__rdiv__");
    if (!attribute) throw except::NotImplementedException("__rdiv__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::modulu(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__mod__");
    if (!attribute) throw except::NotImplementedException("__mod__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
std::shared_ptr<Object> Object::reversedModulu(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__rmod__");
    if (!attribute) throw except::NotImplementedException("__rmod__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::negate() {
    std::shared_ptr<Object> attribute = findAttribute("__neg__");
    if (!attribute) throw except::NotImplementedException("__neg__ does not exists for type '" + type()->name() + "'");
    return attribute->call({});
}

std::shared_ptr<Object> Object::greaterEqual(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__ge__");
    if (!attribute) throw except::NotImplementedException("__ge__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::greater(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__gt__");
    if (!attribute) throw except::NotImplementedException("__gt__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::lesserEqual(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__le__");
    if (!attribute) throw except::NotImplementedException("__le__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::lesser(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__lt__");
    if (!attribute) throw except::NotImplementedException("__lt__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::equal(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__eq__");
    if (!attribute) throw except::NotImplementedException("__eq__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::notEqual(std::shared_ptr<Object> another) {
    std::shared_ptr<Object> attribute = findAttribute("__neq__");
    if (!attribute) throw except::NotImplementedException("__neq__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

std::shared_ptr<Object> Object::createNew(const CallArgs& args) {
    std::shared_ptr<Object> attribute = findAttribute("__new__");
    if (!attribute) throw except::NotImplementedException("__new__ does not exists for type '" + type()->name() + "'");
    return attribute->call(args);
}

std::shared_ptr<Object> Object::initialize(const CallArgs& args) {
    std::shared_ptr<Object> attribute = findAttribute("__init__");
    if (!attribute) throw except::NotImplementedException("__init__ does not exists for type '" + type()->name() + "'");
    return attribute->call(args);
}

std::shared_ptr<Object> Object::call(const CallArgs& args) {
    try {
        std::shared_ptr<Object> attribute = findAttribute("__call__");
        if (!attribute) throw except::NotImplementedException("__call__ does not exists for type '" + type()->name() + "'");
        attribute->call({ args });
    } catch (const semantics::ReturnSignal& ret) {
        return ret.value();
    }
    return None::getNone();
}

std::shared_ptr<Object> Object::get(std::shared_ptr<Object> instance, std::shared_ptr<TypeObject> owner) {
    if (!type_->hasAttribute("__get__"))
        throw except::NotImplementedException("__get__ does not exists for type '" + type()->name() + "'");

    auto fnRaw = type_->getAttributeRaw("__get__");
    return fnRaw->call({ _shared_from_this(), instance, owner });
}

bool Object::hasAttribute(const std::string& varName) {
    utils::Scope& members = *members_;
    return members.find(varName) != members.end();
}

std::shared_ptr<Object> Object::getAttributeRaw(const std::string& varName) {
    utils::Scope& members = *members_;

    // TODO: even in raw, check if the properly has a __get__ function, if so - call it.
    auto member = members.find(varName);
    if (member == members.end()) throw except::AttributeError(
        "'" + type_->name() + "' object has no attribute '" + varName + "'");
    
    if (std::holds_alternative<utils::LazyValue>(member->second)) {
        // It's a LazyValue — resolve and cache
        utils::LazyValue& lazy = std::get<utils::LazyValue>(member->second);
        std::shared_ptr<Object> resolved = lazy.creator();
        member->second = resolved;
    }

    return std::get<std::shared_ptr<Object>>(member->second);
}

std::shared_ptr<Object> Object::findAttribute(const std::string& varName) { 
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

    return nullptr;
}

std::shared_ptr<Object> Object::getAttribute(const std::string& varName) { 
    std::shared_ptr<Object> ob = findAttribute(varName);
    if (!ob) throw except::AttributeError("'" + type_->name() + "' object has no attribute '" + varName + "'");
    return ob;
}

void Object::setAttribute(const std::string& varName, utils::MemberEntry value) {
    utils::Scope& members = *members_;

    members[varName] = value;
}
