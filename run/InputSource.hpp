#ifndef INPUT_SOURCE_HPP
#define INPUT_SOURCE_HPP

#include <string>

namespace run {

class InputSource {
public:
    virtual std::string getNext() = 0;
    virtual bool eof() const = 0;
    virtual ~InputSource() = default;
};

}

#endif // INPUT_SOURCE_HPP