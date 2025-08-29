#include "Break.hpp"
#include "Signal.hpp"
#include "utils/utils.hpp"
using namespace semantics;

core::Object* Break::evaluate(runtime::Environment& state) const {
    UNREFERENCED(state);
    
    throw BreakSignal();
}
