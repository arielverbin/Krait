#include "Function.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include "None.hpp"
#include "BoundMethod.hpp"
#include "exceptions/exceptions.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include <sstream>

using namespace core;

Function::Function(NativeFunc nativeFunc, size_t numArgs, bool checkArgs) 
        : utils::EnableSharedFromThis<Object, Function>(KraitBuiltins::functionType),
        isBuiltIn_(true), checkArgs_(checkArgs), nativeFunc_(nativeFunc), numArgs_(numArgs) {}

Function::Function(std::shared_ptr<semantics::ASTNode> body,
                   std::vector<std::string> params,
                   runtime::Environment closure)
    : utils::EnableSharedFromThis<Object, Function>(KraitBuiltins::functionType),
    isBuiltIn_(false), checkArgs_(true), body_(body), params_(params), closure_(std::move(closure)) {}

std::string Function::_type_() {
    return "function";
}

std::shared_ptr<Object> Function::callOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException(
            "function.__call__ requires at least 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<Function>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to function.__call__ must be a function");

    size_t expected = self->isBuiltIn_ ? self->numArgs_ : self->params_.size();
    size_t received = args.size() - 1;
    if ((self->checkArgs_) && received != expected)
        throw except::InvalidArgumentException(
            "function call with incorrect number of arguments (expected " + std::to_string(expected) +
            " args, got " + std::to_string(received) + ")");

    if (self->isBuiltIn_) {
        return self->nativeFunc_(CallArgs(args.begin() + 1, args.end()));
    }

    auto evalScope = self->closure_.createChildEnvironment();
    for (size_t i = 0; i < self->params_.size(); ++i) {
        evalScope.defineVariable(self->params_[i], args[i + 1]);
    }
    try { self->body_->evaluate(evalScope); }
    catch (const semantics::ReturnSignal& ret) { return ret.value(); }
    return None::getNone();
}
std::shared_ptr<Object> Function::call(const CallArgs& args) {
    CallArgs fullArgs{ _shared_from_this() };
    fullArgs.insert(fullArgs.end(), args.begin(), args.end());
    return Function::callOp(fullArgs);
}

std::shared_ptr<Object> Function::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "function.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<Function>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to function.__str__ must be a function");
    
    std::ostringstream oss;
    oss <<"<" + self->type_->name() +" at "  << self.get() << " (" << (self->isBuiltIn_ ? "" : "non ") << "built-in)>";
    return std::make_shared<String>(oss.str());
}
std::shared_ptr<String> Function::toString() {
    return std::dynamic_pointer_cast<String>(Function::toStringOp({ _shared_from_this() }));
}

std::shared_ptr<Object> Function::getOp(const CallArgs& args) {
    if (args.size() != 2) 
        throw except::AttributeError(
            "function.__get__ requires exactly 2 arguments (received " + std::to_string(args.size()) + "");

    std::shared_ptr<Function> func = std::dynamic_pointer_cast<Function>(args[0]);
    if (!func)
        throw except::InvalidArgumentException("first argument to function.__get__ must be a function");

    std::shared_ptr<Object> instance = std::dynamic_pointer_cast<core::Object>(args[1]);
    return std::make_shared<BoundMethod>(instance, func);
}

std::shared_ptr<Object> Function::get(std::shared_ptr<Object> instance) {
    return Function::getOp( { _shared_from_this(), instance });
}