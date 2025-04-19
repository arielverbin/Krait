#ifdef KRAIT_TESTING
#include <iostream>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "tests/lib/catch.hpp"
#include "tests/interpreter/commands.hpp"
#include "tests/interpreter/utils.hpp"
#include "parser/Parser.hpp"

#include "snippets.hpp"
#include "utils.hpp"

#include "lexer/Lexer.hpp"

TEST_CASE("Tokenises expressions correctly") {
    const std::string code = "x = 1 + 2\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) == "IDENTIFIER(x), ASSIGN(=), NUMBER(1), PLUS(+), NUMBER(2), NEWLINE(\\n), EOF()");
}

TEST_CASE("Tokenises keywords correctly") {
    const std::string code = "if x == 1:\n    pass\nelse:\n    print(x)\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "IF(if), IDENTIFIER(x), EQUAL(==), NUMBER(1), COLON(:), NEWLINE(\\n), "
        "INDENT(), PASS(pass), NEWLINE(\\n), DEDENT(), "
        "ELSE(else), COLON(:), NEWLINE(\\n), "
        "INDENT(), PRINT(print), LPAREN((), IDENTIFIER(x), RPAREN()), NEWLINE(\\n), DEDENT(), EOF()");
}

TEST_CASE("Tokenises multiline strings split by newline") {
    const std::string code = "x = \"start of string\"\n\" continues here\"\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), STRING(start of string continues here), NEWLINE(\\n), EOF()");
}

TEST_CASE("Ignores indentation and newline inside parentheses") {
    const std::string code = "x = (1 +\n    2)\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), LPAREN((), NUMBER(1), PLUS(+), NUMBER(2), RPAREN()), NEWLINE(\\n), EOF()");
}

TEST_CASE("Ignores indentation inside nested parentheses") {
    const std::string code = "x = (1 + (2 +\n        3))\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), LPAREN((), NUMBER(1), PLUS(+), LPAREN((), NUMBER(2), "
        "PLUS(+), NUMBER(3), RPAREN()), RPAREN()), NEWLINE(\\n), EOF()");
}

TEST_CASE("Handles logical operators and comparisons") {
    const std::string code = "x = y and z or not a\nx == y != z <= 10 >= 2 < 4 > 1\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), IDENTIFIER(y), AND(and), IDENTIFIER(z), OR(or), NOT(not), IDENTIFIER(a), NEWLINE(\\n), "
        "IDENTIFIER(x), EQUAL(==), IDENTIFIER(y), NOTEQUAL(!=), IDENTIFIER(z), LESSEQUAL(<=), NUMBER(10), "
        "GREATEREQUAL(>=), NUMBER(2), LESS(<), NUMBER(4), GREATER(>), NUMBER(1), NEWLINE(\\n), EOF()");
}

TEST_CASE("Handles boolean and special literals") {
    const std::string code = "x = True\ny = False\nz = None\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), TRUE(True), NEWLINE(\\n), "
        "IDENTIFIER(y), ASSIGN(=), FALSE(False), NEWLINE(\\n), "
        "IDENTIFIER(z), ASSIGN(=), NONE(None), NEWLINE(\\n), EOF()");
}

TEST_CASE("Handles commas and function calls") {
    const std::string code = "func(a, b, c)\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(func), LPAREN((), IDENTIFIER(a), COMMA(,), IDENTIFIER(b), COMMA(,), "
        "IDENTIFIER(c), RPAREN()), NEWLINE(\\n), EOF()");
}

TEST_CASE("Handles return, break, continue") {
    const std::string code = "return x\nbreak\ncontinue\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "RETURN(return), IDENTIFIER(x), NEWLINE(\\n), BREAK(break), NEWLINE(\\n), CONTINUE(continue), NEWLINE(\\n), EOF()");
}

