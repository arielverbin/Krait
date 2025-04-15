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

// int main() {
//     std::cout << std::endl;
//     std::cout << "                  TESTING INTERPRETER                  " << std::endl;
//     std::cout << "=======================================================" << std::endl;
//     std::cout << "                Running test commands...               " << std::endl;
//     std::cout << "=================== Test Commands 1 ===================" << std::endl;
//     interpreter::Interpreter i(testCommands1);
//     i.interpret();

//     std::cout << "=================== Test Commands 2 ===================" << std::endl;
//     interpreter::Interpreter i2(testCommands2);
//     i2.interpret();
    
//     std::cout << "============== DONE TESTING INTERPRETER! ==============" << std::endl;
//     std::cout << std::endl;
//     std::cout << "                     TESTING LEXER                     " << std::endl;
//     std::cout << "=======================================================" << std::endl;
//     std::cout << "                Running test commands...               " << std::endl;
//     std::cout << "=================== Test Commands 1 ===================" << std::endl;
//     lexer::Lexer lex("myvar_cool_var2_1 = 	15 + 3 * (3 - 4)\n   \nx=3 + \t 3");
//     streamTokens(lex);
//     std::cout << "=================== Test Commands 2 ===================" << std::endl;
//     lexer::Lexer lex2(
//         "def myFunc():\n"
//             "    print(\"hello\")\n"
//             "        if True:\n"
//             "            print(\"inner\")\n"
//             "            continue\n"
//             "        else:\n"
//             "            print(\"else\")\n"
//             "            break\n\n"
//             "            def hello():\n"
//             "                print(\"hello\")\n"
//             "    func(1, 2\n"
//             "              , 3)\n"
//             "\n"
//             "\n"
//             "myFunc()\n"
//             "print(\"done\")\n"
//     );
        
//     streamTokens(lex2);

//     std::cout << "=================== Test Commands 3 ===================" << std::endl;
//     lexer::Lexer lex3(
//         " print(\n"
//         "    \"hello\"\n"
//         "       'world'\n"
//         " )\n"
//         "if True:\n"
//         "    print(arg1, arg2\n"
//         " ,arg3)\n"
//         "print(1, 2, 3)\n"

//     );
        
//     streamTokens(lex3);

//     std::cout << "=================== Test Commands 4 ===================" << std::endl;
//     lexer::Lexer lex4(
//         "print(\"hello this is a very long string\"\n"
// 		"   \" this is more of this string\")"
//     );
        
//     streamTokens(lex4);
//     return 0;
// }
