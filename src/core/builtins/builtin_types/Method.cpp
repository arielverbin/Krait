#include "Method.hpp"
#include "String.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"
#include "core/builtins/KraitBuiltins.hpp"

using namespace core;

Method::Method(Object* instance, Object* callable)
    : Object(KraitBuiltins::methodType), instance_(instance), callable_(callable) {
        setAttribute("__function__", callable_);
        setAttribute("__self__", instance_);
    }

Object* Method::callOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException(
            "method.__call__ requires at least 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = dynamic_cast<Method*>(args[0]);
    if (!self)
        throw except::TypeError("first argument to method.__call__ must be a method");
    
    // Prepend the bound instance, then forward to underlying callable
    CallArgs newArgs(args.begin() + 1, args.end());
    newArgs.insert(newArgs.begin(), self->instance_);
    return self->callable_->call(newArgs);
}
Object* Method::call(const CallArgs&  args) {
    CallArgs fullArgs{ this };
    fullArgs.insert(fullArgs.end(), args.begin(), args.end());
    return Method::callOp(fullArgs);
}

Object* Method::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "method.__str__ requires exactly 1 argument (got " + std::to_string(args.size()) + ")");
    auto self = dynamic_cast<Method*>(args[0]);
    if (!self)
        throw except::TypeError("first argument to method.__str__ must be a method");
    
    std::ostringstream oss;
    oss << "<" << self->type()->name() << " for type '" << self->instance_->type()->name();
    oss <<  "' at "  << self << ">";
    return gc::make_tracked<String>(oss.str());
}
String* Method::toString() {
    return static_cast<String*>(Method::toStringOp({ this }));
}

Object* Method::createNewOp(const CallArgs& args) {
    if (args.size() != 3)
        throw except::InvalidArgumentException(
            "method.__new__ requires exactly 3 arguments (got " + std::to_string(args.size()) + ")");

    auto self = dynamic_cast<Method*>(args[0]);
    if (!self)
        throw except::TypeError("first argument to method.__str__ must be a method");

    auto func = dynamic_cast<Function*>(args[0]);
    if (!func) throw except::TypeError("method.__new__ expected second argument to be a function");

    return func->get(args[2], args[2]->type());
}
