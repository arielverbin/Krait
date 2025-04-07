#include "Boolean.hpp"
#include <exception>

using namespace core;

Boolean::Boolean(bool value) : value_(value) {}

std::string Boolean::_type_() {
    return "Boolean";
}

std::string Boolean::_str_() {
    return value_ ? "True" : "False";
}
