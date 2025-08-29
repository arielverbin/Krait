#include "TypeObject.hpp"
#include "exceptions/exceptions.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "core/builtins/builtin_types/ClassMethod.hpp"
using namespace core;
#include <iostream>


TypeObject::TypeObject(std::string name, Function::NativeFunc creator) 
    : Object(typeType), name_(std::move(name)) {
        if (creator != nullptr) {
            setAttribute("__new__", gc::make_tracked<core::ClassMethod>(gc::make_tracked<core::Function>(creator)));
        }
    }

TypeObject* TypeObject::initType() {
    auto self = gc::make_tracked<TypeObject>("type", nullptr);
    
    // re-initialize type's type object
    self->type_ = self;
    self->setAttribute("__class__", self);
    return self;
}

// Initialize the type 'type'.
TypeObject* TypeObject::typeType = nullptr;

const std::string& TypeObject::name() {
    return name_;
}

Object* TypeObject::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "type.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = dynamic_cast<TypeObject*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to type.__str__ must be a type");
    return gc::make_tracked<String>("<type '" + self->name() + "'>");
}
String* TypeObject::toString() {
    return dynamic_cast<String*>(TypeObject::toStringOp({ this }));
}

Object* TypeObject::callOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException(
            "type.__call__ requires at least 1 argument (received " + std::to_string(args.size()) + ")"
        );
        
    auto self = dynamic_cast<TypeObject*>(args[0]);
    if (!self) throw except::InvalidArgumentException("first argument to type.__call__ must be a type");
    
    Object* newInstance = self->createNew(CallArgs(args.begin() + 1, args.end()));

    if (self->hasAttribute("__init__"))
        newInstance->initialize(CallArgs(args.begin() + 1, args.end()));
    
    return newInstance;
}
Object* TypeObject::call(const CallArgs& args) {
    CallArgs fullArgs{ this };
    fullArgs.insert(fullArgs.end(), args.begin(), args.end());
    return TypeObject::callOp(fullArgs);
}