#include <iostream>
#include "tests/interpreter/commands.hpp"
#include "interpreter/Interpreter.hpp"

int main() {
    interpreter::Interpreter i(recursionTest);
    i.interpret();
}