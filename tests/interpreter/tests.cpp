#ifdef KRAIT_TESTING

#include <iostream>
#include <vector>

#include "commands.hpp"
#include "interpreter/Interpreter.hpp"
#define CATCH_CONFIG_MAIN

#include "tests/lib/catch.hpp"

TEST_CASE("Interprets while loops and normal flow correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getNormalFlowCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);
    REQUIRE(buffer.str() == "currentNumber is 1\nRES=21\nreachedEnd=False\n-----\ncurrentNumber is 2\n"
                            "RES=22\nreachedEnd=False\n-----\ncurrentNumber is 3\nReached 3!\n");
}

TEST_CASE("Interprets function closures correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getFunctionClosuresCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "Returned Summer with 3\nsumWith3(5) = 8\nsumWith3(10) = 13\nReturned Summer with 5\n"
                            "sumWith5(5) = 10\nsumWith5(10) = 15\nTOTAL CALLS: 0\n");
}

TEST_CASE("Interprets calculations correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getCalculationsCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "7\n26\n16\n105\n4\n25\n");
}

TEST_CASE("Interprets control flow correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getControlFlowTestCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "Odd: 1\nEven: 2\nOdd: 3\nEven: 4\nOdd: 5\nEven: 6\nBreak at 7\n");
}

TEST_CASE("Interprets recursion correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getRecursionTestCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "120\n13\n");
}

TEST_CASE("Interprets logical operators correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getLogicalOperatorsTestCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "False\nTrue\nFalse\nTrue\n");
}

TEST_CASE("Interprets nested closures correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getNestedClosureTestCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "18\n");
}

TEST_CASE("Interprets nested if-else correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getNestedIfElseTestCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "Grade: C\n");
}

TEST_CASE("Interprets nested loops correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getNestedLoopTestCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "i=0, j=0\ni=0, j=1\ni=0, j=2\nBreaking inner loop at j=3\ni=1, j=0\ni=1, j=1\ni=1, j=2\n"
                            "Breaking inner loop at j=3\ni=2, j=0\ni=2, j=1\ni=2, j=2\nBreaking inner loop at j=3\n");
}

TEST_CASE("Interprets functions without arguments correctly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getNoArgFunctionTestCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "Hello, world!\n");
}

TEST_CASE("Interprets basic property access properly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getBasicAccessPropertyCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "10\n100\n");
}

TEST_CASE("Interprets basic class definition properly") {
    // Redirect std::cout
    std::ostringstream buffer;
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf());

    interpreter::Interpreter i;
    i.interpret(CODE(getBasicClassCode()));

    // Restore std::cout
    std::cout.rdbuf(old_buf);

    REQUIRE(buffer.str() == "In init with: 12\nA OBJ mem: 12\n");
}


#endif // KRAIT_TESTING