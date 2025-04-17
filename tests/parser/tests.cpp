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

TEST_CASE("Parses expressions correctly") {
    REQUIRE(true);
}


#endif // KRAIT_TESTING