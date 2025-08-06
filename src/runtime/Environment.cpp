#include <iostream>
#include "Environment.hpp"
#include "exceptions/exceptions.hpp"
using namespace runtime;

std::shared_ptr<utils::Scope> Environment::pushNewScope() {
    scopeStack_.push_back(std::make_shared<utils::Scope>());
    return scopeStack_.back();
}

void Environment::pushScope(std::shared_ptr<utils::Scope> scope) {
    scopeStack_.push_back(scope);
}

std::shared_ptr<utils::Scope> Environment::popLastScope() {
    std::shared_ptr<utils::Scope> lastScope = scopeStack_.back();
    scopeStack_.pop_back();
    return lastScope;
}

std::shared_ptr<core::Object> Environment::getVariable(std::string varName) {
    for (auto scopeIt = scopeStack_.rbegin(); scopeIt != scopeStack_.rend(); ++scopeIt) {
        utils::Scope& scope = **scopeIt;

        if (scope.find(varName) != scope.end()) {
            if (std::holds_alternative<utils::LazyValue>(scope[varName])) {
                // It's a LazyValue — resolve and cache
                utils::LazyValue& lazy = std::get<utils::LazyValue>(scope[varName]);
                std::shared_ptr<core::Object> resolved = lazy.creator();
                scope[varName] = resolved;
            }

            return std::get<std::shared_ptr<core::Object>>(scope[varName]);
        }
    }
    return nullptr;
}

void Environment::setVariable(std::string varName, std::shared_ptr<core::Object> value) {
    for (auto scopeIt = scopeStack_.rbegin(); scopeIt != scopeStack_.rend(); ++scopeIt) {
        utils::Scope& scope = **scopeIt;

        if (scope.find(varName) != scope.end()) {
            scope[varName] = value;
            return;
        }
    }
    if (scopeStack_.size() > 0) {
        // Variable does not exists, create a new one inside the top scope.
        (*scopeStack_.back())[varName] = value;
    } else {
        throw except::RuntimeError("program runs without a scope.");
    }
}

void Environment::defineVariable(std::string varName, std::shared_ptr<core::Object> value) {
    if (scopeStack_.empty()) {
        throw except::RuntimeError("Program runs without a scope.");
    }

    (*scopeStack_.back())[varName] = value;
}

Environment Environment::createChildEnvironment() {
    Environment childEnv(*this);
    childEnv.pushNewScope();
    return childEnv;
}
