#include "Integer.hpp"
#include <sstream>
#include <exception>

using namespace core;

Integer::Integer(long value) : value_(value) {}

std::shared_ptr<Object> Integer::_add_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(Integer(value_ + i->value_));
    }

    throw std::runtime_error("[EXCEPTION] Attempt of addition with unsupported datatype.");
}


std::shared_ptr<Object> Integer::_sub_(Object& another) {
    if (Integer* i = dynamic_cast<Integer*>(&another)) {
        return std::make_shared<Integer>(Integer(value_ - i->value_));
    }

    throw std::runtime_error("[EXCEPTION] Attempt of subtraction with unsupported datatype.");
}

std::string Integer::_str_() {
    std::ostringstream oss;
    oss << value_;
    return oss.str();
}
