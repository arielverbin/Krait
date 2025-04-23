#ifndef RUNNER_HPP
#define RUNNER_HPP

#include "run/InputSource.hpp"

#include "semantics/ASTNode.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "interpreter/Interpreter.hpp"
#include <fstream>
#include <iostream>

namespace run {

class Runner {
private:
    InputSource& source_;
    parser::Parser parser_;
    interpreter::Interpreter interpreter_;

public:
    Runner(InputSource& source);
    void run();
};

} // namespace run

#endif // RUNNER_HPP