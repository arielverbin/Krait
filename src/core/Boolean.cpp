#include "Boolean.hpp"
#include <exception>

using namespace core;

Boolean::Boolean(bool value) : value_(value) {}

Boolean::operator bool() const {
    return value_;
}

std::string Boolean::_type_() {
    return "Boolean";
}

std::string Boolean::_str_() {
    return value_ ? "True" : "False";
}
