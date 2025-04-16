#include <iostream>
#include <vector>

#include "commands.hpp"
#include "interpreter/Interpreter.hpp"
#define CATCH_CONFIG_MAIN

#include "../lib/catch.hpp"

TEST_CASE("Interprets while loops and normal flow correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(normalFlow);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);
    REQUIRE(buffer.str() == "currentNumber is 1\nRES=21\nreachedEnd=False\n-----\ncurrentNumber is 2\n"
                            "RES=22\nreachedEnd=False\n-----\ncurrentNumber is 3\nReached 3!\n");
}

TEST_CASE("Interprets function closures correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(functionClosures);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "Returned Summer with 3\nsumWith3(5) = 8\nsumWith3(10) = 13\nReturned Summer with 5\n"
                            "sumWith5(5) = 10\nsumWith5(10) = 15\nTOTAL CALLS: 4\n");
}

TEST_CASE("Interprets calculations correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(calculations);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "7\n26\n16\n105\n4\n25\n");
}

TEST_CASE("Interprets control flow correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(controlFlowTest);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "Odd: 1\nEven: 2\nOdd: 3\nEven: 4\nOdd: 5\nEven: 6\nBreak at 7\n");
}

TEST_CASE("Interprets recursion correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(recursionTest);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "120\n13\n");
}

TEST_CASE("Interprets logical operators correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(logicalOperatorsTest);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "False\nTrue\nFalse\nTrue\n");
}

TEST_CASE("Interprets nested closures correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(nestedClosureTest);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "18\n");
}

TEST_CASE("Interprets nested if-else correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(nestedIfElseTest);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "Grade: C\n");
}

TEST_CASE("Interprets nested loops correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(nestedLoopTest);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "i=0, j=0\ni=0, j=1\ni=0, j=2\nBreaking inner loop at j=3\ni=1, j=0\ni=1, j=1\ni=1, j=2\n"
                            "Breaking inner loop at j=3\ni=2, j=0\ni=2, j=1\ni=2, j=2\nBreaking inner loop at j=3\n");
}

TEST_CASE("Interprets functions without arguments correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i(noArgFunctionTest);
    i.interpret();

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "Hello, world!\n");
}


