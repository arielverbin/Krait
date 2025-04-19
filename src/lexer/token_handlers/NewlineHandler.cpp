#include "TokenHandler.hpp"
#include "exceptions/exceptions.hpp"
namespace lexer {

NewlineHandler::NewlineHandler(LexerContext& context) : TokenHandler(context) {}

bool NewlineHandler::match() const {
    if (context_.pos.position == 0 && !isAtEnd()) return true;
    char c = peekChar();
    // Match if we encounter the end of the line, and we are not inside parentheses.
    return !isAtEnd() && (c == '\n' || c == '#');
}

std::optional<Token> NewlineHandler::emit() const {
    LexerPosition& pos = context_.pos;
    LexerPosition start = context_.pos;

    size_t indent = skipWhitespace();

    if (context_.currentGroupingLevel > 0 || context_.lineBroke) {
        context_.lineBroke = false;
        return std::nullopt;
    }

    if (indent % 4 != 0) {
        throw except::LexicalError("Indentation must be a multiple of 4 spaces",
            pos.line, pos.column);
    }

    indent = indent / 4;

    if (indent > context_.currentIndentLevel) {
        for (size_t i = context_.currentIndentLevel; i < indent; ++i) {
            context_.pendingTokens.push_back(Token(TokenType::INDENT, "", context_.pos));
        }
    } else {
        for (size_t i = context_.currentIndentLevel; i > indent; --i) {
            context_.pendingTokens.push_back(Token(TokenType::DEDENT, "", context_.pos));
        }
    }
    context_.currentIndentLevel = indent;

    if (start.position != 0) {
        return Token(TokenType::NEWLINE, "\\n", start);
    } else return std::nullopt;
}

void NewlineHandler::handleComment() const {
    while (!isAtEnd()) {
        char c = nextChar();
        if (c == '\n') {
            break;
        }
    }
}

size_t NewlineHandler::skipWhitespace() const {
    LexerPosition& pos = context_.pos;
    size_t indent = 0;

    while (!isAtEnd()) {
        char ch = peekChar();
        if (ch == ' ') { nextChar(); indent++; }
        else if (ch == '\n') { nextChar(); indent = 0; }
        else if (ch == '#') { handleComment();  indent = 0; }
        else if (std::isspace(ch)) {
            throw except::LexicalError("Unexpected whitespace (only spaces are allowed)",
                pos.line, pos.column);
        } else {
            return indent; // return the indent level
        };
    }
    return 0;
}


} // namespace lexer
