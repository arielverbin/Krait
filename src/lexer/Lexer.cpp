#include "Lexer.hpp"
#include "exceptions/exceptions.hpp"
#include "lexer/token_handlers/TokenHandler.hpp"
#include <iostream>

namespace lexer {

Lexer::Lexer() : context_() {
    // The order of these is important and sets the priorities.
    handlers_.emplace_back(std::make_unique<NewlineHandler>(context_));
    handlers_.emplace_back(std::make_unique<IdentifierHandler>(context_));
    handlers_.emplace_back(std::make_unique<NumberHandler>(context_));
    handlers_.emplace_back(std::make_unique<StringHandler>(context_));
    handlers_.emplace_back(std::make_unique<OperatorHandler>(context_));
    handlers_.emplace_back(std::make_unique<GroupingHandler>(context_));
    handlers_.emplace_back(std::make_unique<WhitespaceHandler>(context_));
    handlers_.emplace_back(std::make_unique<CleanupHandler>(context_));
}

std::vector<Token> Lexer::tokenize(const std::string& code, bool doCleanup) {
    context_.load(code);
    context_.doCleanup = doCleanup;

    std::vector<Token> res;
    do {
        Token t = nextToken();
        res.push_back(t);
    } while (res.back().type() != TokenType::END_OF_FILE);

    if (!context_.doCleanup) res.pop_back();
    else reset();

    return res;
}

Token Lexer::nextToken() {
    while (true) {
        if (!context_.pendingTokens.empty()) {
            Token token = context_.pendingTokens.front();
            context_.pendingTokens.pop_front();
            return token;
        }

        bool wasMatched = false;
        for (auto& handler : handlers_) {
            auto prevPos = context_.pos.position;
            auto prevPending = context_.pendingTokens.size();

            if (handler->match()) {
                wasMatched = true;
                if (auto token = handler->emit()) {
                    return *token;
                }
                if (prevPos < context_.pos.position) break;
                if (prevPending < context_.pendingTokens.size()) break;
            }
        }

        if (!wasMatched) {
            char c = context_.source[context_.pos.position];
            throw except::SyntaxError(
                "Unexpected character '" + std::string(1, c) + "' (" + std::to_string(static_cast<int>(c)) + ")",
                context_.pos.line,
                context_.pos.column
            );
        }
    }
}

void Lexer::reset() {
    context_.reset();
}

} // namespace lexer
