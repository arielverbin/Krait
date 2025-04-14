#include "Pass.hpp"
#include "utils/utils.hpp"
#include "core/None.hpp"
using namespace semantics;

std::shared_ptr<core::Object> Pass::evaluate(runtime::Environment& state) {
    UNREFERENCED(state);

    return core::None::getNone();
}
