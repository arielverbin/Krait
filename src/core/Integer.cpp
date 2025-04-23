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

std::shared_ptr<Object> Integer::_add_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(value_ + i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support addition with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_sub_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(value_ - i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support subtraction with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_mult_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(value_ * i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not multiplication addition with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_div_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        if (i->value_ != 0) {
            return std::make_shared<Integer>(value_ / i->value_);
        }

        throw except::DivisionByZeroException(*this);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support division with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_mod_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        if (i->value_ != 0) {
            return std::make_shared<Integer>(value_ % i->value_);
        }

        throw except::DivisionByZeroException(*this);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support modulo with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_neg_() {
    return std::make_shared<Integer>(-value_);
}

std::shared_ptr<Object> Integer::_bool_() {
    return Boolean::get(value_ != 0);
}

Integer::operator bool() const {
    return value_ != 0;
}

std::shared_ptr<Object> Integer::_ge_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ >= i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '>=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_gt_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ > i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '>=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_le_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ <= i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '<=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_lt_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ < i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '<=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Integer::_eq_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ == i->value_);
    }
    return Boolean::get(false);
}

std::shared_ptr<Object> Integer::_neq_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return Boolean::get(value_ != i->value_);
    }
    return Boolean::get(true);
}

std::shared_ptr<Object> Integer::_str_() {
    std::ostringstream oss;
    oss << value_;
    return std::make_shared<String>(oss.str());
}

long Integer::toRawLong() {
    return value_;
}
