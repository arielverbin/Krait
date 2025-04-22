#ifndef FILE_SOURCE_HPP
#define FILE_SOURCE_HPP

#include <fstream>
#include "run/InputSource.hpp"

namespace run {

class FileSource : public InputSource {
private:
    std::ifstream file_;
    bool eof_;

public:
    FileSource(const std::string& filePath);
    std::string getNext() override;
    bool eof() const override;

    ~FileSource() = default;
};

}

#endif // FILE_SOURCE_HPP