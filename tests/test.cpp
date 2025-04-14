#include <iostream>
#include <vector>

#include "commands.hpp"
#include "interpreter/Interpreter.hpp"
#include "lexer/Lexer.hpp"

int main() {
    std::cout << std::endl;
    std::cout << "                  TESTING INTERPRETER                  " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "                Running test commands...               " << std::endl;
    std::cout << "=================== Test Commands 1 ===================" << std::endl;
    interpreter::Interpreter i(testCommands1);
    i.interpret();

    std::cout << "=================== Test Commands 2 ===================" << std::endl;
    interpreter::Interpreter i2(testCommands2);
    i2.interpret();
    
    std::cout << "============== DONE TESTING INTERPRETER! ==============" << std::endl;
    std::cout << std::endl;
    std::cout << "                     TESTING LEXER                     " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "                Running test commands...               " << std::endl;
    std::cout << "=================== Test Commands 1 ===================" << std::endl;
    lexer::Lexer lex("myvar_cool_var2_1 = 	15 + 3 * (3 - 4)\n   \nx=3 + \t 3");
    streamTokens(lex);
    std::cout << "=================== Test Commands 2 ===================" << std::endl;
    lexer::Lexer lex2(
R"(def myFunc():
    print("hello")
    if True:
        print("inner")
        continue
    else:
        print("else")
        break
        def hello():
            print("hello")

myFunc()
print("done"))"
);
        
    streamTokens(lex2);

    std::cout << "=================== Test Commands 3 ===================" << std::endl;
    lexer::Lexer lex3(
R"(while i <= 10:
    if (i * 2) + 5 / 2 == 8 - 1:
        print(i)
    else: 
        print("odd")
        i = i +   1

print("done")
)");
        
    streamTokens(lex3);

    return 0;
}
