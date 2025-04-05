#include "Assign.hpp"
#include <iostream>

using namespace semantics;

Assign::Assign(std::string targetVar, std::shared_ptr<ASTNode> source)
    : targetVar_(targetVar), source_(source) {}

std::shared_ptr<core::Object> Assign::evaluate(runtime::GlobalState& state) {
    std::shared_ptr<core::Object> source = source_->evaluate(state);

    auto& currentScopeMap = state.currentScope().map;
    auto& globalScopeMap = state.globalScope().map;

    if (currentScopeMap.find(targetVar_) != currentScopeMap.end()) {
        // If the target var already exists in the current scope, update its value.
        currentScopeMap[targetVar_] = source;
    } else if (globalScopeMap.find(targetVar_) != globalScopeMap.end()) {
        // If the target var already exists in the global scope, update its value.
        globalScopeMap[targetVar_] = source;
    } else {
        // If the target var does not exist, create it in the current scope.
        currentScopeMap[targetVar_] = source;
    }

    // Assignment return the assigned value as a return value.
    return source;
}