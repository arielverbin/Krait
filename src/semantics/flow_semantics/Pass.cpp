#include "Pass.hpp"
#include "utils/utils.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

core::Object* Pass::evaluate(runtime::Frame& state) const {
    // runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();
    UNREFERENCED(state);

    return core::None::getNone();
}
