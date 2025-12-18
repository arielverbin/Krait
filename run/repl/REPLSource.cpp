#include "run/repl/REPLSource.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
using namespace run;

#define COLOR_PROMPT(color, text) "\001" color "\002" text "\001" RESET "\002"

REPLSource::REPLSource() : analyzer_(), eof_(false) {
    std::cout << BOLDMAGENTA << "🐍 Krait " 
    << RESET << "(v1.0.1" 
    #ifdef KRAIT_DEBUGGING
    << CYAN << " DEBUG" << RESET
    #endif // KRAIT_DEBUGGING
    << ")"
    << GREY << " / REPL session started "
    << RESET << std::endl;
}

std::string REPLSource::getLine(bool notComplete) {
    char* input = notComplete ? readline(COLOR_PROMPT(CYAN, "... ")) : readline(COLOR_PROMPT(BLUE, ">>> "));

    if (!input) return "\n";
    if (*input) add_history(input);

    std::string line(input);
    free(input);

    if (line == "exit") {
        eof_ = true;
        std::cout << BOLDMAGENTA << "GOODBYE " << RESET 
        << GREY << "/ REPL session ended "
        << RESET << std::endl;
        line = "";
    }

    return line + "\n";
}

std::vector<lexer::Token> REPLSource::nextStatement() {

    currentSource_ = getLine();
    bool doCleanup = false;

    auto tokens = lexer_.tokenize(currentSource_, doCleanup);
    analyzer_.feed(tokens);

    while (!analyzer_.statementComplete()) {
        std::string line = getLine(/*notComplete=*/true);
        currentSource_ += line;
        doCleanup = (line == "\n");

        auto tokens = lexer_.tokenize(line, doCleanup);
        analyzer_.feed(tokens);
    }

    lexer_.reset();
    return analyzer_.consume();
}

bool REPLSource::exitOnError() {
    analyzer_.consume();
    lexer_.reset();
    return false;
}

const std::string& REPLSource::source() {
    return currentSource_;
}

bool REPLSource::eof() const {
    return eof_;
}

REPLSource::~REPLSource() {
    clear_history();
}