#include <iostream>
#include "Frame.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "exceptions/exceptions.hpp"
using namespace runtime;

Frame::Frame() : core::Object(core::KraitBuiltins::frameType, true), evalContext_(gc::make_tracked<EvalContext>()) {}
Frame::Frame(const Frame& env)
    : core::Object(core::KraitBuiltins::frameType, true),
    scopeStack_(env.scopeStack_),
    evalContext_(gc::make_tracked<EvalContext>()) {}

EvalContext* Frame::context() {
    return evalContext_;
}

core::Scope* Frame::pushNewScope() {
    scopeStack_.push_back(gc::make_tracked<core::Scope>());
    return scopeStack_.back();
}

void Frame::pushScope(core::Scope* scope) {
    scopeStack_.push_back(scope);
}

core::Scope* Frame::popLastScope() {
    core::Scope* lastScope = scopeStack_.back();
    scopeStack_.pop_back();
    return lastScope;
}

core::Object* Frame::getVariable(std::string varName) {
    for (auto scopeIt = scopeStack_.rbegin(); scopeIt != scopeStack_.rend(); ++scopeIt) {
        core::Scope* scope = *scopeIt;
        if (scope->hasMember(varName)) return scope->getMember(varName);
    }
    return nullptr;
}

// void Frame::setVariable(std::string varName, core::Object* value) {
//     for (auto scopeIt = scopeStack_.rbegin(); scopeIt != scopeStack_.rend(); ++scopeIt) {
//         core::Scope* scope = *scopeIt;

//         if (scope->hasMember(varName)) {
//             scope->setMember(varName, value);
//             return;
//         }
//     }
//     if (scopeStack_.size() > 0) {
//         // Variable does not exists, create a new one inside the top scope.
//         (*scopeStack_.back()).setMember(varName, value);
//     } else {
//         throw except::RuntimeError("whoa! program runs without a scope");
//     }
// }

void Frame::defineVariable(std::string varName, core::Object* value) {
    if (scopeStack_.empty()) {
        throw except::RuntimeError("program runs without a scope.");
    }

    (*scopeStack_.back()).setMember(varName, value);
}

std::vector<gc::GCTrackable*> Frame::referencees() {
    auto referencees_ = Object::referencees();
    for (auto scope : scopeStack_) {
        referencees_.push_back(scope);
    }
    referencees_.push_back(evalContext_);
    return referencees_;
}
