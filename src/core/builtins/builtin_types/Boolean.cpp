#include "Boolean.hpp"
#include "String.hpp"
#include "core/TypeObject.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

Boolean::Boolean(bool value)
    : utils::EnableSharedFromThis<Object, Boolean>(KraitBuiltins::boolType), value_(value) {}

Boolean::operator bool() const {
    return value_;
}

std::shared_ptr<Boolean> Boolean::get(bool value) {
    static std::shared_ptr<Boolean> true_ = std::make_shared<Boolean>(true);
    static std::shared_ptr<Boolean> false_ = std::make_shared<Boolean>(false);
    return value ? true_ : false_;
}

std::shared_ptr<Object> Boolean::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "boolean.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<Boolean>(args[0]);
    if (!self)
        throw except::TypeError("first argument to boolean.__str__ must be a boolean");
    
    std::shared_ptr<String> trueStr = std::make_shared<String>("True");
    std::shared_ptr<String> falseStr = std::make_shared<String>("False");
    return *self ? trueStr : falseStr;
}
std::shared_ptr<String> Boolean::toString() {
    return std::dynamic_pointer_cast<String>(Boolean::toStringOp({ _shared_from_this() }));
}

std::shared_ptr<Object> Boolean::toBoolOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "boolean.__bool__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<Boolean>(args[0]);
    if (self) return self;

    throw except::TypeError("first argument to boolean.__bool__ must be a boolean");
}
std::shared_ptr<Boolean> Boolean::toBool() {
    return std::dynamic_pointer_cast<Boolean>(Boolean::toBoolOp({ _shared_from_this() }));
}

std::shared_ptr<Object> Boolean::equalOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "boolean.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Boolean>(args[0]);
    auto b = std::dynamic_pointer_cast<Boolean>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "boolean.__eq__ expects both arguments to be booleans");
    return get(a->value_ == b->value_);
}
std::shared_ptr<Object> Boolean::equal(std::shared_ptr<Object> another) {
    return Boolean::equalOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Boolean::notEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "boolean.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Boolean>(args[0]);
    auto b = std::dynamic_pointer_cast<Boolean>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "boolean.__eq__ expects both arguments to be booleans");
    return get(a->value_ != b->value_);
}
std::shared_ptr<Object> Boolean::notEqual(std::shared_ptr<Object> another) {
    return Boolean::notEqualOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Boolean::createNewOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
                "bool.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }
    
    auto classType = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!classType) {
        throw except::TypeError("bool.__new__ expects first argument to be a type (got: '"
            + classType->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::boolType) {
        throw except::TypeError("bool.__new__ expects first argument to be a subclass of '"
            + KraitBuiltins::boolType->name() +"' (got: '" + classType->name() + "')");  
    }

    auto value = std::dynamic_pointer_cast<Boolean>(args[1]);
    if (!value) return args[1]->toBool();
    return value; 
}
