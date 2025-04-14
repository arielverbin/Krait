#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <stack>
#include <optional>
#include <unordered_map>
#include <functional>

namespace lexer {
enum class TokenType {
    IDENTIFIER, // variable names

    // data types
    INT,
    STRING,
    TRUE, FALSE,
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
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
};

struct LexerPosition {
    size_t line = 1;
    size_t column = 1;
    size_t position = 0;
    std::stack<size_t> lineLengths; // Store length of each line

    void nextChar(char c) {
        column++;
        position++;
        if (c == '\n') {
            lineLengths.push(column);
            line++;
            column = 1;
        }
    }

    void prevChar() {
        if (position == 0) return;
        position--;
        column--;

        if (column == 0) {
            if (lineLengths.empty()) return;
            line--;
            column = lineLengths.top();
            lineLengths.pop();
        }
    }
};

class Lexer { 
public:
    using TokenHandler = std::function<Token(Lexer&)>;

    explicit Lexer(const std::string& source);
    Token makeToken(TokenType type, const std::string& value);
    bool match(char expected);
    Token identifier();
    Token number();
    Token string();

    Token peek();             // look at current token without consuming it
    Token next();             // consume current token
    bool isAtEnd() const;

private:
    const std::string source_;
    LexerPosition lexerPosition_;

    // Handles for each token
    static const std::unordered_map<char, TokenHandler> tokenHandlers_;
    static const std::unordered_map<std::string, TokenType> keywords_;
    
    std::vector<Token> m_bufferedTokens; // queue of tokens to return
    std::vector<int> m_indentStack = { 0 }; // indentation levels (starts with 0)
    
    void skipWhitespace();
    Token handleNewlineAndIndent();
    char peekChar() const;
    char nextChar();
};

} // namespace lexer
#endif // LEXER_HPP