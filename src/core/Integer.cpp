#include "Integer.hpp"
#include <sstream>
#include <exception>

using namespace core;

Integer::Integer(long value) : value_(value) {}

std::string Integer::_type_() {
    return "Integer";
}

std::shared_ptr<Object> Integer::_add_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(Integer(value_ + i->value_));
    }

    throw std::runtime_error("[EXCEPTION] Type '" + this->_type_() + "' "
                    "does not support addition with type '" + another._type_() +"'.");
}


std::shared_ptr<Object> Integer::_sub_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(Integer(value_ - i->value_));
    }

    throw std::runtime_error("[EXCEPTION] Type '" + this->_type_() + "' "
                    "does not support subtraction with type '" + another._type_() +"'.");}

std::string Integer::_str_() {
    std::ostringstream oss;
    oss << value_;
    return oss.str();
}
