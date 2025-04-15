#include "Continue.hpp"
#include "Signal.hpp"
#include "utils/utils.hpp"
using namespace semantics;

std::shared_ptr<core::Object> Continue::evaluate(runtime::Environment& state) const {
    UNREFERENCED(state);
    
    throw ContinueSignal();
}
