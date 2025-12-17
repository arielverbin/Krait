#include "Const.hpp"
#include "utils/utils.hpp"
using namespace semantics;

Const::Const(core::Object* obj) : obj_(obj) {}

core::Object* Const::evaluate(runtime::Frame& state) const {
    // runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();
    UNREFERENCED(state);

    return obj_;
}
