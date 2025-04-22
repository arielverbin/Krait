#include <iostream>
#include "run/Runner.hpp"
#include "run/FileSource.hpp"
#include "run/REPLSource.hpp"

// void printSyntaxError(const except::SyntaxError& e, const std::string& source) {
//     int lineNumber = e.line();
//     int column     = e.column();
    
//     // Split source into lines
//     std::istringstream stream(source);
//     std::string line;
//     int currentLine = 1;
    
//     while (std::getline(stream, line)) {
//         if (currentLine == lineNumber) {
//             break;
//         }
//         ++currentLine;
//     }

//     // Print the error header
//     std::cerr << "SyntaxError at line " << lineNumber << ": " << e.what() << "\n";
    
//     // Print the line with the error
//     std::cerr << lineNumber << " | " << line << "\n";

//     // Print the caret (^) under the right column
//     std::cerr << std::string(std::to_string(lineNumber).size(), ' ') << " | "
//               << std::string(column - 1, ' ') << "^\n";
// }

int main(int argc, char* argv[]) {
    std::unique_ptr<run::InputSource> source;
    
    if (argc < 2) {
        source = std::make_unique<run::REPLSource>();
    } else {
        source = std::make_unique<run::FileSource>(argv[1]);
    }
    
    run::Runner runner(*source);
    runner.run();
}