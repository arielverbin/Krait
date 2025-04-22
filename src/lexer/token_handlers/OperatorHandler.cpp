#include "TokenHandler.hpp"
#include "exceptions/exceptions.hpp"
namespace lexer {

OperatorHandler::OperatorHandler(LexerContext& context)
    : TokenHandler(context), operators_({
        // Must be sorted in descending length to ensure longest match first
        {"==", TokenType::EQ}, {"!=", TokenType::NEQ},
        {"<=", TokenType::LTE}, {">=", TokenType::GTE},
        
        {"+", TokenType::PLUS}, {"-", TokenType::MINUS}, {"*", TokenType::STAR},
        {"/", TokenType::SLASH}, {"=", TokenType::ASSIGN}, {"<", TokenType::LT},
        {">", TokenType::GT}, {"!", TokenType::NOT}, {":", TokenType::COLON},
        {"%", TokenType::MOD}, {",", TokenType::COMMA}, {".", TokenType::DOT},
    }) {}

bool OperatorHandler::match() const {
    return tryMatch().type != TokenType::UNKNOWN;
}

std::optional<Token> OperatorHandler::emit() const {
    LexerPosition& pos = context_.pos;
    auto start = pos;
    auto result = tryMatch();

    if (result.type == TokenType::UNKNOWN) {
        throw except::SyntaxError("Unknown operator", pos.line, pos.column);
    }

    std::string res;

    for (size_t i = 0; i < result.len; ++i) {
        res += nextChar();
    }

    return Token(result.type, res, start);
}

OperatorHandler::MatchResult OperatorHandler::tryMatch() const {
    const std::string_view& source = context_.source;
    LexerPosition& pos = context_.pos;
    std::string_view view = std::string_view(source).substr(pos.position);

    for (const auto& [op, tokType] : operators_) {
        if (view.starts_with(op)) {
            return {tokType, op.size()};
        }
    }

    return {TokenType::UNKNOWN, 0};
}

} // namespace lexer