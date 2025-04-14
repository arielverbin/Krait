#ifndef CORE_BUILTINS_METHODS_HPP
#define CORE_BUILTINS_METHODS_HPP

#include "../Function.hpp"
#include <iostream>
#include "../Boolean.hpp"

// builtin supports,
std::shared_ptr<core::Object> __str_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_str_();
}

std::shared_ptr<core::Object> __add_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_add_(*other);
}

std::shared_ptr<core::Object> __sub_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_sub_(*other);
}

std::shared_ptr<core::Object> __mult_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_mult_(*other);
}

std::shared_ptr<core::Object> __div_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_div_(*other);
}

std::shared_ptr<core::Object> __neg_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    return self->_neg_();
}

std::shared_ptr<core::Object> __ge_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_ge_(*other);
}

std::shared_ptr<core::Object> __le_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_le_(*other);
}

std::shared_ptr<core::Object> __eq_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_eq_(*other);
}

std::shared_ptr<core::Object> __bool_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    return self->_bool_();
}

std::shared_ptr<core::Object> __and_(core::ArgList args) {
    auto first = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto second = std::dynamic_pointer_cast<core::Object>(args[1]);

    return *first->_bool_() ? second : first;
}

std::shared_ptr<core::Object> __or_(core::ArgList args) {
    auto first = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto second = std::dynamic_pointer_cast<core::Object>(args[1]);

    return *first->_bool_() ? first : second;
}

std::shared_ptr<core::Object> __not_(core::ArgList args) {
    auto first = std::dynamic_pointer_cast<core::Object>(args[0]);

    return core::Boolean::get(!(*first->_bool_()));
}

std::map<std::string, core::Function::NativeFunc> builtinMethods = {
    {"_str_", __str_ }, {"_add_", __add_ }, {"_sub_", __sub_ },
    {"_mult_", __mult_ }, {"_div_", __div_ }, {"_neg_", __neg_ },
    {"_ge_", __ge_ }, {"_le_", __le_ }, {"_eq_", __eq_ },
    {"_bool_", __bool_}, {"_and_", __and_ }, {"_or_", __or_ },
    {"_not_", __not_ },
};

#endif // CORE_BUILTINS_METHODS_HPP