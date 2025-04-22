#ifndef REPL_SOURCE_HPP
#define REPL_SOURCE_HPP

#include "run/InputSource.hpp"

namespace run {

class REPLSource : public InputSource {
private:
    bool eof_;
    std::istream& inStream_;
    std::ostream& outStream_;

public:
    REPLSource();
    std::string getNext() override;
    bool eof() const override;
    virtual ~REPLSource() = default;
};

}

#endif // REPL_SOURCE_HPP