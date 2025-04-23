#ifndef CORE_BUILTINS_METHODS_HPP
#define CORE_BUILTINS_METHODS_HPP

#include "../Function.hpp"
#include <iostream>
#include "../Boolean.hpp"

namespace builtin {

struct BuiltinFunction {
    core::Function::NativeFunc func;
    std::string name;
    size_t numArgs;
};

// builtin supports,
std::shared_ptr<core::Object> __str_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
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

std::shared_ptr<core::Object> __mod_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_mod_(*other);
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

std::shared_ptr<core::Object> __gt_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_gt_(*other);
}

std::shared_ptr<core::Object> __le_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_le_(*other);
}

std::shared_ptr<core::Object> __lt_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_lt_(*other);
}

std::shared_ptr<core::Object> __eq_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_eq_(*other);
}

std::shared_ptr<core::Object> __neq_(core::ArgList args) {
    auto self = std::dynamic_pointer_cast<core::Object>(args[0]);
    auto other = std::dynamic_pointer_cast<core::Object>(args[1]);
    return self->_neq_(*other);
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

std::vector<BuiltinFunction> builtinMethods = {
    {__str_, "_str_", 1}, {__add_, "_add_", 2}, {__sub_, "_sub_", 2},
    {__mult_, "_mult_", 2}, {__div_, "_div_", 2}, {__mod_, "_mod_", 2}, {__neg_, "_neg_", 1},
    {__ge_, "_ge_", 2}, {__gt_, "_gt_", 2}, {__le_, "_le_", 2}, {__lt_, "_lt_", 2},
    {__eq_, "_eq_", 2}, {__neq_, "_neq_", 2}, {__bool_, "_bool_", 1}, {__and_, "_and_", 2},
    {__or_, "_or_", 2}, {__not_, "_not_", 1},
};

}


#endif // CORE_BUILTINS_METHODS_HPP