#ifndef INPUT_SOURCE_HPP
#define INPUT_SOURCE_HPP

#include <string>
#include "lexer/Token.hpp"
#include "lexer/Lexer.hpp"

namespace run {

class InputSource {
public:
    InputSource() { lexer_.reset(); }
    virtual std::vector<lexer::Token> nextStatement() = 0;
    virtual bool eof() const = 0;
    virtual bool exitOnError() const = 0;
    virtual const std::string& source() = 0;
    virtual ~InputSource() = default;

protected:
    lexer::Lexer lexer_;
};

}

#endif // INPUT_SOURCE_HPP
