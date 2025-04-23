#include "Token.hpp"
using namespace lexer;

Token::Token(TokenType type, const std::string& value, const LexerPosition& pos)
    : type_(type), value_(value), line_(pos.line), column_(pos.column) {}

TokenType Token::type() const { return type_; }

const std::string& Token::value() const { return value_; }

size_t Token::line() const { return line_; }

size_t Token::column() const { return column_; }
