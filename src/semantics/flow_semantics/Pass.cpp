#include "Pass.hpp"
#include "utils/utils.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

std::shared_ptr<core::Object> Pass::evaluate(runtime::Environment& state) const {
    UNREFERENCED(state);

    return core::None::getNone();
}
