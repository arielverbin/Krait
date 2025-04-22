#include "TokenHandler.hpp"

namespace lexer {

TokenHandler::TokenHandler(LexerContext& context) : context_(context) {}

bool TokenHandler::isAtEnd(LexerPosition& pos) const {
    return pos.position >= context_.source.size();
}

bool TokenHandler::isAtEnd() const {
    return isAtEnd(context_.pos);
}

char TokenHandler::nextChar() const {
    const std::string_view& source = context_.source;
    LexerPosition& pos = context_.pos;

    if (pos.position >= source.size()) return '\0';
    char c = source[pos.position];
    pos.forwards(source[pos.position]);

    return c;
}

char TokenHandler::peekChar() const {
    const std::string_view& source = context_.source;
    LexerPosition& pos = context_.pos;

    if (pos.position >= source.size()) return '\0';
    return source[pos.position];
}

} // namespace lexer
