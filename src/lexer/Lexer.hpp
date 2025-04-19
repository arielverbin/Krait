#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <memory>
#include <functional>
#include <optional>
#include <string>

#include "Token.hpp"
#include "LexerContext.hpp"
#include "lexer/token_handlers/TokenHandler.hpp"

namespace lexer {

class Lexer {
public:
    using Handler = std::function<std::optional<Token>(Lexer&)>;

    explicit Lexer(const std::string& input);

    bool isAtEnd();
    Token nextToken();

    static std::vector<Token> tokenize(const std::string& code);

private:
    LexerContext context_;
    std::vector<std::unique_ptr<TokenHandler>> handlers_;
};

} // namespace lexer

#endif // LEXER_HPP
