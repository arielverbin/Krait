#include "TokenHandler.hpp"
#include <algorithm>

namespace lexer {

GroupingHandler::GroupingHandler(LexerContext& context) : TokenHandler(context), 
    openings_({
        {'(', TokenType::LPAREN},
        {'[', TokenType::LBRACKET},
    }),
    closings_({
        {')', TokenType::RPAREN},
        {']', TokenType::RBRACKET},
    }) {}

bool GroupingHandler::match() const {
    char c = peekChar();

    bool op = std::any_of(openings_.begin(), openings_.end(),
        [c](const auto& pair) { return pair.first == c; }) ||
        std::any_of(closings_.begin(), closings_.end(),
        [c](const auto& pair) { return pair.first == c; });

    return op || c == '\\';
}

std::optional<Token> GroupingHandler::emit() const {
    LexerPosition& pos = context_.pos;
    char c = nextChar();

    if (c == '\\') {
        context_.lineBroke = true;
        return std::nullopt;
    }

    auto openingIt = std::find_if(openings_.begin(), openings_.end(),
        [c](const auto& pair) { return pair.first == c; });
    if (openingIt != openings_.end()) {
        context_.currentGroupingLevel++;
        return Token(openingIt->second, std::string(1, c), pos);
    }

    auto closingIt = std::find_if(closings_.begin(), closings_.end(),
        [c](const auto& pair) { return pair.first == c; });
    if (closingIt != closings_.end()) {
        context_.currentGroupingLevel--;
        return Token(closingIt->second, std::string(1, c), pos);
    }

    return Token(TokenType::UNKNOWN, "\\n", pos);
}

} // namespace lexer