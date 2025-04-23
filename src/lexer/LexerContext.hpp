#ifndef LEXER_CONTEXT_HPP
#define LEXER_CONTEXT_HPP

#include <string>
#include <queue>
#include "utils/utils.hpp"
#include "Token.hpp"
#include "LexerPosition.hpp"

namespace lexer {
struct LexerContext {
    std::string_view source;            // reference to immutable source code
    LexerPosition pos;                  // mutable current position
    std::deque<Token> pendingTokens;    // for queued-up tokens (INDENT/DEDENT/etc.)
    size_t currentIndentLevel;          // for tracking current indentation level
    size_t currentGroupingLevel;        // for tracking parentheses nesting
    bool lineBroke;                     // for tracking line breaking using a backslash.
    bool doCleanup;                     // Whether do emit cleanup (DEDENT's)

    LexerContext()
        : source(), pos(), pendingTokens({}), currentIndentLevel(0), currentGroupingLevel(0), lineBroke(false), doCleanup(true) {}

    void load(const std::string& src) {
        source = src;
        pos.append();
    }

    void reset() {
        currentIndentLevel = 0;
        currentGroupingLevel = 0;
        pos.reset();
        lineBroke = false;
        pendingTokens = {};
        doCleanup = true;
    }

};

} // namespace lexer

#endif // LEXER_CONTEXT_HPP