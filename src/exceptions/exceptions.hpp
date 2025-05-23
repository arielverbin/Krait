#ifndef KRAIT_EXCEPTIONS_HPP
#define KRAIT_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>
#include "core/Object.hpp"
#include "core/builtins/builtin_types/String.hpp"

namespace except {

// Base exception class for Krait
class KraitException : public std::exception {
protected:
    std::string message;
public:
    explicit KraitException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class SyntaxError : public KraitException {
public:
    explicit SyntaxError(const std::string& msg, size_t line, size_t column)
        : KraitException(msg), line_(line), column_(column) {}
    
    size_t line() const { return line_; }
    size_t column() const { return column_; }

private:
    size_t line_;
    size_t column_;
};

class RuntimeError : public KraitException {
public:
    explicit RuntimeError(const std::string& msg)
        : KraitException(msg) {}
};

// Exception for invalid arguments
class InvalidArgumentException : public RuntimeError {
public:
    explicit InvalidArgumentException(const std::string& msg)
        : RuntimeError("InvalidArgumentException: " + msg) {}
};

// Exception for invalid types
class TypeError : public RuntimeError {
public:
    explicit TypeError(const std::string& msg) 
        : RuntimeError("TypeError: " + msg) {}
};

// Exception for attribute erros
class AttributeError : public RuntimeError {
public:
    explicit AttributeError(const std::string& msg)
        : RuntimeError("AttributeError: " + msg) {}
};

// Exception for variable not found
class VariableNotFoundException : public RuntimeError {
public:
    explicit VariableNotFoundException(const std::string& varName)
        : RuntimeError("VariableNotFoundException: " + varName) {}
};

// Exception for type mismatch
class TypeMismatchException : public RuntimeError {
public:
    explicit TypeMismatchException(const std::string& expected, const std::string& actual)
        : RuntimeError("TypeMismatchException: expected " + expected + ", got " + actual) {}
};

// Exception for division by zero
class DivisionByZeroException : public RuntimeError {
public:
    explicit DivisionByZeroException(core::Object& dividend)
        : RuntimeError("DivisionByZeroException") {
            message += ": tried to divide " + static_cast<std::string>(*dividend.toString()) + " with 0";
        }
};

// Exception for not implemented features
class NotImplementedException : public RuntimeError {
public:
    explicit NotImplementedException(const std::string& msg)
        : RuntimeError("NotImplementedException: " + msg) {}
};

} // namespace except

#endif // KRAIT_EXCEPTIONS_HPP
