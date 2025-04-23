#ifndef LEXER_TOKEN_HPP
#define LEXER_TOKEN_HPP

#include <string>
#include "LexerPosition.hpp"

namespace lexer {
enum class TokenType {
    IDENTIFIER, // variable names

    // data types
    INT, FLOAT,
    STRING,
    TRU, FALS,
    NONE,

    // keywords
    DEF, RETURN, IF, ELSE, WHILE, PRINT,
    PASS, BREAK, CONTINUE,

    // operators
    ASSIGN,      // '='
    PLUS,        // '+'
    MINUS,       // '-'
    STAR,        // '*'
    SLASH,       // '/'
    DOT,         // '.'
    MOD,         // '%'

    // punctuation
    LPAREN, RPAREN,
    COLON, COMMA,

    // comparison operators
    EQ, NEQ, LT, LTE, GT, GTE,

    // logical operators
    AND, OR, NOT,

    // special tokens
    INDENT, DEDENT,
    NEWLINE,
    END_OF_FILE,

    // unknown token, used for error handling
    UNKNOWN
};

class Token {
private:
    TokenType type_;
    std::string value_;
    size_t line_;
    size_t column_;

public:
    Token(TokenType type, const std::string& value, const LexerPosition& pos);
    TokenType type() const;
    const std::string& value() const;
    size_t line() const;
    size_t column() const;
};

} // namespace lexer

#endif // LEXER_TOKEN_HPP