#ifndef SEMANTICS_SIGNAL_HPP
#define SEMANTICS_SIGNAL_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class Signal : public std::exception {
};

class ReturnSignal : public Signal {
private:
    std::shared_ptr<core::Object> value_;
    
public:
    ReturnSignal(std::shared_ptr<core::Object> value) : value_(std::move(value)) {}
    
    std::shared_ptr<core::Object> value() const { return value_; }
    
    const char* what() const noexcept override {
        return "ReturnSignal";
    }
};

class BreakSignal : public Signal {
public:
    BreakSignal() = default;
    
    const char* what() const noexcept override {
        return "BreakSignal";
    }
};

class ContinueSignal : public Signal {
public:
    ContinueSignal() = default;
    
    const char* what() const noexcept override {
        return "ContinueSignal";
    }
};

} // namespace semantics

#endif // SEMANTICS_SIGNAL_HPP