TEST_CASE("Handles nested blocks and correct indent/dedent") {
    const std::string code = "def f():\n    if x:\n        return 1\n    else:\n        return 2\n";
    auto tokens = lexer::Lexer::tokenize(code);

    REQUIRE(stringifyTokens(tokens) ==
        "DEF(def), IDENTIFIER(f), LPAREN((), RPAREN()), COLON(:), NEWLINE(\\n), "
        "INDENT(), IF(if), IDENTIFIER(x), COLON(:), NEWLINE(\\n), "
        "INDENT(), RETURN(return), NUMBER(1), NEWLINE(\\n), DEDENT(), "
        "ELSE(else), COLON(:), NEWLINE(\\n), "
        "INDENT(), RETURN(return), NUMBER(2), NEWLINE(\\n), DEDENT(), DEDENT(), EOF()");
}

// Tests an empty input file.
TEST_CASE("Empty input") {
    const std::string code = "";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) == "EOF()");
}

// Tests input that starts with a newline.
TEST_CASE("Starts with newline") {
    const std::string code = "\nx=42\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) == "IDENTIFIER(x), ASSIGN(=), NUMBER(42), NEWLINE(\\n), EOF()");
}

// Tests input that starts with a newline.
TEST_CASE("Starts with newlines and spaces") {
    const std::string code = "\n\n  \nx=42\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) == "IDENTIFIER(x), ASSIGN(=), NUMBER(42), NEWLINE(\\n), EOF()");
}

// Tests multiple consecutive newlines and blank lines.
TEST_CASE("Multiple consecutive newlines") {
    const std::string code = "x = 42\n\ny = 13\n";
    auto tokens = lexer::Lexer::tokenize(code);
    // Expect a NEWLINE token for the blank line.
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), NUMBER(42), NEWLINE(\\n), "
        "IDENTIFIER(y), ASSIGN(=), NUMBER(13), NEWLINE(\\n), EOF()");
}

// Tests that trailing spaces do not affect tokenisation.
TEST_CASE("Handles trailing whitespace") {
    const std::string code = "x = 5    \n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) == "IDENTIFIER(x), ASSIGN(=), NUMBER(5), NEWLINE(\\n), EOF()");
}

// Tests conversion of Windows-style CRLF newlines into standard NEWLINE tokens.
TEST_CASE("Handles Windows style newlines") {
    const std::string code = "x = 1\r\ny = 2\r\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), NUMBER(1), NEWLINE(\\n), "
        "IDENTIFIER(y), ASSIGN(=), NUMBER(2), NEWLINE(\\n), EOF()");
}

// Tests that leading indentation on the first line is correctly handled.
TEST_CASE("Handles leading indentation on first line") {
    const std::string code = "    x = 1\n";
    auto tokens = lexer::Lexer::tokenize(code);
    // Here the lexer should produce an INDENT then close it with a DEDENT at EOF.
    REQUIRE(stringifyTokens(tokens) ==
        "INDENT(), IDENTIFIER(x), ASSIGN(=), NUMBER(1), NEWLINE(\\n), DEDENT(), EOF()");
}

// Tests that comments are ignored during tokenisation (assuming comment lines are skipped).
TEST_CASE("Ignores comments") {
    const std::string code = "x = 1 # initialize x\nx=x  + 1";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) == "IDENTIFIER(x), ASSIGN(=), NUMBER(1), NEWLINE(\\n), "
        "IDENTIFIER(x), ASSIGN(=), IDENTIFIER(x), PLUS(+), NUMBER(1), EOF()");
}

// Tests line continuation using the backslash to ignore the newline.
TEST_CASE("Line continuation with backslash") {
    const std::string code = "x = 1 + \\\n2\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), NUMBER(1), PLUS(+), NUMBER(2), NEWLINE(\\n), EOF()");
}

// Tests that a backslash continuation skips over multiple blank/whitespace-only lines.
TEST_CASE("Continuation over multiple blank and whitespace lines") {
    const std::string code =
        "x = 1 + \\\n"
        "\n"
        "   \n"
        " \n"
        "2\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), NUMBER(1), PLUS(+), NUMBER(2), NEWLINE(\\n), EOF()");
}

