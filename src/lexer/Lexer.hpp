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
    Lexer();

    /**
     * Tokenise the given source code.
     * @param code the source code to tokenize.
     * @param doCleanup whether to emit cleanup tokens at the end. This is useful for cases where
     *                  the source code is not complete.
     * 
     * @return the continuation of the stream of tokens.
     *
     * NOTE:  Lexer is stateful. it will continue to emit tokens from where it left.
     *        The last call to tokenize must set doCleanup to true, in order to ensure a proper
     *        ending of the stream.
     */
    std::vector<Token> tokenize(const std::string& code, bool doCleanup = true);

    /**
     * Resets the Lexer's state. A call to tokenize with doCleanup also resets the lexer.
     */
    void reset();

private:
    Token nextToken();

    LexerContext context_;
    std::vector<std::unique_ptr<TokenHandler>> handlers_;
};

} // namespace lexer

#endif // LEXER_HPP
