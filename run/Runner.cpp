#include "Runner.hpp"

namespace run {

Runner::Runner(InputSource& source) : source_(source), lexer_(), parser_(), interpreter_() {}

void Runner::run() {
    while(!source_.eof()) {
        std::string nextCode = source_.getNext();
        auto tokens = lexer_.tokenize(nextCode);
        auto ast = parser_.parse(tokens);
        interpreter_.interpret(ast);
    }
}
    
}
