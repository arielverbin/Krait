#ifdef KRAIT_TESTING
#include <iostream>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "tests/lib/catch.hpp"
#include "tests/interpreter/utils.hpp"
#include "parser/Parser.hpp"

#include "snippets.hpp"
#include "utils.hpp"

#include "lexer/Lexer.hpp"
#include "../interpreter/commands.hpp"

#define TEST_EXPRESSION(num) \
TEST_CASE("Parses expressions correctly (" #num ")") { \
    runtime::EvalContext::initGlobalContext(); \
    auto tokens = lexer::Lexer().tokenize(expressionTest##num); \
    try { \
        auto ast = parser::Parser().parse(tokens); \
        REQUIRE(getResult(expressionTest##num)->stringify() == ast->stringify()); \
    } catch (const except::SyntaxError& err) { \
        printSyntaxError(err, expressionTest##num); \
        REQUIRE(false); \
    } \
    runtime::EvalContext::popContext(); \
}

// Expressions, precedence and associativity
TEST_EXPRESSION(1)
TEST_EXPRESSION(2)
TEST_EXPRESSION(3)
TEST_EXPRESSION(4)
TEST_EXPRESSION(5)
TEST_EXPRESSION(6)
TEST_EXPRESSION(7)
TEST_EXPRESSION(8)
TEST_EXPRESSION(9)
TEST_EXPRESSION(10)
TEST_EXPRESSION(11)
TEST_EXPRESSION(12)
TEST_EXPRESSION(13)
TEST_EXPRESSION(14)
TEST_EXPRESSION(15)
TEST_EXPRESSION(16)
TEST_EXPRESSION(17)
TEST_EXPRESSION(18)
TEST_EXPRESSION(19)
TEST_EXPRESSION(20)
TEST_EXPRESSION(21)

// functions, if, print and while statements
TEST_EXPRESSION(22)
TEST_EXPRESSION(23)
TEST_EXPRESSION(24)
TEST_EXPRESSION(25)
TEST_EXPRESSION(26)

// function calls
TEST_EXPRESSION(27)
TEST_EXPRESSION(28)
TEST_EXPRESSION(29)
TEST_EXPRESSION(30)
TEST_EXPRESSION(31)
TEST_EXPRESSION(32)
TEST_EXPRESSION(33)

#endif // KRAIT_TESTING