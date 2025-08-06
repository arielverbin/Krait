#include <memory>
#include "String.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

String::String(std::string value)
    : utils::EnableSharedFromThis<Object, String>(KraitBuiltins::stringType), value_(value) {}

String::operator std::string() const {
    return value_;
}

// Supported operations
std::shared_ptr<Object> String::addOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "string.__add__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    }
    auto first = std::dynamic_pointer_cast<String>(args[0]);
    auto second = std::dynamic_pointer_cast<String>(args[1]);
    if (first && second) {
        return std::make_shared<String>(first->value_ + second->value_);
    }

    throw except::NotImplementedException(
        "string.__add__ expects 2 arguments of type '" + KraitBuiltins::stringType->name() + "'");
}
std::shared_ptr<Object> String::add(std::shared_ptr<Object> another) {
    return String::addOp({ _shared_from_this(), another });
}
std::shared_ptr<Object> String::reversedAdd(std::shared_ptr<Object> another) {
    return String::addOp({ another, _shared_from_this() });
}

std::shared_ptr<Object> String::multiplyOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "string.__mult__ requires exactly 2 argument (received " + std::to_string(args.size()) + ")");
    }
    auto self = std::dynamic_pointer_cast<String>(args[0]);
    auto times = std::dynamic_pointer_cast<Integer>(args[1]);
    if (self && times) {
        long count = static_cast<long>(*times);
        std::string result = "";
        result.reserve(self->value_.size() * (count < 0 ? 0 : count));
        for (long i = 0; i < count; ++i) {
            result += self->value_;
        }
        return std::make_shared<String>(std::move(result));
    }

    throw except::NotImplementedException(
        "string.__mult__ expects arguments of type '"
            + KraitBuiltins::stringType->name() + "', '" + KraitBuiltins::intType->name() +"'");
}
std::shared_ptr<Object> String::multiply(std::shared_ptr<Object> another) {
    return String::multiplyOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> String::reversedMultiply(std::shared_ptr<Object> another) {
    return String::multiplyOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> String::equalOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "string.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }
    auto first = std::dynamic_pointer_cast<String>(args[0]);
    auto second = std::dynamic_pointer_cast<String>(args[1]);

    if (!first || !second) {
        throw except::NotImplementedException("string.__eq__ expects both arguments to be strings");
    }

    return Boolean::get(first && second && (first->value_ == second->value_));
}
std::shared_ptr<Object> String::equal(std::shared_ptr<Object> another) {
    return String::equalOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> String::notEqualOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "string.__neq__ requires exactly 2 argument (received " + std::to_string(args.size()) + ")");
    }
    auto first = std::dynamic_pointer_cast<String>(args[0]);
    auto second = std::dynamic_pointer_cast<String>(args[1]);

    if (!first || !second) {
        throw except::NotImplementedException("string.__eq__ expects both arguments to be strings");
    }

    return Boolean::get(!first || !second || (first->value_ != second->value_));
}
std::shared_ptr<Object> String::notEqual(std::shared_ptr<Object> another) {
    return String::notEqualOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> String::toBoolOp(const CallArgs& args) {
    if (args.size() != 1) {
        throw except::InvalidArgumentException(
            "string.__bool__ requires 1 argument (received " + std::to_string(args.size()) + ")");
    }
    auto self = std::dynamic_pointer_cast<String>(args[0]);
    if (self) {
        return Boolean::get(!self->value_.empty());
    }

    throw except::TypeError("first argument to string.__bool__ must be a string");
}
std::shared_ptr<Boolean> String::toBool() {
    return std::dynamic_pointer_cast<Boolean>(String::toBoolOp({ _shared_from_this() }));
}

std::shared_ptr<Object> String::toStringOp(const CallArgs& args) {
    if (args.size() != 1) {
        throw except::InvalidArgumentException(
            "string.__str__ requires no arguments (received " + std::to_string(args.size() - 1) + ")");
    }
    auto self = std::dynamic_pointer_cast<String>(args[0]);
    if (self) return self;

    throw except::TypeError("first argument to string.__str__ must be a string");
}
std::shared_ptr<String> String::toString() {
    return std::dynamic_pointer_cast<String>(String::toStringOp({ _shared_from_this() }));
}

std::shared_ptr<Object> String::createNewOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
                "str.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }
    
    auto classType = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!classType) {
        throw except::TypeError("str.__new__ expects first argument to be a type (got: '"
            + classType->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::stringType) {
        throw except::TypeError("str.__new__ expects first argument to be subclass of '"
            + KraitBuiltins::stringType->name() +"' (got: '" + classType->name() + "')");  
    }

    auto value = std::dynamic_pointer_cast<String>(args[1]);
    return value ? value : args[1]->toString();
}
