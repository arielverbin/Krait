#include <memory>
#include "String.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

String::String(std::string value) : Object(KraitBuiltins::stringType), value_(value) {}

String::operator std::string() const {
    return value_;
}

// Supported operations
Object* String::addOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "string.__add__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    }
    auto first = dynamic_cast<String*>(args[0]);
    auto second = dynamic_cast<String*>(args[1]);
    if (first && second) {
        return gc::make_guarded<String>(first->value_ + second->value_);
    }

    throw except::NotImplementedException(
        "string.__add__ expects 2 arguments of type '" + KraitBuiltins::stringType->name() + "'");
}
Object* String::add(Object* another) {
    return String::addOp({ this, another });
}
Object* String::reversedAdd(Object* another) {
    return String::addOp({ another, this });
}

Object* String::multiplyOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "string.__mult__ requires exactly 2 argument (received " + std::to_string(args.size()) + ")");
    }
    auto self = dynamic_cast<String*>(args[0]);
    auto times = dynamic_cast<Integer*>(args[1]);
    if (self && times) {
        long count = static_cast<long>(*times);
        std::string result = "";
        result.reserve(self->value_.size() * (count < 0 ? 0 : count));
        for (long i = 0; i < count; ++i) {
            result += self->value_;
        }
        return gc::make_guarded<String>(std::move(result));
    }

    throw except::NotImplementedException(
        "string.__mult__ expects arguments of type '"
            + KraitBuiltins::stringType->name() + "', '" + KraitBuiltins::intType->name() +"'");
}
Object* String::multiply(Object* another) {
    return String::multiplyOp({ this, another });
}

Object* String::reversedMultiply(Object* another) {
    return String::multiplyOp({ this, another });
}

Object* String::equalOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "string.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }
    auto first = dynamic_cast<String*>(args[0]);
    auto second = dynamic_cast<String*>(args[1]);

    if (!first || !second) {
        throw except::NotImplementedException("string.__eq__ expects both arguments to be strings");
    }

    return Boolean::get(first && second && (first->value_ == second->value_));
}
Object* String::equal(Object* another) {
    return String::equalOp({ this, another });
}

Object* String::notEqualOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "string.__neq__ requires exactly 2 argument (received " + std::to_string(args.size()) + ")");
    }
    auto first = dynamic_cast<String*>(args[0]);
    auto second = dynamic_cast<String*>(args[1]);

    if (!first || !second) {
        throw except::NotImplementedException("string.__eq__ expects both arguments to be strings");
    }

    return Boolean::get(!first || !second || (first->value_ != second->value_));
}
Object* String::notEqual(Object* another) {
    return String::notEqualOp({ this, another });
}

Object* String::toBoolOp(const CallArgs& args) {
    if (args.size() != 1) {
        throw except::InvalidArgumentException(
            "string.__bool__ requires 1 argument (received " + std::to_string(args.size()) + ")");
    }
    auto self = dynamic_cast<String*>(args[0]);
    if (self) {
        return Boolean::get(!self->value_.empty());
    }

    throw except::TypeError("first argument to string.__bool__ must be a string");
}
Boolean* String::toBool() {
    return static_cast<Boolean*>(String::toBoolOp({ this }));
}

Object* String::toStringOp(const CallArgs& args) {
    if (args.size() != 1) {
        throw except::InvalidArgumentException(
            "string.__str__ requires no arguments (received " + std::to_string(args.size() - 1) + ")");
    }
    auto self = dynamic_cast<String*>(args[0]);
    if (self) return self;

    throw except::TypeError("first argument to string.__str__ must be a string");
}
String* String::toString() {
    return static_cast<String*>(String::toStringOp({ this }));
}

Object* String::createNewOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
                "str.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }
    
    auto classType = dynamic_cast<TypeObject*>(args[0]);
    if (!classType) {
        throw except::TypeError("str.__new__ expects first argument to be a type (got: '"
            + classType->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::stringType) {
        throw except::TypeError("str.__new__ expects first argument to be subclass of '"
            + KraitBuiltins::stringType->name() +"' (got: '" + classType->name() + "')");  
    }

    auto value = dynamic_cast<String*>(args[1]);
    return value ? value : args[1]->toString();
}
