#include "Integer.hpp"
#include <sstream>
#include "String.hpp"
#include "Boolean.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

Integer::Integer(long value) : value_(value) {}

std::string Integer::_type_() {
    return "Integer";
}

std::shared_ptr<Object> Integer::add(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(value_ + i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support addition with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::subtract(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(value_ - i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support subtraction with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::multiply(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(value_ * i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not multiplication addition with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::divide(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        if (i->value_ != 0) {
            return std::make_shared<Integer>(value_ / i->value_);
        }

        throw except::DivisionByZeroException(*this);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support division with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::modulu(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        if (i->value_ != 0) {
            return std::make_shared<Integer>(value_ % i->value_);
        }

        throw except::DivisionByZeroException(*this);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support modulo with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::negate() {
    return std::make_shared<Integer>(-value_);
}

std::shared_ptr<Object> Integer::toBool() {
    return Boolean::get(value_ != 0);
}

Integer::operator bool() const {
    return value_ != 0;
}

std::shared_ptr<Object> Integer::greaterEqual(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ >= i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '>=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::greater(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ > i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '>=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::lesserEqual(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ <= i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '<=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::lesser(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ < i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '<=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::equal(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ == i->value_);
    }
    return Boolean::get(false);
}

std::shared_ptr<Object> Integer::notEqual(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ != i->value_);
    }
    return Boolean::get(true);
}

std::shared_ptr<Object> Integer::toString() {
    std::ostringstream oss;
    oss << value_;
    return std::make_shared<String>(oss.str());
}

long Integer::toRawLong() {
    return value_;
}
