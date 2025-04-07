#include "Object.hpp"
#include "../utils/utils.hpp"

#include <sstream>

using namespace core;

void* Object::_loc_() {
    return this;
}

std::string Object::_str_() {
    std::ostringstream oss;
    oss << "<Object at " << this << ">";
    return oss.str();
}

std::string Object::_type_() {
    return "Object";
}

std::shared_ptr<Object> Object::_add_(Object& another) {
    UNREFERENCED(another);

    throw std::runtime_error("[EXCEPTION] Type '" + self->_type_() + "' does not support addition.");
}


std::shared_ptr<Object> Object::_sub_(Object& another) {
    UNREFERENCED(another);

    throw std::runtime_error("[EXCEPTION] Type '" + self->_type_() + "' does not support subtraction.");
}

std::shared_ptr<Object> Object::_att_(std::string varName) {
    auto member = members_.find(varName);

    if (member != members_.end()) {
        return member->second;
    }

    throw std::exception();
}
