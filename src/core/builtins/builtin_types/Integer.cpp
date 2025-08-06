#include "Integer.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"

using namespace core;

Integer::Integer(long value)
    : utils::EnableSharedFromThis<Object, Integer>(KraitBuiltins::intType), value_(value) {}

Integer::operator long() const {
    return value_;
}

std::shared_ptr<Object> Integer::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "integer.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = std::dynamic_pointer_cast<Integer>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to integer.__str__ must be an integer");
    return std::make_shared<String>(std::to_string(self->value_));
}
std::shared_ptr<String> Integer::toString() {
    return std::dynamic_pointer_cast<String>(
        Integer::toStringOp({ _shared_from_this() }));
}

std::shared_ptr<Object> Integer::toBoolOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "integer.__bool__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = std::dynamic_pointer_cast<Integer>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to integer.__bool__ must be an integer");
    return Boolean::get(self->value_ != 0);
}
std::shared_ptr<Boolean> Integer::toBool() {
    return std::dynamic_pointer_cast<Boolean>(Integer::toBoolOp({ _shared_from_this() }));
}

std::shared_ptr<Object> Integer::addOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__add__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
            
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__add__ expects both arguments to be integers");
    return std::make_shared<Integer>(a->value_ + b->value_);
}
std::shared_ptr<Object> Integer::add(std::shared_ptr<Object> another) {
    return Integer::addOp({ _shared_from_this(), another });
}
std::shared_ptr<Object> Integer::reversedAdd(std::shared_ptr<Object> another) {
    return Integer::addOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::subtractOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__sub__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__sub__ expects both arguments to be integers");
    return std::make_shared<Integer>(a->value_ - b->value_);
}
std::shared_ptr<Object> Integer::subtract(std::shared_ptr<Object> another) {
    return Integer::subtractOp({ _shared_from_this(), another });
}
std::shared_ptr<Object> Integer::reversedSubtract(std::shared_ptr<Object> another) {
    return Integer::subtractOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::multiplyOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__mul__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__mul__ expects both arguments to be integers");
    return std::make_shared<Integer>(a->value_ * b->value_);
}
std::shared_ptr<Object> Integer::multiply(std::shared_ptr<Object> another) {
    return Integer::multiplyOp({ _shared_from_this(), another });
}
std::shared_ptr<Object> Integer::reversedMultiply(std::shared_ptr<Object> another) {
    return Integer::multiplyOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::divideOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__div__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__div__ expects both arguments to be integers");

    if (b->value_ == 0) throw except::DivisionByZeroException(*a);

    return std::make_shared<Integer>(a->value_ / b->value_);
}
std::shared_ptr<Object> Integer::divide(std::shared_ptr<Object> another) {
    return Integer::divideOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::reversedDivideOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__div__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__div__ expects both arguments to be integers");

    if (a->value_ == 0) throw except::DivisionByZeroException(*b);

    return std::make_shared<Integer>(b->value_ / a->value_);
}
std::shared_ptr<Object> Integer::reversedDivide(std::shared_ptr<Object> another) {
    return Integer::reversedDivideOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::moduluOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__mod__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__mod__ expects both arguments to be integers");
            
    if (b->value_ == 0) throw except::DivisionByZeroException(*a);

    return std::make_shared<Integer>(a->value_ % b->value_);
}
std::shared_ptr<Object> Integer::modulu(std::shared_ptr<Object> another) {
    return Integer::moduluOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::reversedModuluOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__mod__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__mod__ expects both arguments to be integers");
  
    if (a->value_ == 0) throw except::DivisionByZeroException(*b);

    return std::make_shared<Integer>(b->value_ % a->value_);
}
std::shared_ptr<Object> Integer::reversedModulu(std::shared_ptr<Object> another) {
    return Integer::moduluOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::negateOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "integer.__neg__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    if (!a)
        throw except::InvalidArgumentException(
            "integer.__neg__ expects an integer argument");
    return std::make_shared<Integer>(-a->value_);
}
std::shared_ptr<Object> Integer::negate() {
    return Integer::negateOp({ _shared_from_this() });
}

std::shared_ptr<Object> Integer::greaterEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__ge__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__ge__ expects both arguments to be integers");
    return Boolean::get(a->value_ >= b->value_);
}
std::shared_ptr<Object> Integer::greaterEqual(std::shared_ptr<Object> another) {
    return Integer::greaterEqualOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::greaterOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__gt__ requires exactly 2 arguments (received " + std:: to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__gt__ expects both arguments to be integers");
    return Boolean::get(a->value_ > b->value_);
}
std::shared_ptr<Object> Integer::greater(std::shared_ptr<Object> another) {
    return Integer::greaterOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::lesserEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__le__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__le__ expects both arguments to be integers");
    return Boolean::get(a->value_ <= b->value_);
}
std::shared_ptr<Object> Integer::lesserEqual(std::shared_ptr<Object> another) {
    return Integer::lesserEqualOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::lesserOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__lt__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__lt__ expects both arguments to be integers");
    return Boolean::get(a->value_ < b->value_);
}
std::shared_ptr<Object> Integer::lesser(std::shared_ptr<Object> another) {
    return Integer::lesserOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::equalOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__eq__ expects both arguments to be integers");
    return Boolean::get(a->value_ == b->value_);
}
std::shared_ptr<Object> Integer::equal(std::shared_ptr<Object> another) {
    return Integer::equalOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::notEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "integer.__ne__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = std::dynamic_pointer_cast<Integer>(args[0]);
    auto b = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "integer.__ne__ expects both arguments to be integers");
    return Boolean::get(a->value_ != b->value_);
}
std::shared_ptr<Object> Integer::notEqual(std::shared_ptr<Object> another) {
    return Integer::notEqualOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> Integer::createNewOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
                "integer.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");

    auto classType = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!classType) {
        throw except::TypeError("integer.__new__ expects first argument to be a type (got: '"
            + classType->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::intType) {
        throw except::TypeError("integer.__new__ expects first argument to be a subclass of '"
            + KraitBuiltins::intType->name() +"' (got: '" + classType->name() + "')");  
    }

    auto value = std::dynamic_pointer_cast<Integer>(args[1]);
    if (!value) throw except::TypeError("integer.__new__ expects argument to be an integer");  

    return value;
}
