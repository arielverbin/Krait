#include "Runner.hpp"
#include "run/ErrorPrinter.hpp"
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"

namespace run {

Runner::Runner(InputSource& source) : source_(source), parser_(), interpreter_() {}

void Runner::run() {
    do {
        try {
            auto tokens = source_.nextStatement();
            auto ast = parser_.parse(tokens);
            interpreter_.interpret(ast);
        
        // handle exceptions
        } catch (const except::SyntaxError& err) {
            std::cerr << ErrorPrinter::format(source_.source(), err);
            if (source_.exitOnError()) break;
        } catch (const except::RuntimeError& err) {
            std::cerr << ErrorPrinter::format(source_.source(), err);
            if (source_.exitOnError()) break;
        } catch (const except::KraitException& err) {
            std::cerr << ErrorPrinter::format(source_.source(), err);
        }

    } while (!source_.eof());
}
    
}
