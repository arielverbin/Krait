#include "Function.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include "None.hpp"
#include "Method.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "exceptions/exceptions.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include <sstream>

using namespace core;

Function::Function(NativeFunc nativeFunc) 
    : Object(KraitBuiltins::functionType), isBuiltIn_(true), nativeFunc_(nativeFunc) {}

Function::Function(std::shared_ptr<semantics::ASTNode> body,
                   std::vector<std::string> params,
                   runtime::Frame* closure)
    : Object(KraitBuiltins::functionType),
    isBuiltIn_(false), body_(body), params_(params), closure_(closure) {
        setAttribute("__closure__", closure_);
    }

Object* Function::callOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException(
            "function.__call__ requires at least 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = dynamic_cast<Function*>(args[0]);
    if (!self)
        throw except::TypeError("first argument to function.__call__ must be a function");

    if (self->isBuiltIn_) {
        // no need to check number of arguments for builtin functions, they need to do it themselves.
        return self->nativeFunc_(CallArgs(args.begin() + 1, args.end()));
    }

    size_t expected = self->params_.size();
    size_t received = args.size() - 1;
    if (received != expected) {
        throw except::InvalidArgumentException(
            "function call with incorrect number of arguments (expected " + std::to_string(expected) +
            " args, got " + std::to_string(received) + ")");
    }

    self->closure_->pushNewScope();
    for (size_t i = 0; i < self->params_.size(); ++i) {
        self->closure_->defineVariable(self->params_[i], args[i + 1]);
    }

    Object* returnValue = None::getNone();

    try { self->body_->evaluate(*self->closure_); }  // execute the function
    catch (const semantics::ReturnSignal& ret) {  returnValue = ret.value(); }

    self->closure_->popLastScope();
    return returnValue;
}

Object* Function::call(const CallArgs& args) {
    CallArgs fullArgs{ this };
    fullArgs.insert(fullArgs.end(), args.begin(), args.end());
    return Function::callOp(fullArgs);
}

Object* Function::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "function.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = dynamic_cast<Function*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to function.__str__ must be a function");
    
    std::ostringstream oss;
    oss <<"<" + self->type()->name() + " at "  << self << " (" << (self->isBuiltIn_ ? "" : "non ") << "built-in)>";
    return gc::make_tracked<String>(oss.str());
}
String* Function::toString() {
    return dynamic_cast<String*>(Function::toStringOp({ this }));
}

Object* Function::getOp(const CallArgs& args) {
    if (args.size() != 3) 
        throw except::AttributeError(
            "function.__get__ requires exactly 3 arguments (received " + std::to_string(args.size()) + ")");

    Function* self = dynamic_cast<Function*>(args[0]);
    if (!self) {
        throw except::TypeError(
            "first argument to function.__get__ must be a function (got: " + args[0]->type()->name() + ")"
        );
    }

    Object* instance = dynamic_cast<core::Object*>(args[1]);

    // don't bind to None type
    if (instance->type() == KraitBuiltins::noneType) return self;

    return gc::make_tracked<Method>(instance, self);
}

Object* Function::get(Object* instance, TypeObject* owner) {
    return Function::getOp( { this, instance, owner });
}

Object* Function::createNewOp(const CallArgs& args) {
    UNREFERENCED(args);
    throw except::NotImplementedException("explicit function constructor is not supported yet");
}