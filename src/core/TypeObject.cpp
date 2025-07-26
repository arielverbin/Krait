#include "TypeObject.hpp"
#include "exceptions/exceptions.hpp"
#include "builtins/builtin_types/None.hpp"
using namespace core;
#include <iostream>


TypeObject::TypeObject(std::string name)
    : utils::EnableSharedFromThis<Object, TypeObject>(typeType), name_(std::move(name)) {}

std::shared_ptr<TypeObject> TypeObject::initType() {
    auto self = std::make_shared<TypeObject>("type");
    
    // re-initialize type's type object
    self->type_ = self;
    self->setAttribute("__class__", self);
    return self;
}

// Initialize the type 'type'.
std::shared_ptr<TypeObject> TypeObject::typeType = TypeObject::initType();

const std::string& TypeObject::name() {
    return name_;
}

std::shared_ptr<Object> TypeObject::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "type.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to type.__str__ must be a type");
    return std::make_shared<String>("<type '" + self->name() + "'>");
}
std::shared_ptr<String> TypeObject::toString() {
    return std::dynamic_pointer_cast<String>(TypeObject::toStringOp({ _shared_from_this() }));
}

std::shared_ptr<Object> TypeObject::callOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException(
            "type.__call__ requires at least 1 argument (received " + std::to_string(args.size()) + ")"
        );
    auto self = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!self) throw except::InvalidArgumentException("first argument to type.__call__ must be a type");
    
    std::shared_ptr<Object> newInstance = self->createNew(CallArgs(args.begin() + 1, args.end()));
    
    if (self->hasAttribute("__init__"))
        newInstance->initialize(CallArgs(args.begin() + 1, args.end()));
    
    return newInstance;
}
std::shared_ptr<Object> TypeObject::call(const CallArgs& args) {
    CallArgs fullArgs{ _shared_from_this() };
    fullArgs.insert(fullArgs.end(), args.begin(), args.end());
    return TypeObject::callOp(fullArgs);
}