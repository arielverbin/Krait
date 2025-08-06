#include "Method.hpp"
#include "String.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"
#include "core/builtins/KraitBuiltins.hpp"

using namespace core;

Method::Method(std::shared_ptr<Object> instance,
                         std::shared_ptr<Object> callable)
    : utils::EnableSharedFromThis<Object, Method>(KraitBuiltins::methodType),
      instance_(instance), callable_(callable) {}

std::shared_ptr<Object> Method::callOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException(
            "method.__call__ requires at least 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<Method>(args[0]);
    if (!self)
        throw except::TypeError("first argument to method.__call__ must be a method");
    
    // Prepend the bound instance, then forward to underlying callable
    CallArgs newArgs(args.begin() + 1, args.end());
    newArgs.insert(newArgs.begin(), self->instance_);
    return self->callable_->call(newArgs);
}
std::shared_ptr<Object> Method::call(const CallArgs&  args) {
    CallArgs fullArgs{ _shared_from_this() };
    fullArgs.insert(fullArgs.end(), args.begin(), args.end());
    return Method::callOp(fullArgs);
}

std::shared_ptr<Object> Method::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "method.__str__ requires exactly 1 argument (got " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<Method>(args[0]);
    if (!self)
        throw except::TypeError("first argument to method.__str__ must be a method");
    
    std::ostringstream oss;
    oss << "<" << self->type()->name() << " for type '" << self->instance_->type()->name();
    oss <<  "' at "  << self.get() << ">";
    return std::make_shared<String>(oss.str());
}
std::shared_ptr<String> Method::toString() {
    return std::dynamic_pointer_cast<String>(Method::toStringOp({ _shared_from_this() }));
}

std::shared_ptr<Object> Method::createNewOp(const CallArgs& args) {
    if (args.size() != 3)
        throw except::InvalidArgumentException(
            "method.__new__ requires exactly 3 arguments (got " + std::to_string(args.size()) + ")");

    auto self = std::dynamic_pointer_cast<Method>(args[0]);
    if (!self)
        throw except::TypeError("first argument to method.__str__ must be a method");

    auto func = std::dynamic_pointer_cast<Function>(args[0]);
    if (!func) throw except::TypeError("method.__new__ expected second argument to be a function");

    return func->get(args[2], args[2]->type());
}