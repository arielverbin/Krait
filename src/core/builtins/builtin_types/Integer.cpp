#include "Integer.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"

using namespace core;

Integer::Integer(long value) : Object(KraitBuiltins::intType), value_(value) {}

Integer::operator long() const {
    return value_;
}

Object* Integer::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "integer.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = dynamic_cast<Integer*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to integer.__str__ must be an integer");
    return gc::make_guarded<String>(std::to_string(self->value_));
}
String* Integer::toString() {
    return static_cast<String*>(Integer::toStringOp({ this }));
}

Object* Integer::toBoolOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "integer.__bool__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = dynamic_cast<Integer*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to integer.__bool__ must be an integer");
    return Boolean::get(self->value_ != 0);
}
Boolean* Integer::toBool() {
    return static_cast<Boolean*>(Integer::toBoolOp({ this }));
}

Object* Integer::addOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__add__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
            
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__add__ expects both arguments to be integers");
    return gc::make_guarded<Integer>(a->value_ + b->value_);
}
Object* Integer::add(Object* another) {
    return Integer::addOp({ this, another });
}
Object* Integer::reversedAdd(Object* another) {
    return Integer::addOp({ another, this });
}

Object* Integer::subtractOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__sub__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__sub__ expects both arguments to be integers");
    return gc::make_guarded<Integer>(a->value_ - b->value_);
}
Object* Integer::subtract(Object* another) {
    return Integer::subtractOp({ this, another });
}
Object* Integer::reversedSubtract(Object* another) {
    return Integer::subtractOp({ another, this });
}

Object* Integer::multiplyOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__mul__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__mul__ expects both arguments to be integers");
    return gc::make_guarded<Integer>(a->value_ * b->value_);
}
Object* Integer::multiply(Object* another) {
    return Integer::multiplyOp({ this, another });
}
Object* Integer::reversedMultiply(Object* another) {
    return Integer::multiplyOp({ another, this });
}

Object* Integer::divideOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__div__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__div__ expects both arguments to be integers");

    if (b->value_ == 0) throw except::DivisionByZeroException(*a);

    return gc::make_guarded<Integer>(a->value_ / b->value_);
}
Object* Integer::divide(Object* another) {
    return Integer::divideOp({ this, another });
}
Object* Integer::reversedDivide(Object* another) {
    return Integer::divideOp({ another, this });
}

Object* Integer::moduluOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__mod__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__mod__ expects both arguments to be integers");
            
    if (b->value_ == 0) throw except::DivisionByZeroException(*a);

    return gc::make_guarded<Integer>(a->value_ % b->value_);
}
Object* Integer::modulu(Object* another) {
    return Integer::moduluOp({ this, another });
}

Object* Integer::reversedModulu(Object* another) {
    return Integer::moduluOp({ another, this });
}

Object* Integer::negateOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "integer.__neg__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    if (!a)
        throw except::InvalidArgumentException(
            "integer.__neg__ expects an integer argument");
    return gc::make_guarded<Integer>(-a->value_);
}
Object* Integer::negate() {
    return Integer::negateOp({ this });
}

Object* Integer::greaterEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__ge__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__ge__ expects both arguments to be integers");
    return Boolean::get(a->value_ >= b->value_);
}
Object* Integer::greaterEqual(Object* another) {
    return Integer::greaterEqualOp({ this, another });
}

Object* Integer::greaterOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__gt__ requires exactly 2 arguments (received " + std:: to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__gt__ expects both arguments to be integers");
    return Boolean::get(a->value_ > b->value_);
}
Object* Integer::greater(Object* another) {
    return Integer::greaterOp({ this, another });
}

Object* Integer::lesserEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__le__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__le__ expects both arguments to be integers");
    return Boolean::get(a->value_ <= b->value_);
}
Object* Integer::lesserEqual(Object* another) {
    return Integer::lesserEqualOp({ this, another });
}

Object* Integer::lesserOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__lt__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__lt__ expects both arguments to be integers");
    return Boolean::get(a->value_ < b->value_);
}
Object* Integer::lesser(Object* another) {
    return Integer::lesserOp({ this, another });
}

Object* Integer::equalOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__eq__ expects both arguments to be integers");
    return Boolean::get(a->value_ == b->value_);
}
Object* Integer::equal(Object* another) {
    return Integer::equalOp({ this, another });
}

Object* Integer::notEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__ne__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Integer*>(args[0]);
    auto b = dynamic_cast<Integer*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__ne__ expects both arguments to be integers");
    return Boolean::get(a->value_ != b->value_);
}
Object* Integer::notEqual(Object* another) {
    return Integer::notEqualOp({ this, another });
}

Object* Integer::createNewOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
                "integer.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    auto classType = dynamic_cast<TypeObject*>(args[0]);
    if (!classType) {
        throw except::TypeError("integer.__new__ expects first argument to be a type (got: '"
            + classType->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::intType) {
        throw except::TypeError("integer.__new__ expects first argument to be a subclass of '"
            + KraitBuiltins::intType->name() +"' (got: '" + classType->name() + "')");  
    }

    auto value = dynamic_cast<Integer*>(args[1]);
    if (!value) throw except::TypeError("integer.__new__ expects argument to be an integer");  

    return value;
}
