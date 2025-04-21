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
#include "../interpreter/commands.hpp"

#define TEST_EXPRESSION(num) \
TEST_CASE("Parses expressions correctly (" #num ")") { \
    auto tokens = lexer::Lexer::tokenize(expressionTest##num); \
    auto ast = parser::Parser(tokens).parse(); \
    REQUIRE(getResult(expressionTest##num)->stringify() == ast->stringify()); \
}

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

#endif // KRAIT_TESTING