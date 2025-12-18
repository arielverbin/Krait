#include "TypeObject.hpp"
#include "exceptions/exceptions.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "core/builtins/builtin_types/ClassMethod.hpp"
#include "core/builtins/KraitBuiltins.hpp"
using namespace core;


TypeObject::TypeObject(std::string name, Function::NativeFunc creator) 
    : Object(KraitBuiltins::typeType), name_(std::move(name)) {
        if (creator != nullptr) {
            setAttribute(__NEW__, gc::make_guarded<core::ClassMethod>(gc::make_guarded<core::Function>(creator)));
        }
}

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
    return gc::make_guarded<String>("<type '" + self->name() + "'>");
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

    if (self->hasAttribute(__INIT__))
        newInstance->initialize(CallArgs(args.begin() + 1, args.end()));
    
    return newInstance;
}
Object* TypeObject::call(const CallArgs& args) {
    CallArgs fullArgs{ this };
    fullArgs.insert(fullArgs.end(), args.begin(), args.end());
    return TypeObject::callOp(fullArgs);
}