#include <sstream>
#include "Object.hpp"
#include "Boolean.hpp"
#include "String.hpp"
#include "BoundMethod.hpp"
#include "Function.hpp"
#include "utils/utils.hpp"
#include "core/builtins/builtin_methods.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

Object::Object() {
    self_ = std::shared_ptr<Object>(this, [](Object*) { /* no delete */ });
    auto self = self_;

    // Define all builtin methods.
    for (auto& builtin : builtinMethods) {
        members_[builtin.first] = LazyValue([self, builtin]() -> std::shared_ptr<BoundMethod> {
            std::shared_ptr<Function> func = std::make_shared<Function>(builtin.second);
            return std::make_shared<BoundMethod>(self, func);
        });
    }
}

Object::operator bool() const {
    return true;
}

void* Object::_loc_() {
    return this;
}

std::shared_ptr<Object> Object::_str_() {
    std::ostringstream oss;
    oss << "<Object at " << this << ">";
    return std::make_shared<String>(oss.str());
}

std::string Object::_type_() {
    return "Object";
}

std::shared_ptr<Object> Object::_att_(std::string varName) {
    auto member = members_.find(varName);

    if (member == members_.end()) {
        throw except::VariableNotFoundException(varName);
    }

    if (std::holds_alternative<std::shared_ptr<Object>>(member->second)) {
        return std::get<std::shared_ptr<Object>>(member->second);
    }

    // It's a LazyValue — resolve and cache
    LazyValue& lazy = std::get<LazyValue>(member->second);
    std::shared_ptr<Object> resolved = lazy.creator();
    member->second = resolved;
    return resolved;
}

std::shared_ptr<Object> Object::_add_(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type " + this->_type_() +
        " does not support addition.");
}

std::shared_ptr<Object> Object::_sub_(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type " + this->_type_() +
        " does not support subtraction.");
}

std::shared_ptr<Object> Object::_mult_(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type " + this->_type_() +
    " does not support multiplication.");
}

std::shared_ptr<Object> Object::_div_(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type " + this->_type_() +
        " does not support division.");
}

std::shared_ptr<Object> Object::_mod_(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type " + this->_type_() +
        " does not support modulo.");
}

std::shared_ptr<Object> Object::_neg_() {
    throw except::NotImplementedException("Object of type " + this->_type_() +
        " does not support negation.");
}

std::shared_ptr<Object> Object::_bool_() {
    return Boolean::get(false);
}

std::shared_ptr<Object> Object::_ge_(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type " + this->_type_() +
        " does not support '>=' comparison.");
}

std::shared_ptr<Object> Object::_le_(Object& another) {
    UNREFERENCED(another);

    throw except::NotImplementedException("Object of type " + this->_type_() +
        " does not support '<=' comparison.");
}

std::shared_ptr<Object> Object::_call_(std::vector<std::shared_ptr<Object>> args) {
    UNREFERENCED(args);

    throw except::NotImplementedException("Object of type " + this->_type_() +
        " is not callable.");
}

std::shared_ptr<Object> Object::_eq_(Object& another) {
    UNREFERENCED(another);

    // By default, fallback to identity comparison
    return Boolean::get(this == &another);
}
