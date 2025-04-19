#include "TokenHandler.hpp"
#include "exceptions/exceptions.hpp"

namespace lexer {

StringHandler::StringHandler(LexerContext& context) : TokenHandler(context) {}

bool StringHandler::match() const {
    char c = peekChar();
    return !isAtEnd() && (c == '"' || c == '\'');
}

std::optional<Token> StringHandler::emit() const {
    LexerPosition& pos = context_.pos;
    LexerPosition start = pos;
    std::string value;

    char openingQuote = nextChar(); // consume opening quote

    while (!isAtEnd()) {
        char c = nextChar();

        if (c == '\n') throw except::LexicalError("Unterminated string literal before newline", start.line, start.column);
        if (c == openingQuote) return Token(TokenType::STRING, value, start);

        if (c == '\\') {
            value += parseEscapeSequence();
        } else {
            value += c;
        }
    }

    throw except::LexicalError("Unterminated string literal", start.line, start.column);
    
}

char StringHandler::parseEscapeSequence() const {
    if (isAtEnd()) return '\\'; // handle edge case
    char next = nextChar();
    switch (next) {
        case 'n': return '\n';
        case 't': return '\t';
        case '\\': return '\\';
        case '"': return '"';
        case '\'': return '\'';
        default: return next; // unknown escapes are just passed through
    }
}

} // namespace lexer
