#include "EvalContext.hpp"
#include "exceptions/exceptions.hpp"
using namespace runtime;

std::stack<EvalContext*> EvalContext::evalContexts_;

std::vector<gc::GCTrackable*> EvalContext::referencees() {
    return std::vector<gc::GCTrackable*>(evalStack_.begin(), evalStack_.end());;
}

EvalContext::EvalGuard EvalContext::Guard() {
    return EvalGuard(*this);
}

EvalContext::EvalGuard EvalContext::Guard() {
    return EvalGuard(*this);
}

EvalContext::EvalGuard::EvalGuard(EvalContext& context)
    : context_(context), stackSize_(context_.evalStack_.size()) {
}

void EvalContext::EvalGuard::protect(core::Object* obj) {
    if (obj != nullptr) {
        context_.evalStack_.push_back(obj);
    }
}

EvalContext::EvalGuard::~EvalGuard() {
    if (context_.evalStack_.size() < stackSize_) throw except::RuntimeError("wtf");

    context_.evalStack_.resize(stackSize_);
}

void EvalContext::push(EvalContext* ec) { evalContexts_.push(ec); }
void EvalContext::pop() { evalContexts_.pop(); }
EvalContext& EvalContext::current() { return *evalContexts_.top(); }
