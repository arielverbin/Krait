#include "BoundMethod.hpp"
#include "String.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"
#include "core/builtins/KraitBuiltins.hpp"

using namespace core;

BoundMethod::BoundMethod(std::shared_ptr<Object> instance,
                         std::shared_ptr<Object> callable)
    : utils::EnableSharedFromThis<Object, BoundMethod>(KraitBuiltins::boundMethodType),
      instance_(instance), callable_(callable) {}

std::string BoundMethod::_type_() {
    return "method";
}

std::shared_ptr<Object> BoundMethod::callOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException(
            "method.__call__ requires at least 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<BoundMethod>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to method.__call__ must be a method");
    
    // Prepend the bound instance, then forward to underlying callable
    CallArgs newArgs(args.begin() + 1, args.end());
    newArgs.insert(newArgs.begin(), self->instance_);
    return self->callable_->call(newArgs);
}
std::shared_ptr<Object> BoundMethod::call(const CallArgs&  args) {
    CallArgs fullArgs{ _shared_from_this() };
    fullArgs.insert(fullArgs.end(), args.begin(), args.end());
    return BoundMethod::callOp(fullArgs);
}

std::shared_ptr<Object> BoundMethod::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "method.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<BoundMethod>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to method.__str__ must be a BoundMethod");
    
    std::ostringstream oss;
    oss << "<" << self->type_->name() << " for type '" << self->instance_->_type_();
    oss <<  "' at "  << self.get() << ">";
    return std::make_shared<String>(oss.str());
}
std::shared_ptr<String> BoundMethod::toString() {
    return std::dynamic_pointer_cast<String>(BoundMethod::toStringOp({ _shared_from_this() }));
}
