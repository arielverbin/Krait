#include <iostream>
#include <vector>

#include "commands.hpp"
#include "interpreter/Interpreter.hpp"

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
    
    return 0;
}
