#include "None.hpp"
#include "String.hpp"
#include "Boolean.hpp"
using namespace core;

std::shared_ptr<Object> None::_str_() {
    static std::shared_ptr<Object> none = std::make_shared<String>("None");
    return none;
}

std::string None::_type_() {
    return "None";
}

None::operator bool() const {
    return false;
}

std::shared_ptr<Object> None::_bool_() {
    return Boolean::get(false);
}

std::shared_ptr<Object> None::_eq_(Object& another) {
    return dynamic_cast<None*>(&another) == nullptr ? Boolean::get(false) : Boolean::get(false);
}

std::shared_ptr<None> None::getNone() {
    static std::shared_ptr<None> none_(new None());
    return none_;
}
