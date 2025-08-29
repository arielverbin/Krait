#include <iostream>
#include "Environment.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "exceptions/exceptions.hpp"
using namespace runtime;

core::Scope* Environment::pushNewScope() {
    scopeStack_.push_back(gc::make_tracked<core::Scope>());
    return scopeStack_.back();
}

void Environment::pushScope(core::Scope* scope) {
    scopeStack_.push_back(scope);
}

core::Scope* Environment::popLastScope() {
    core::Scope* lastScope = scopeStack_.back();
    scopeStack_.pop_back();
    return lastScope;
}

core::Object* Environment::getVariable(std::string varName) {
    for (auto scopeIt = scopeStack_.rbegin(); scopeIt != scopeStack_.rend(); ++scopeIt) {
        core::Scope* scope = *scopeIt;
        if (scope->hasMember(varName)) return scope->getMember(varName);
    }
    return nullptr;
}

void Environment::setVariable(std::string varName, core::Object* value) {
    for (auto scopeIt = scopeStack_.rbegin(); scopeIt != scopeStack_.rend(); ++scopeIt) {
        core::Scope* scope = *scopeIt;

        if (scope->hasMember(varName)) {
            scope->setMember(varName, value);
            return;
        }
    }
    if (scopeStack_.size() > 0) {
        // Variable does not exists, create a new one inside the top scope.
        (*scopeStack_.back()).setMember(varName, value);
    } else {
        throw except::RuntimeError("whoa! program runs without a scope");
    }
}

void Environment::defineVariable(std::string varName, core::Object* value) {
    if (scopeStack_.empty()) {
        throw except::RuntimeError("Program runs without a scope.");
    }

    (*scopeStack_.back()).setMember(varName, value);
}

Environment Environment::createChildEnvironment() {
    Environment childEnv(*this);
    childEnv.pushNewScope();
    return childEnv;
}

std::vector<gc::GCTrackable*> Environment::referencees() {
    return std::vector<gc::GCTrackable*>();
}