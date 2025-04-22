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

std::string FileSource::getNext() {
    eof_ = true;

    if (file_) {
        std::stringstream buffer;
        buffer << file_.rdbuf();
        return buffer.str();
    }

    return utils::empty;
}

bool FileSource::eof() const {
    return eof_;
}
