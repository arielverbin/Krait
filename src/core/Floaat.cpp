#include "Float.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include "exceptions/exceptions.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace core;

Float::Float(double value) : value_(value) {}

std::string Float::_type_() {
    return "Float";
}

std::shared_ptr<Object> Float::_add_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return std::make_shared<Float>(value_ + i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support addition with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_sub_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return std::make_shared<Float>(value_ - i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support subtraction with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_mult_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return std::make_shared<Float>(value_ * i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not multiplication addition with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_div_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        if (i->value_ != 0) {
            return std::make_shared<Float>(value_ / i->value_);
        }

        throw except::DivisionByZeroException();
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support division with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_mod_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        if (i->value_ != 0) {
            return std::make_shared<Float>(std::fmod(value_, i->value_));
        }

        throw except::DivisionByZeroException();
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support modulo with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_neg_() {
    return std::make_shared<Float>(-value_);
}

std::shared_ptr<Object> Float::_bool_() {
    return Boolean::get(value_ != 0);
}

Float::operator bool() const {
    return value_ != 0;
}

std::shared_ptr<Object> Float::_ge_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return Boolean::get(value_ >= i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '>=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_gt_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return Boolean::get(value_ > i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '>=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_le_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return Boolean::get(value_ <= i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '<=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_lt_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return Boolean::get(value_ < i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                "does not support '<=' comparison with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> Float::_eq_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return Boolean::get(value_ == i->value_);
    }
    return Boolean::get(false);
}

std::shared_ptr<Object> Float::_neq_(Object& another) {
    if (Float* i = dynamic_cast<Float*>(&another)) {
        return Boolean::get(value_ != i->value_);
    }
    return Boolean::get(true);
}

std::shared_ptr<Object> Float::_str_() {
    std::ostringstream oss;
    // Sets the precision to 17 digits after decimal point, for printing
    oss << std::setprecision(17);
    oss << value_;
    return std::make_shared<String>(oss.str());
}

double Float::toRawDouble() {
    return value_;
}
