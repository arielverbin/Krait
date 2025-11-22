#include "Object.hpp"

#include <sstream>
#include <iostream>
#include "core/gc/GarbageCollector.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/builtin_types/Scope.hpp"
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

Object::Object(TypeObject *type, bool initScope) : type_(type), members_(initScope ? gc::make_tracked<Scope>() : nullptr) {
    // All objects expose their type objects through the '__class__' attribute.
    if (initScope) setAttribute("__class__", type_);
}

TypeObject* Object::type() {
    return type_;
}

Scope* Object::getScope() {
    return members_;
}

String* Object::toString() {
    if (!type()->hasAttribute("__str__")) {
        // all objects must have a string representation
        std::ostringstream oss;
        oss <<"<'" + type()->name() + "' object at " <<  this << + ">";
        return gc::make_tracked<String>(oss.str());
    }

    Object* string = getAttribute("__str__")->call({});
    if (core::String *s = dynamic_cast<core::String*>(string)) {
        return s;
    }
    throw except::TypeError("'__str__' returned non-string (type '" + string->type()->name() + "')");
}

Boolean* Object::toBool() {
    if (type()->hasAttribute("__bool__")) {
        Object* boolean = getTypeAttribute("__bool__")->call({});
        if (core::Boolean* b = dynamic_cast<core::Boolean*>(boolean)) {
            return b;
        }
        throw except::TypeError("'__bool__' returned non-bool (type '" + boolean->type()->name() + "')");
    }
    
    // all objects return True by default, except None
    return Boolean::get(type() != KraitBuiltins::noneType);
}

Object* Object::add(Object* another) {
    Object* attribute = getTypeAttribute("__add__");
    if (!attribute) throw except::NotImplementedException("__add__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedAdd(Object* another) {
    Object* attribute = getTypeAttribute("__radd__");
    if (!attribute) throw except::NotImplementedException("__radd__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::subtract(Object* another) {
    Object* attribute = getTypeAttribute("__sub__");
    if (!attribute) throw except::NotImplementedException("__sub__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedSubtract(Object* another) {
    Object* attribute = getTypeAttribute("__rsub__");
    if (!attribute) throw except::NotImplementedException("__rsub__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::multiply(Object* another) {
    Object* attribute = getTypeAttribute("__mult__");
    if (!attribute) throw except::NotImplementedException("__mult__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedMultiply(Object* another) {
    Object* attribute = getTypeAttribute("__rmult__");
    if (!attribute) throw except::NotImplementedException("__rmult__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::divide(Object* another) {
    Object* attribute = getTypeAttribute("__div__");
    if (!attribute) throw except::NotImplementedException("__div__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedDivide(Object* another) {
    Object* attribute = getTypeAttribute("__rdiv__");
    if (!attribute) throw except::NotImplementedException("__rdiv__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::modulu(Object* another) {
    Object* attribute = getTypeAttribute("__mod__");
    if (!attribute) throw except::NotImplementedException("__mod__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedModulu(Object* another) {
    Object* attribute = getTypeAttribute("__rmod__");
    if (!attribute) throw except::NotImplementedException("__rmod__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::negate() {
    Object* attribute = getTypeAttribute("__neg__");
    if (!attribute) throw except::NotImplementedException("__neg__ does not exists for type '" + type()->name() + "'");
    return attribute->call({});
}

Object* Object::greaterEqual(Object* another) {
    Object* attribute = getTypeAttribute("__ge__");
    if (!attribute) throw except::NotImplementedException("__ge__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::greater(Object* another) {
    Object* attribute = getTypeAttribute("__gt__");
    if (!attribute) throw except::NotImplementedException("__gt__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::lesserEqual(Object* another) {
    Object* attribute = getTypeAttribute("__le__");
    if (!attribute) throw except::NotImplementedException("__le__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::lesser(Object* another) {
    Object* attribute = getTypeAttribute("__lt__");
    if (!attribute) throw except::NotImplementedException("__lt__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::equal(Object* another) {
    Object* attribute = getTypeAttribute("__eq__");
    if (!attribute) throw except::NotImplementedException("__eq__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::notEqual(Object* another) {
    Object* attribute = getTypeAttribute("__neq__");
    if (!attribute) throw except::NotImplementedException("__neq__ does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::createNew(const CallArgs& args) {
    Object* attribute = findAttribute("__new__");
    if (!attribute) throw except::NotImplementedException("__new__ does not exists for type '" + type()->name() + "'");
    return attribute->call(args);
}

Object* Object::initialize(const CallArgs& args) {
    Object* attribute = findAttribute("__init__");
    if (!attribute) throw except::NotImplementedException("__init__ does not exists for type '" + type()->name() + "'");
    return attribute->call(args);
}

Object* Object::call(const CallArgs& args) {
    try {
        Object* attribute = getTypeAttribute("__call__");
        if (!attribute) throw except::NotImplementedException("__call__ does not exists for type '" + type()->name() + "'");
        attribute->call({ args });
    } catch (const semantics::ReturnSignal& ret) {
        return ret.value();
    }
    return None::getNone();
}

Object* Object::get(Object* instance, Object* owner) {
    if (!type()->hasAttribute("__get__"))
        throw except::NotImplementedException("__get__ does not exists for type '" + type()->name() + "'");

    auto fnRaw = type()->getAttributeRaw("__get__");
    return fnRaw->call({ this, instance, owner });
}

bool Object::hasAttribute(const std::string& varName) {
    return getScope()->hasMember(varName);
}

Object* Object::getAttributeRaw(const std::string& varName, Object* instance) {
    Object* attribute = getScope()->getMember(varName);
    if (!attribute->type()->hasAttribute("__get__")) {
        return attribute;
    }
    Object* bound = attribute->get(instance ? instance : core::None::getNone(), this);

    // // cache on the instance's own dict and return
    /** TODO: add mechanism to remove the cached result in case the object was changed on the instance's type */
    // setAttribute(varName, bound); 
    return bound;
}

Object* Object::findAttribute(const std::string& varName) { 
    return hasAttribute(varName) ? getAttributeRaw(varName, nullptr) : getTypeAttribute(varName);
}

Object* Object::getTypeAttribute(const std::string& varName) {
    if (type()->hasAttribute(varName)) {
        return type()->getAttributeRaw(varName, this); 
    }

    /** TODO: fallback to __getattr__ implementation */

    return nullptr;
}

Object* Object::getAttribute(const std::string& varName) { 
    Object* ob = findAttribute(varName);
    if (!ob) throw except::AttributeError("'" + type()->name() + "' object has no attribute '" + varName + "'");
    return ob;
}

void Object::setAttribute(const std::string& varName, MemberEntry value) {
    getScope()->setMember(varName, value);
}

std::vector<gc::GCTrackable*> Object::referencees() {
    return std::vector<gc::GCTrackable*>();
}