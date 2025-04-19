#ifndef TOKEN_HANDLER_HPP
#define TOKEN_HANDLER_HPP

#include <map>
#include "lexer/LexerContext.hpp"
#include "lexer/LexerPosition.hpp"
#include "lexer/Token.hpp"

namespace lexer {

/**
 * Abstract base class for token handlers.
 * Each handler is responsible for matching and emitting a specific type of token.
 */
class TokenHandler {
public:
    TokenHandler(LexerContext& context);

    /**
     * Checks if the current position in the source string matches the token type.
     * @return true if the token type matches, false otherwise.
     */
    virtual bool match() const = 0;

    /**
     * Tries to emit a token from the source string.
     * @return The emitted token, or optnull.
     */
    virtual std::optional<Token> emit() const = 0;
    virtual ~TokenHandler() = default;

protected:
    // Reference to the lexer context for shared state
    LexerContext& context_;

    /**
     * Returns whether the current position is at the end of the source string.
     */
    bool isAtEnd(LexerPosition& pos) const;
    bool isAtEnd() const;

    /**
     * Returns the next character in the source string and advances the position.
     * If the end of the string is reached, it returns '\0'.
     */
    char nextChar() const;
    /**
     * Returns the next character in the source string without advancing the position.
     * If the end of the string is reached, it returns '\0'.
     */
    char peekChar() const;
};

/**
 * Handles identifiers and keywords.
 * Keywords are defined in the constructor.
 * Identifiers are any alphanumeric string starting with a letter or underscore.
 */
class IdentifierHandler : public TokenHandler {
public:
    IdentifierHandler(LexerContext& context);
    bool match() const override;
    std::optional<Token> emit() const override;

private:
    std::map<std::string_view, TokenType> keywords_;
    bool isKeyword(const std::string& text) const;
    TokenType getTokenType(const std::string& text) const;
};

/**
 * Handles number tokens.
 * Numbers are defined as any sequence of digits.
 */
class NumberHandler : public TokenHandler {
public:
    NumberHandler(LexerContext& context);
    bool match() const override;
    std::optional<Token> emit() const override;
};

/**
 * Handles string tokens.
 * Strings are defined as any sequence of characters enclosed in single or double quotes.
 * Strings can be multi-line and can contain escape sequences.
 */
class StringHandler : public TokenHandler {
public:
    StringHandler(LexerContext& context);
    bool match() const override;
    std::optional<Token> emit() const override;
private:
    char parseEscapeSequence() const;
};

/**
 * Handles operator tokens.
 */
class OperatorHandler : public TokenHandler {
public:
    OperatorHandler(LexerContext& context);
    bool match() const override;
    std::optional<Token> emit() const override;

private:
    struct MatchResult { TokenType type; size_t len; };
    MatchResult tryMatch() const;
    const std::vector<std::pair<std::string, TokenType>> operators_;
};

/**
 * Handles grouping tokens (parentheses).
 * Parentheses are defined as '(' and ')'. Can be extended to handle other grouping symbols.
 */
class GroupingHandler : public TokenHandler {
public:
    GroupingHandler(LexerContext& context);
    bool match() const override;
    std::optional<Token> emit() const override;
};

/**
 * Handles whitespace characters in a single line.
 */
class WhitespaceHandler : public TokenHandler {
public:
    WhitespaceHandler(LexerContext& context);
    bool match() const override;
    std::optional<Token> emit() const override;
};

/**
 * Handles newline cases and indentation tokens, including comments.
 * Indentation is defined as a multiple of 4 spaces.
 */
class NewlineHandler : public TokenHandler {
public:
    NewlineHandler(LexerContext& context);
    bool match() const override;
    std::optional<Token> emit() const override;
private:
    /**
     * Skips the entire comment line.
     * Comments start with '#' and continue until the end of the line.
     */
    void handleComment() const;
    /**
     * Skips whitespace characters in the source string.
     * @return the indent level on the first non-empty line encountered.
     */
    size_t skipWhitespace() const;
};
    
/**
 * Handles cleanup when the end of the source string is reached.
 * For instance, emitting implicit DEDENT's.
 */
class CleanupHandler : public TokenHandler {
public:
    CleanupHandler(LexerContext& context);
    bool match() const override;
    std::optional<Token> emit() const override;
};

} // namespace lexer

#endif
