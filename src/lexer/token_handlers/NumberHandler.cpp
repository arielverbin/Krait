#include "TokenHandler.hpp"
#include <cctype>

namespace lexer {

NumberHandler::NumberHandler(LexerContext& context) : TokenHandler(context) {}

bool NumberHandler::match() const {
    return !isAtEnd() && std::isdigit(peekChar());
}

std::optional<Token> NumberHandler::emit() const {
    LexerPosition& pos = context_.pos;
    LexerPosition start = pos;
    std::string number;

    while (!isAtEnd() && std::isdigit(peekChar())) {
        number += nextChar();
    }

    // Handle optional decimal part
    if (!isAtEnd() && peekChar() == '.') {
        number += nextChar();
        while (!isAtEnd() && std::isdigit(peekChar())) {
            number += nextChar();
        }
        return Token(TokenType::FLOAT, number, start);
    }

    return Token(TokenType::INT, number, start);
}

} // namespace lexer
