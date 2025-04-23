#ifndef REPL_SOURCE_HPP
#define REPL_SOURCE_HPP

#include "run/repl/TokenStreamAnalyzer.hpp"
#include "run/InputSource.hpp"

namespace run {

class REPLSource : public InputSource {
private:
    TokenStreamAnalyzer analyzer_;
    bool eof_;
    std::string currentSource_;
    std::string getLine(bool notComplete = false);

public:
    REPLSource();
    std::vector<lexer::Token> nextStatement() override;
    bool eof() const override;
    bool exitOnError() override;
    const std::string& source() override;
    virtual ~REPLSource() = default;
};

}

#endif // REPL_SOURCE_HPP