#include "EvalContext.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "exceptions/exceptions.hpp"
using namespace runtime;

std::stack<EvalContext*> EvalContext::evalContexts_;

std::vector<gc::GCTrackable*> EvalContext::referencees() {
    return std::vector<gc::GCTrackable*>(evalStack_.begin(), evalStack_.end());;
}

EvalContext::EvalGuard EvalContext::Guard() {
    return EvalGuard(*this);
}

void EvalContext::push(gc::GCTrackable* obj) {
    evalStack_.push_back(obj);
}

void EvalContext::pop() {
    evalStack_.pop_back();
}

EvalContext::EvalGuard::EvalGuard(EvalContext& context)
    : context_(context), stackSize_(context_.evalStack_.size()) {
}

void EvalContext::EvalGuard::protect(gc::GCTrackable* obj) {
    if (obj != nullptr) {
        context_.evalStack_.push_back(obj);
    }
}

EvalContext::EvalGuard::~EvalGuard() {
    context_.evalStack_.resize(stackSize_);
}

void EvalContext::pushContext(EvalContext* ec) { evalContexts_.push(ec); }
void EvalContext::popContext() {
    if (evalContexts_.empty()) {
        // should never happen
        throw std::runtime_error("evaluation context is empty");
    }

    evalContexts_.pop();
}

EvalContext* EvalContext::initGlobalContext() {
    EvalContext* globalContext_ = gc::make_tracked<EvalContext>();
    gc::GarbageCollector::instance().defineRoot(globalContext_);
    evalContexts_.push(globalContext_);
    return globalContext_;
}

EvalContext& EvalContext::current() {
    if (evalContexts_.empty()) {
        // should never happen
        throw std::runtime_error("evaluation context is empty");
    }

    return *evalContexts_.top();
}
