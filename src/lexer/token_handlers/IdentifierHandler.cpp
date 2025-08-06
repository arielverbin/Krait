#include "TokenHandler.hpp"
#include <cctype>

namespace lexer {

IdentifierHandler::IdentifierHandler(LexerContext& context)
    : TokenHandler(context), keywords_({
        {"def", TokenType::DEF}, {"class", TokenType::CLASS}, {"return", TokenType::RETURN},
        {"if", TokenType::IF}, {"else", TokenType::ELSE},
        {"while", TokenType::WHILE}, {"print", TokenType::PRINT},
        {"pass", TokenType::PASS}, {"break", TokenType::BREAK},
        {"continue", TokenType::CONTINUE}, {"True", TokenType::TRU},
        {"False", TokenType::FALS}, {"None", TokenType::NONE},
        {"and", TokenType::AND}, {"or", TokenType::OR}, {"not", TokenType::NOT}
    }) {}

bool IdentifierHandler::match() const {
    char c = peekChar();
    return !isAtEnd() && (std::isalpha(c) || c == '_');
}

std::optional<Token> IdentifierHandler::emit() const {
    LexerPosition& pos = context_.pos;
    LexerPosition start = pos;
    std::string result;

    while (!isAtEnd()) {
        char c = peekChar();
        if (std::isalnum(c) || c == '_') {
            result += c;
            nextChar();
        } else {
            break;
        }
    }

    return Token(getTokenType(result), result, start);
}

bool IdentifierHandler::isKeyword(const std::string& text) const {
    return keywords_.find(text) != keywords_.end();
}

TokenType IdentifierHandler::getTokenType(const std::string& text) const {
    auto it = keywords_.find(text);
    return it != keywords_.end() ? it->second : TokenType::IDENTIFIER;
}

} // namespace lexer
