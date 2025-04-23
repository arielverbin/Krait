#include "TokenHandler.hpp"
namespace lexer {

CleanupHandler::CleanupHandler(LexerContext& context) : TokenHandler(context) {}

bool CleanupHandler::match() const {
    return isAtEnd();
}

std::optional<Token> CleanupHandler::emit() const {
    if (context_.doCleanup) {
        for (size_t i = 0; i < context_.currentIndentLevel; ++i) {
            context_.pendingTokens.push_back(Token(TokenType::DEDENT, "", context_.pos));
        }
        context_.currentIndentLevel = 0;
    }

    context_.pendingTokens.push_back(Token(TokenType::END_OF_FILE, "", context_.pos));
    return std::nullopt;
}

} // namespace lexer