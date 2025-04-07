#include "String.hpp"
#include "../utils/utils.hpp"

using namespace core;

String::String(std::string value) : value_(value) {}

std::shared_ptr<Object> String::_add_(Object& another) {
    if (String* i = dynamic_cast<String*>(&another)) {
        return std::make_shared<String>(String(value_ + i->value_));
    }

    throw std::runtime_error("[EXCEPTION] Type 'String' does not support addition.");
}

std::shared_ptr<Object> String::_sub_(Object& another) {
    UNREFERENCED(another);
    
    throw std::runtime_error("[EXCEPTION] Type 'String' does not support subtraction.");
}

std::string String::_str_() {
    return value_;
}