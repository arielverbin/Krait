#include "TokenHandler.hpp"
#include <cctype>

namespace lexer {

WhitespaceHandler::WhitespaceHandler(LexerContext& context) : TokenHandler(context) {}

bool WhitespaceHandler::match() const {
    char c = peekChar();
    return (!isAtEnd()) && std::isspace(c) && (c != '\n');
}

std::optional<Token> WhitespaceHandler::emit() const {
    while (!isAtEnd() && std::isspace(peekChar()) && peekChar() != '\n') {
        nextChar();
    }
    return std::nullopt;
}

} // namespace lexer
