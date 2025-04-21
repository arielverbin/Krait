#ifndef KRAIT_EXCEPTIONS_HPP
#define KRAIT_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

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

// Exception for invalid arguments
class InvalidArgumentException : public KraitException {
public:
    explicit InvalidArgumentException(const std::string& msg)
        : KraitException("Invalid Argument: " + msg) {}
};

// Exception for runtime errors
class RuntimeException : public KraitException {
public:
    explicit RuntimeException(const std::string& msg)
        : KraitException("Runtime Error: " + msg) {}
};

// Exception for variable not found
class VariableNotFoundException : public RuntimeException {
public:
    explicit VariableNotFoundException(const std::string& varName)
        : RuntimeException("Variable not found: " + varName) {}
};

// Exception for type mismatch
class TypeMismatchException : public RuntimeException {
public:
    explicit TypeMismatchException(const std::string& expected, const std::string& actual)
        : RuntimeException("Type mismatch: expected " + expected + ", got " + actual) {}
};

// Exception for division by zero
class DivisionByZeroException : public RuntimeException {
public:
    explicit DivisionByZeroException()
        : RuntimeException("Division by zero") {}
};

// Exception for not implemented features
class NotImplementedException : public RuntimeException {
public:
    explicit NotImplementedException(const std::string& msg)
        : RuntimeException("Not Implemented: " + msg) {}
};

class LexicalError : public KraitException {
public:
    explicit LexicalError(const std::string& msg, size_t line, size_t column)
        : KraitException("Lexical Error at line " + std::to_string(line) +
                         ", column " + std::to_string(column) + ": " + msg) {}
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


} // namespace except

#endif // KRAIT_EXCEPTIONS_HPP
