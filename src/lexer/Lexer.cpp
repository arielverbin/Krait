#include "Lexer.hpp"

#include "exceptions/exceptions.hpp"
#include "lexer/token_handlers/TokenHandler.hpp"

namespace lexer {

Lexer::Lexer(const std::string& input) : context_(input) {
    // The order of these is important and sets the priorities.
    handlers_.emplace_back(std::make_unique<IdentifierHandler>(context_));
    handlers_.emplace_back(std::make_unique<NumberHandler>(context_));
    handlers_.emplace_back(std::make_unique<StringHandler>(context_));
    handlers_.emplace_back(std::make_unique<OperatorHandler>(context_));
    handlers_.emplace_back(std::make_unique<GroupingHandler>(context_));
    handlers_.emplace_back(std::make_unique<NewlineHandler>(context_));
    handlers_.emplace_back(std::make_unique<WhitespaceHandler>(context_));
    handlers_.emplace_back(std::make_unique<CleanupHandler>(context_));
}

Token Lexer::nextToken() {
    while (true) {
        if (!context_.pendingTokens.empty()) {
            Token token = context_.pendingTokens.back();
            context_.pendingTokens.pop_back();
            return token;
        }

        bool wasMatched = false;
        for (auto& handler : handlers_) {
            if (handler->match()) {
                wasMatched = true;
                if (auto token = handler->emit()) {
                    return *token;
                }
                break;
            }
        }

        if (!wasMatched) {
            char c = context_.source[context_.pos.position];
            throw except::LexicalError(
                "Unexpected character '" + std::string(1, c) + "' (" + std::to_string(static_cast<int>(c)) + ")",
                context_.pos.line,
                context_.pos.column
            );
        }
    }
}

std::vector<Token> Lexer::tokenize(const std::string& code) {
    Lexer l(code);
    std::vector<Token> res;

    do {
        Token t = l.nextToken();
        res.push_back(t);
    } while (res.back().type() != TokenType::END_OF_FILE);

    return res;
}

} // namespace lexer
