#ifndef CORE_UTILS_HPP
#define CORE_UTILS_HPP

#include "core/Object.hpp"
#include "exceptions/exceptions.hpp"
#include "core/builtins/builtin_types/Float.hpp"
#include "core/builtins/builtin_types/Integer.hpp"

namespace utils {

template<typename T>
T getNumericValue(core::Object* o) {
    core::Float* f = dynamic_cast<core::Float*>(o);
    if (f) return static_cast<T>(*f);

    core::Integer* i = dynamic_cast<core::Integer*>(o);
    if (i) return static_cast<T>(*i);

    throw except::NotImplementedException("expected a numeric type (got: '" + o->type()->name() + "')");
}

} // namespace utils

#endif // CORE_UTILS_HPP