// Tests that comment-only lines between a backslash and the continued line are skipped.
TEST_CASE("Continuation skips comment-only lines") {
    const std::string code =
        "y = 10 - \\\n"
        "# this is a comment\n"
        "# another comment\n"
        "5\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(y), ASSIGN(=), NUMBER(10), MINUS(-), NUMBER(5), NEWLINE(\\n), EOF()");
}

// Tests multi‑segment continuation (two backslashes), joining three physical lines.
TEST_CASE("Multiple consecutive backslashes for multi‑segment continuation") {
    const std::string code =
        "z = a + \\\n"
        "    b + \\\n"
        "c\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(z), ASSIGN(=), IDENTIFIER(a), PLUS(+), IDENTIFIER(b), PLUS(+), IDENTIFIER(c), NEWLINE(\\n), EOF()");
}

// Tests that a backslash inside a comment does NOT trigger continuation.
TEST_CASE("Backslash in comment does not trigger continuation") {
    const std::string code =
        "x = 3 # comment with backslash \\\n"
        "+ 4\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), NUMBER(3), NEWLINE(\\n), PLUS(+), NUMBER(4), NEWLINE(\\n), EOF()");
}


// Tests string literals that include escaped quotes.
TEST_CASE("String literal with escaped quotes") {
    const std::string code = "x = \"This is a \\\"test\\\" string\"\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), STRING(This is a \"test\" string), NEWLINE(\\n), EOF()");
}

// Tests that a hash symbol inside a string literal is not treated as a comment.
TEST_CASE("String literal containing hash symbol") {
    const std::string code = "x = \"# not a comment\"\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), STRING(# not a comment), NEWLINE(\\n), EOF()");
}

// Tests deeply nested blocks with multiple dedents to check proper INDENT/DEDENT balance.
TEST_CASE("Handles nested blocks with multiple dedents") {
    const std::string code =
        "if x:\n"
        "    if y:\n"
        "        z = 1\n"
        "    else:\n"
        "        z = 2\n"
        "a = 3\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IF(if), IDENTIFIER(x), COLON(:), NEWLINE(\\n), "
        "INDENT(), IF(if), IDENTIFIER(y), COLON(:), NEWLINE(\\n), "
        "INDENT(), IDENTIFIER(z), ASSIGN(=), NUMBER(1), NEWLINE(\\n), "
        "DEDENT(), ELSE(else), COLON(:), NEWLINE(\\n), "
        "INDENT(), IDENTIFIER(z), ASSIGN(=), NUMBER(2), NEWLINE(\\n), "
        "DEDENT(), DEDENT(), IDENTIFIER(a), ASSIGN(=), NUMBER(3), NEWLINE(\\n), EOF()");
}

// Tests that negative numbers are tokenised as a MINUS operator followed by a NUMBER.
TEST_CASE("Handles negative numbers and minus operator") {
    const std::string code = "x = -5\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(x), ASSIGN(=), MINUS(-), NUMBER(5), NEWLINE(\\n), EOF()");
}

// Tests block termination with an implicit dedent at the end of the file.
TEST_CASE("Handles block ending with implicit dedent") {
    const std::string code = "if x:\n    y = 1\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IF(if), IDENTIFIER(x), COLON(:), NEWLINE(\\n), "
        "INDENT(), IDENTIFIER(y), ASSIGN(=), NUMBER(1), NEWLINE(\\n), DEDENT(), EOF()");
}

// Comments after implicit continuation lines (inside brackets).
TEST_CASE("Comments after implicit continuation lines") {
    const std::string code =
        "data = (1,  # first\n"
        "      2,  # second\n"
        "        3)  # third\n";
    auto tokens = lexer::Lexer::tokenize(code);
    REQUIRE(stringifyTokens(tokens) ==
        "IDENTIFIER(data), ASSIGN(=), LPAREN((), NUMBER(1), COMMA(,), NUMBER(2), COMMA(,), NUMBER(3), RPAREN()), NEWLINE(\\n), EOF()");
}

#endif // KRAIT_TESTING