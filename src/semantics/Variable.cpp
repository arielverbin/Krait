#include "Variable.hpp"
using namespace semantics;

Variable::Variable(std::string targetVar)
    : targetVar_(targetVar) {}


std::shared_ptr<core::Object> Variable::evaluate(runtime::GlobalState& state) {
    auto currentScopeMap = state.currentScope().map;
    auto globalScopeMap = state.globalScope().map;

    // If the target var already exists in the current scope, return its value.
    if (currentScopeMap.find(targetVar_) != currentScopeMap.end()) {
        return currentScopeMap[targetVar_];
    }

    // If the target var already exists in the global scope, return its value.
    if (globalScopeMap.find(targetVar_) != globalScopeMap.end()) {
        return globalScopeMap[targetVar_];
    }

    // Variable was not found.
    throw std::runtime_error("[EXCEPTION] Could not find " + targetVar_ + ".");
}