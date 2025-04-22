#ifndef PARSER_CONTEXT_HPP
#define PARSER_CONTEXT_HPP

#include <span>
#include "lexer/Token.hpp"

namespace parser {

struct ParserContext {
    std::span<lexer::Token> tokens;
    size_t current;

    ParserContext() : tokens(), current(0) {}

    void load(std::vector<lexer::Token>& tokenStream) {
        tokens = tokenStream;
        current = 0;
    }
};

}

#endif // PARSER_CONTEXT_HPP