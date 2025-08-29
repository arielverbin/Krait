#include "Const.hpp"
#include "utils/utils.hpp"
using namespace semantics;

Const::Const(core::Object* obj) : obj_(obj) {}

core::Object* Const::evaluate(runtime::Environment& state) const {
    UNREFERENCED(state);

    return obj_;
}
