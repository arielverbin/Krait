#include "Pass.hpp"
#include "utils/utils.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

core::Object* Pass::evaluate(runtime::Frame& state) const {
    UNREFERENCED(state);

    return core::None::getNone();
}
