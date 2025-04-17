#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <functional>
#include "Token.hpp"

namespace lexer {

class Lexer { 
public:
    explicit Lexer(const std::string& source);

    Token peek();             // look at current token without consuming it
    void init();              // Initializes the lexer
    Token next();             // consume current token
    bool isAtEnd() const;

    static std::vector<Token> tokenize(std::string source);

private:
    const std::string source_;
    LexerPosition lexerPosition_;

    bool match(char expected);
    Token identifier();
    Token number();
    Token string();
    
    Token handleOperatorOrPunct(char c); // handles operators and punctuation

    static const std::unordered_map<std::string, TokenType> keywords_;
    
    std::vector<Token> pendingTokens_; // queue of tokens to return
    size_t indentLevel_ = 0; // indentation levels (starts with 0)
    size_t groupingLevel_ = 0;
    
    void skipWhitespace(bool skipNewLine=false);
    Token handleNewlineAndIndent();
    char peekChar() const;
    char nextChar();

    void handleLineSplit();
    void handleComment();

    void emitCleanups();
    Token emitPending();
};

} // namespace lexer
#endif // LEXER_HPP