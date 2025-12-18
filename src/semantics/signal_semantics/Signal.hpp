#ifndef SEMANTICS_SIGNAL_HPP
#define SEMANTICS_SIGNAL_HPP

#include "semantics/ASTNode.hpp"
#include "exceptions/exceptions.hpp"

namespace semantics {

class Signal : public except::RuntimeError {
public:
    Signal(const std::string& message) : except::RuntimeError(message) {}
};

class ReturnSignal : public Signal {
private:
    core::Object* value_;
    
public:
    ReturnSignal(core::Object* value) : Signal("encountered a return statement in an unexpected place"), value_(std::move(value)) {}
    
    core::Object* value() const { return value_; }
};

class BreakSignal : public Signal {
public:
    BreakSignal() : Signal("encountered a break statement in an unexpected place") {}
};

class ContinueSignal : public Signal {
public:
    ContinueSignal() : Signal("encountered a continue statement in an unexpected place") {}
};

} // namespace semantics

#endif // SEMANTICS_SIGNAL_HPP
