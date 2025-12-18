#include "run/FileSource.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <sstream>
using namespace run;

FileSource::FileSource(const std::string& filePath) : file_(filePath.c_str()), eof_(false) {
    if (!file_) {
        std::cerr << "Error: Could not open file " << filePath << "\n";
    }
}

void FileSource::readScript() {
    if (file_) {
        std::stringstream buffer;
        buffer << file_.rdbuf();
        script_ = buffer.str();

        if (!script_.empty() && script_.back() != '\n') {
            script_ += '\n';
        }

    } else {
        script_ = "\n";
    }
}

std::vector<lexer::Token> FileSource::nextStatement() {
    eof_ = true;
    if (!script_.empty()) return lexer_.tokenize(script_);

    readScript();
    return lexer_.tokenize(script_);
}

const std::string& FileSource::source() {
    if (script_.empty()) readScript();

    return script_;
}

bool FileSource::eof() const {
    return eof_;
}
