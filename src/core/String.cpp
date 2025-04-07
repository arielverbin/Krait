#include "String.hpp"
using namespace core;

String::String(std::string value) : value_(value) {}

std::string String::_type_() {
    return "String";
}

std::shared_ptr<Object> String::_add_(Object& another) {
    if (String* i = dynamic_cast<String*>(&another)) {
        return std::make_shared<String>(String(value_ + i->value_));
    }

    throw std::runtime_error("[EXCEPTION] Type '" + this->_type_() + "' "
                    "does not support addition with type '" + another._type_() +"'.");
}

std::string String::_str_() {
    return value_;
}