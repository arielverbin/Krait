#include "Const.hpp"
#include "utils/utils.hpp"
using namespace semantics;

Const::Const(std::shared_ptr<core::Object> obj) : obj_(obj) {}

std::shared_ptr<core::Object> Const::evaluate(runtime::Environment& state) {
    UNREFERENCED(state);

    return obj_;
}
