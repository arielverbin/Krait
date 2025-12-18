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

using namespace core;

Object::Object(TypeObject *type) : type_(type), members_(nullptr) {}

TypeObject* Object::type() {
    return type_;
}

Scope* Object::getScope() {
    if (type_ == nullptr) throw except::RuntimeError("can't init scope for object with no type");
    
    if (members_ == nullptr) {
        // lazy scope initialization
        // All objects expose their type objects through the '__class__' attribute.
        // and their dictionary through the '__dict__' attribute.
        members_ = gc::make_guarded<Scope>();
        setAttribute(__CLASS__, type_);
        setAttribute(__DICT__, members_);
    }
    
    return members_;
}

String* Object::toString() {
    if (!type()->hasAttribute(__STR__)) {
        // all objects must have a string representation
        std::ostringstream oss;
        oss <<"<'" + type()->name() + "' object at " <<  this << + ">";
        return gc::make_guarded<String>(oss.str());
    }

    Object* string = getAttribute(__STR__)->call({});
    if (core::String *s = dynamic_cast<core::String*>(string)) {
        return s;
    }
    throw except::TypeError("'" __STR__ "' returned non-string (type '" + string->type()->name() + "')");
}

Boolean* Object::toBool() {
    if (type()->hasAttribute(__BOOL__)) {
        Object* boolean = getTypeAttribute(__BOOL__)->call({});
        if (core::Boolean* b = dynamic_cast<core::Boolean*>(boolean)) {
            return b;
        }
        throw except::TypeError("'" __BOOL__ "' returned non-bool (type '" + boolean->type()->name() + "')");
    }
    
    // all objects return True by default, except None
    return Boolean::get(type() != KraitBuiltins::noneType);
}

Object* Object::add(Object* another) {
    Object* attribute = getTypeAttribute(__ADD__);
    if (!attribute) throw except::NotImplementedException("'" __ADD__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedAdd(Object* another) {
    Object* attribute = getTypeAttribute(__RADD__);
    if (!attribute) throw except::NotImplementedException("'" __RADD__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::subtract(Object* another) {
    Object* attribute = getTypeAttribute(__SUB__);
    if (!attribute) throw except::NotImplementedException("'" __SUB__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedSubtract(Object* another) {
    Object* attribute = getTypeAttribute(__RSUB__);
    if (!attribute) throw except::NotImplementedException("'" __RSUB__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::multiply(Object* another) {
    Object* attribute = getTypeAttribute(__MUL__);
    if (!attribute) throw except::NotImplementedException("'" __MUL__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedMultiply(Object* another) {
    Object* attribute = getTypeAttribute(__RMUL__);
    if (!attribute) throw except::NotImplementedException("'" __RMUL__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::divide(Object* another) {
    Object* attribute = getTypeAttribute(__DIV__);
    if (!attribute) throw except::NotImplementedException("'" __DIV__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedDivide(Object* another) {
    Object* attribute = getTypeAttribute(__RDIV__);
    if (!attribute) throw except::NotImplementedException("'" __RDIV__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::modulu(Object* another) {
    Object* attribute = getTypeAttribute(__MOD__);
    if (!attribute) throw except::NotImplementedException("'" __MOD__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}
Object* Object::reversedModulu(Object* another) {
    Object* attribute = getTypeAttribute(__RMOD__);
    if (!attribute) throw except::NotImplementedException("'" __RMOD__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::negate() {
    Object* attribute = getTypeAttribute(__NEG__);
    if (!attribute) throw except::NotImplementedException("'" __NEG__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({});
}

Object* Object::greaterEqual(Object* another) {
    Object* attribute = getTypeAttribute(__GE__);
    if (!attribute) throw except::NotImplementedException("'" __GE__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::greater(Object* another) {
    Object* attribute = getTypeAttribute(__GT__);
    if (!attribute) throw except::NotImplementedException("'" __GT__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::lesserEqual(Object* another) {
    Object* attribute = getTypeAttribute(__LE__);
    if (!attribute) throw except::NotImplementedException("'" __LE__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::lesser(Object* another) {
    Object* attribute = getTypeAttribute(__LT__);
    if (!attribute) throw except::NotImplementedException("'" __LT__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::equal(Object* another) {
    Object* attribute = getTypeAttribute(__EQ__);
    if (!attribute) throw except::NotImplementedException("'" __EQ__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::notEqual(Object* another) {
    Object* attribute = getTypeAttribute(__NEQ__);
    if (!attribute) throw except::NotImplementedException("'" __NEQ__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call({ another });
}

Object* Object::createNew(const CallArgs& args) {
    Object* attribute = findAttribute(__NEW__);
    if (!attribute) throw except::NotImplementedException("'" __NEW__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call(args);
}

Object* Object::initialize(const CallArgs& args) {
    Object* attribute = findAttribute(__INIT__);
    if (!attribute) throw except::NotImplementedException("'" __INIT__ "' does not exists for type '" + type()->name() + "'");
    return attribute->call(args);
}

Object* Object::call(const CallArgs& args) {
    try {
        Object* attribute = getTypeAttribute(__CALL__);
        if (!attribute) throw except::NotImplementedException("'" __CALL__ "' does not exists for type '" + type()->name() + "'");
        // TODO: if attribute == this, prevent loop
        return attribute->call({ args });
    } catch (const semantics::ReturnSignal& ret) {
        return ret.value();
    }
    return None::getNone();
}

Object* Object::get(Object* instance, Object* owner) {
    if (!type()->hasAttribute(__GET__))
        throw except::NotImplementedException("'" __GET__ "' does not exists for type '" + type()->name() + "'");

    auto fnRaw = type()->getAttributeRaw(__GET__);
    return fnRaw->call({ this, instance, owner });
}

bool Object::hasAttribute(const std::string& varName) {
    return getScope()->hasMember(varName);
}

Object* Object::getAttributeRaw(const std::string& varName, Object* instance) {
    Object* attribute = getScope()->getMember(varName);
    if (!attribute->type()->hasAttribute(__GET__)) {
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

void Object::setAttribute(const std::string& varName, AttributeEntry value) {
    getScope()->setMember(varName, value);
}

std::vector<gc::GCTrackable*> Object::referencees() {
    if (members_ == nullptr) return {};
    return std::vector<gc::GCTrackable*>({members_});
}