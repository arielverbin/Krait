#ifndef LEXER_CONTEXT_HPP
#define LEXER_CONTEXT_HPP

#include <string>
#include <queue>
#include "Token.hpp"
#include "LexerPosition.hpp"

namespace lexer {
struct LexerContext {
    const std::string& source;          // reference to immutable source code
    LexerPosition pos;                  // mutable current position
    std::deque<Token> pendingTokens;    // for queued-up tokens (INDENT/DEDENT/etc.)
    size_t currentIndentLevel;          // for tracking current indentation level
    size_t currentGroupingLevel;        // for tracking parentheses nesting
    bool lineBroke;                     // for tracking line breaking using a backslash.

    LexerContext(const std::string& source)
        : source(source), pos(), pendingTokens({}), currentIndentLevel(0), currentGroupingLevel(0), lineBroke(false) {}
};

} // namespace lexer

#endif // LEXER_CONTEXT_HPP