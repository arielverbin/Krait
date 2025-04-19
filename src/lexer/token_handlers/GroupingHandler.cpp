#include "TokenHandler.hpp"

namespace lexer {

GroupingHandler::GroupingHandler(LexerContext& context) : TokenHandler(context) {}

bool GroupingHandler::match() const {
    char c = peekChar();
    return c == '(' || c == ')' || c == '\\';
}

std::optional<Token> GroupingHandler::emit() const {
    LexerPosition& pos = context_.pos;
    char c = nextChar();

    if (c == '(') {
        context_.currentGroupingLevel++;
        return Token(TokenType::LPAREN, "(", pos);
    } else if (c == ')') {
        context_.currentGroupingLevel--;
        return Token(TokenType::RPAREN, ")", pos);
    } else if (c == '\\') {
        context_.lineBroke = true;
        return std::nullopt;
    }
    return Token(TokenType::UNKNOWN, "\\n", pos);
}

} // namespace lexer