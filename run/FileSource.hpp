#ifndef FILE_SOURCE_HPP
#define FILE_SOURCE_HPP

#include <fstream>
#include "run/InputSource.hpp"

namespace run {

class FileSource : public InputSource {
private:
    std::ifstream file_;
    std::string script_;
    bool eof_;
    void readScript();

public:
    FileSource(const std::string& filePath);
    std::vector<lexer::Token> nextStatement() override;
    bool eof() const override;
    const std::string& source() override;
    bool exitOnError() override { return true; }

    ~FileSource() = default;
};

}

#endif // FILE_SOURCE_HPP