#include "Boolean.hpp"
#include "String.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

std::shared_ptr<Boolean> Boolean::true_(new Boolean(true));
std::shared_ptr<Boolean> Boolean::false_(new Boolean(false));

Boolean::Boolean(bool value) : value_(value) {}

std::shared_ptr<Boolean> Boolean::get(bool value) {
    return value ? Boolean::true_ : Boolean::false_;
}

std::shared_ptr<Object> Boolean::_bool_() {
    return value_ ? Boolean::true_ : Boolean::false_;
}

Boolean::operator bool() const {
    return value_;
}

bool Boolean::_and_(Boolean& other) const {
    return other.value_ && value_;
}

bool Boolean::_or_(Boolean& other) const {
    return other.value_ || value_;
}

bool Boolean::_not_() const {
    return !value_;
}

std::shared_ptr<Object> Boolean::_eq_(Object& another) {
    if (Boolean* i = dynamic_cast<Boolean*>(&another)) {
        return Boolean::get(value_ == i->value_);
    }
    return Boolean::get(false);
}

std::string Boolean::_type_() {
    return "Boolean";
}

std::shared_ptr<Object> Boolean::_str_() {
    static std::shared_ptr<Object> trueString = std::make_shared<String>("True");
    static std::shared_ptr<Object> falseString = std::make_shared<String>("False");
    return value_ ? trueString : falseString;
}
