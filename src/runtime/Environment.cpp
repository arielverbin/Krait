#include <iostream>
#include "Environment.hpp"
#include "exceptions/exceptions.hpp"
using namespace runtime;

std::shared_ptr<Scope> Environment::pushNewScope() {
    scopeStack_.emplace_back(std::make_shared<Scope>());
    return scopeStack_.back();
}

void Environment::popLastScope() {
    scopeStack_.pop_back();
}

std::shared_ptr<core::Object> Environment::getVariable(std::string varName) {
    for (auto scope = scopeStack_.rbegin(); scope != scopeStack_.rend(); ++scope) {
        if ((*scope)->map.find(varName) != (*scope)->map.end()) {
            return (*scope)->map[varName];
        }
    }
    return nullptr;
}

void Environment::setVariable(std::string varName, std::shared_ptr<core::Object> value) {
    for (auto scope = scopeStack_.rbegin(); scope != scopeStack_.rend(); ++scope) {
        if ((*scope)->map.find(varName) != (*scope)->map.end()) {
            (*scope)->map[varName] = value;
            return;
        }
    }
    if (scopeStack_.size() > 0) {
        // Variable does not exists, create a new one inside the top scope.
        scopeStack_.back()->map[varName] = value;
    } else {
        throw except::RuntimeException("Program runs without a scope.");
    }
}

Environment Environment::createChildEnvironment() {
    Environment childEnv(*this);
    childEnv.pushNewScope();
    return childEnv;
}
