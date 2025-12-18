#include "Float.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include <cmath>
#include "core/TypeObject.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"

using namespace core;

Float::Float(double value) : Object(KraitBuiltins::floatType), value_(value) {}

Float::operator double() const {
    return value_;
}

Object* Float::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "float.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = dynamic_cast<Float*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to float.__str__ must be a float");

    std::stringstream ss;
    ss.precision(15);
    ss << self->value_;
    return gc::make_guarded<String>(ss.str());
}
String* Float::toString() {
    return static_cast<String*>(Float::toStringOp({ this }));
}

Object* Float::toBoolOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "float.__bool__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = dynamic_cast<Float*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to float.__bool__ must be a float");
    return Boolean::get(self->value_ != 0);
}
Boolean* Float::toBool() {
    return static_cast<Boolean*>(Float::toBoolOp({ this }));
}

Object* Float::addOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__add__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);
    return gc::make_guarded<Float>(a + b);
}
Object* Float::add(Object* another) {
    return Float::addOp({ this, another });
}
Object* Float::reversedAdd(Object* another) {
    return Float::addOp({ another, this });
}

Object* Float::subtractOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__sub__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);
    return gc::make_guarded<Float>(a - b);
}
Object* Float::subtract(Object* another) {
    return Float::subtractOp({ this, another });
}
Object* Float::reversedSubtract(Object* another) {
    return Float::subtractOp({ another, this });
}

Object* Float::multiplyOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__mul__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);
    return gc::make_guarded<Float>(a * b);
}
Object* Float::multiply(Object* another) {
    return Float::multiplyOp({ this, another });
}
Object* Float::reversedMultiply(Object* another) {
    return Float::multiplyOp({ another, this });
}

Object* Float::divideOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__div__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);

    if (b == 0) throw except::DivisionByZeroException(*args[0]);

    return gc::make_guarded<Float>(a / b);
}
Object* Float::divide(Object* another) {
    return Float::divideOp({ this, another });
}
Object* Float::reversedDivide(Object* another) {
    return Float::divideOp({ another, this });
}

Object* Float::moduluOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__mod__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);
    if (b == 0) throw except::DivisionByZeroException(*args[0]);

    return gc::make_guarded<Float>(std::fmod(a, b));
}
Object* Float::modulu(Object* another) {
    return Float::moduluOp({ this, another });
}
Object* Float::reversedModulu(Object* another) {
    return Float::moduluOp({ another, this });
}

Object* Float::negateOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "float.__neg__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Float*>(args[0]);
    if (!a)
        throw except::InvalidArgumentException(
            "float.__neg__ expects an float argument");
    return gc::make_guarded<Float>(-a->value_);
}
Object* Float::negate() {
    return Float::negateOp({ this });
}

Object* Float::greaterEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__ge__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);

    return Boolean::get(a >= b);
}
Object* Float::greaterEqual(Object* another) {
    return Float::greaterEqualOp({ this, another });
}

Object* Float::greaterOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__gt__ requires exactly 2 arguments (received " + std:: to_string(args.size()) + ")");

    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);

    return Boolean::get(a > b);
}
Object* Float::greater(Object* another) {
    return Float::greaterOp({ this, another });
}

Object* Float::lesserEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__le__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);

    return Boolean::get(a <= b);
}
Object* Float::lesserEqual(Object* another) {
    return Float::lesserEqualOp({ this, another });
}

Object* Float::lesserOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__lt__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);

    return Boolean::get(a < b);
}
Object* Float::lesser(Object* another) {
    return Float::lesserOp({ this, another });
}

Object* Float::equalOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);

    return Boolean::get(a == b);
}
Object* Float::equal(Object* another) {
    return Float::equalOp({ this, another });
}

Object* Float::notEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__ne__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = Float::getNumericValue<double>(args[0]);
    double b = Float::getNumericValue<double>(args[1]);

    return Boolean::get(a != b);
}
Object* Float::notEqual(Object* another) {
    return Float::notEqualOp({ this, another });
}

Object* Float::createNewOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
                "float.__new__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }

    auto classType = dynamic_cast<TypeObject*>(args[0]);
    if (!classType) {
        throw except::TypeError("float.__new__ expects first argument to be a type (got: '"
            + classType->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::floatType) {
        throw except::TypeError("float.__new__ expects first argument to be a subclass of '"
            + KraitBuiltins::floatType->name() +"' (got: '" + classType->name() + "')");  
    }

    auto value = dynamic_cast<Float*>(args[1]);
    if (!value) throw except::TypeError("float.__new__ expects argument to be a float");  

    return value; 
}
