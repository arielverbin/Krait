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

    while (true) {
        if (isAtEnd()) {
            throw except::LexicalError("Unterminated string literal", start.line, start.column);
        }

        char c = nextChar();

        if (c == '\n') {
            throw except::LexicalError("Unterminated string literal before newline", start.line, start.column);
        }

        if (c == openingQuote) {
            // Peek for continuation
            if (shouldContinueString()) {
                openingQuote = nextChar(); // consume new quote
                continue;
            }
            break; // end of string
        }

        if (c == '\\') {
            value += parseEscapeSequence();
        } else {
            value += c;
        }
    }

    return Token(TokenType::STRING, value, start);
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


bool StringHandler::shouldContinueString() const {
    const std::string& source = context_.source;
    LexerPosition& pos = context_.pos;
    LexerPosition lookahead = pos;

    // Skip whitespace and newlines
    while (!isAtEnd(lookahead) && std::isspace(source[lookahead.position])) {
        lookahead.forwards(source[lookahead.position]);
    }

    if (isAtEnd(lookahead)) return false;

    char next = source[lookahead.position];
    if (next == '"' || next == '\'') {
        // valid continuation, update original pos
        pos = lookahead;
        return true;
    }

    return false;
}

} // namespace lexer
