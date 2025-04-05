#include "GlobalState.hpp"
using namespace runtime;

Scope& GlobalState::currentScope() {
    if (callStack_.empty()) {
        return globalScope_;
    }

    return callStack_.top().scope;
}

Scope& GlobalState::globalScope() {
    return globalScope_;
}