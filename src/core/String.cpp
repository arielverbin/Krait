#include <memory>
#include "String.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

String::String(std::string value) : value_(value) {}

std::string String::_type_() {
    return "String";
}

std::shared_ptr<Object> String::_add_(Object& another) {
    // String addition attempts to convert the given object to a String
    // and concatenate the two strings.
    if (std::shared_ptr<String> i = std::dynamic_pointer_cast<String>(another._str_())) {
        return std::make_shared<String>(value_ + i->value_);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                    "does not support addition with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> String::_mult_(Object& another) {
    // String multiplication attempts to convert the given object to an Integer
    // and repeat the string that many times.
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        std::string result = "";
        for (long j = 0; j < i->toRawLong(); ++j) {
            result += value_;
        }
        return std::make_shared<String>(result);
    }

    throw except::InvalidArgumentException("Type '" + this->_type_() + "' "
                    "does not support multiplication with type '" + another._type_() +"'.");
}

std::shared_ptr<Object> String::_eq_(Object& another) {
    if (String* i = dynamic_cast<String*>(&another)) {
        return Boolean::get(value_ == i->value_);
    }
    return Boolean::get(false);
}

std::shared_ptr<Object> String::_bool_() {
    return Boolean::get(value_.size() != 0);
}

String::operator bool() const {
    return value_.size() != 0;
}

std::shared_ptr<Object> String::_str_() {
    return std::make_shared<String>(value_);
}
