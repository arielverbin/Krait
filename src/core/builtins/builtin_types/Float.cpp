#include "Float.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include <cmath>
#include "core/TypeObject.hpp"
#include "core/builtins/builtin_types/utils.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"

using namespace core;

Float::Float(double value)
    : utils::EnableSharedFromThis<Object, Float>(KraitBuiltins::floatType), value_(value) {}

Float::operator double() const {
    return value_;
}

std::shared_ptr<Object> Float::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "float.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<Float>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to float.__str__ must be a float");

    std::stringstream ss;
    ss.precision(15);
    ss << self->value_;
    return std::make_shared<String>(ss.str());
}
std::shared_ptr<String> Float::toString() {
    return std::dynamic_pointer_cast<String>(
        Float::toStringOp({ _shared_from_this() }));
}

std::shared_ptr<Object> Float::toBoolOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "float.__bool__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = std::dynamic_pointer_cast<Float>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to float.__bool__ must be a float");
    return Boolean::get(self->value_ != 0);
}
std::shared_ptr<Boolean> Float::toBool() {
    return std::dynamic_pointer_cast<Boolean>(Float::toBoolOp({ _shared_from_this() }));
}

std::shared_ptr<Object> Float::addOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__add__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);
    return std::make_shared<Float>(a + b);
}
std::shared_ptr<Object> Float::add(std::shared_ptr<Object> another) {
    return Float::addOp({ _shared_from_this(), another });
}
std::shared_ptr<Object> Float::reversedAdd(std::shared_ptr<Object> another) {
    return Float::addOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::subtractOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__sub__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);
    return std::make_shared<Float>(a - b);
}
std::shared_ptr<Object> Float::subtract(std::shared_ptr<Object> another) {
    return Float::subtractOp({ _shared_from_this(), another });
}
std::shared_ptr<Object> Float::reversedSubtract(std::shared_ptr<Object> another) {
    return Float::addOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::multiplyOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__mul__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);
    return std::make_shared<Float>(a * b);
}
std::shared_ptr<Object> Float::multiply(std::shared_ptr<Object> another) {
    return Float::multiplyOp({ _shared_from_this(), another });
}
std::shared_ptr<Object> Float::reversedMultiply(std::shared_ptr<Object> another) {
    return Float::addOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::divideOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__div__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);

    if (b == 0) throw except::DivisionByZeroException(*args[0]);

    return std::make_shared<Float>(a / b);
}
std::shared_ptr<Object> Float::divide(std::shared_ptr<Object> another) {
    return Float::divideOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::reversedDivideOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__div__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);
    if (a == 0) throw except::DivisionByZeroException(*args[1]);

    return std::make_shared<Float>(b / a);
}
std::shared_ptr<Object> Float::reversedDivide(std::shared_ptr<Object> another) {
    return Float::reversedDivideOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::moduluOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__mod__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);
    if (b == 0) throw except::DivisionByZeroException(*args[0]);

    return std::make_shared<Float>(std::fmod(a, b));
}
std::shared_ptr<Object> Float::modulu(std::shared_ptr<Object> another) {
    return Float::moduluOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::reversedModuluOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__mod__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);
    if (a == 0) throw except::DivisionByZeroException(*args[1]);

    return std::make_shared<Float>(std::fmod(b, a));
}
std::shared_ptr<Object> Float::reversedModulu(std::shared_ptr<Object> another) {
    return Float::moduluOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::negateOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "float.__neg__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Float>(args[0]);
    if (!a)
        throw except::InvalidArgumentException(
            "float.__neg__ expects an float argument");
    return std::make_shared<Float>(-a->value_);
}
std::shared_ptr<Object> Float::negate() {
    return Float::negateOp({ _shared_from_this() });
}

std::shared_ptr<Object> Float::greaterEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__ge__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);

    return Boolean::get(a >= b);
}
std::shared_ptr<Object> Float::greaterEqual(std::shared_ptr<Object> another) {
    return Float::greaterEqualOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::greaterOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__gt__ requires exactly 2 arguments (received " + std:: to_string(args.size()) + ")");

    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);

    return Boolean::get(a > b);
}
std::shared_ptr<Object> Float::greater(std::shared_ptr<Object> another) {
    return Float::greaterOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::lesserEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__le__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);

    return Boolean::get(a <= b);
}
std::shared_ptr<Object> Float::lesserEqual(std::shared_ptr<Object> another) {
    return Float::lesserEqualOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::lesserOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__lt__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);

    return Boolean::get(a < b);
}
std::shared_ptr<Object> Float::lesser(std::shared_ptr<Object> another) {
    return Float::lesserOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::equalOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);

    return Boolean::get(a == b);
}
std::shared_ptr<Object> Float::equal(std::shared_ptr<Object> another) {
    return Float::equalOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::notEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "float.__ne__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    double a = utils::getNumericValue<double>(args[0]);
    double b = utils::getNumericValue<double>(args[1]);

    return Boolean::get(a != b);
}
std::shared_ptr<Object> Float::notEqual(std::shared_ptr<Object> another) {
    return Float::notEqualOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Float::createNewOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
                "float.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }

    auto classType = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!classType) {
        throw except::TypeError("float.__new__ expects first argument to be a type (got: '"
            + classType->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::floatType) {
        throw except::TypeError("float.__new__ expects first argument to be a subclass of '"
            + KraitBuiltins::floatType->name() +"' (got: '" + classType->name() + "')");  
    }

    auto value = std::dynamic_pointer_cast<Float>(args[1]);
    if (!value) throw except::TypeError("float.__new__ expects argument to be a float");  

    return value; 
}
