#include <sstream>
#include "Object.hpp"
#include "Boolean.hpp"
#include "String.hpp"
#include "BoundMethod.hpp"
#include "Function.hpp"
#include "utils/utils.hpp"
#include "core/builtins/builtin_methods.hpp"
#include "exceptions/exceptions.hpp"
#include <iostream>
using namespace core;

Object::Object() {
    // Define all builtin methods.
    for (auto& builtin : builtin::builtinMethods) {
        members_[builtin.name] = 
            utils::LazyValue([builtin]() -> std::shared_ptr<Function> {
                return std::make_shared<Function>(builtin.func, builtin.numArgs);
            });
    }
}

Object::operator bool() const {
    return true;
}

void* Object::_loc_() {
    return this;
}

std::shared_ptr<Object> Object::toString() {
    std::ostringstream oss;
    oss << "<Object at " << this << ">";
    return std::make_shared<String>(oss.str());
}

std::string Object::_type_() {
    return "Object";
}

std::shared_ptr<Object> Object::getAttribute(std::string varName) {
    auto member = members_.find(varName);

    if (member == members_.end()) {
        throw except::VariableNotFoundException("Attribute '" + 
                varName + "' does not exist for type '" + this->_type_() + "'");
    }

    if (std::holds_alternative<std::shared_ptr<Object>>(member->second)) {
        return std::get<std::shared_ptr<Object>>(member->second);
    }

    // It's a LazyValue — resolve and cache
    utils::LazyValue& lazy = std::get<utils::LazyValue>(member->second);
    std::shared_ptr<Object> resolved = lazy.creator();
    member->second = resolved;
    return std::make_shared<BoundMethod>(shared_from_this(), resolved);
}

void Object::setAttribute(std::string varName, MemberEntry value) {
    members_[varName] = value;
}

std::shared_ptr<Object> Object::add(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support addition.");
}

std::shared_ptr<Object> Object::subtract(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support subtraction.");
}

std::shared_ptr<Object> Object::multiply(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support multiplication.");
}

std::shared_ptr<Object> Object::divide(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support division.");
}

std::shared_ptr<Object> Object::modulu(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support modulo.");
}

std::shared_ptr<Object> Object::negate() {
    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support negation.");
}

std::shared_ptr<Object> Object::toBool() {
    return Boolean::get(false);
}

std::shared_ptr<Object> Object::greaterEqual(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support '>=' comparison.");
}

std::shared_ptr<Object> Object::greater(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support '>' comparison.");
}

std::shared_ptr<Object> Object::lesserEqual(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support '<=' comparison.");
}

std::shared_ptr<Object> Object::lesser(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' does not support '<' comparison.");
}

std::shared_ptr<Object> Object::call(std::vector<std::shared_ptr<Object>> args) {
    UNREFERENCED(args);

    throw except::NotImplementedException("Object of type '" + this->_type_() +
        "' is not callable.");
}

std::shared_ptr<Object> Object::equal(Object& another) {
    UNREFERENCED(another);

    // By default, fallback to identity comparison
    return Boolean::get(this == &another);
}

std::shared_ptr<Object> Object::notEqual(Object& another) {
    UNREFERENCED(another);

    // By default, fallback to identity comparison
    return Boolean::get(this != &another);
}
