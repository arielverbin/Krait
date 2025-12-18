#ifndef ERROR_PRINTER_HPP
#define ERROR_PRINTER_HPP

#include "exceptions/exceptions.hpp"
#include "utils/utils.hpp"
#include <sstream>


class ErrorPrinter {
public:
    static std::string format(const std::string& source, const except::SyntaxError& err) {
        int lineNumber = err.line();
        int column = err.column();
        
        // Split source into lines
        std::istringstream stream(source);
        std::string line;
        int currentLine = 1;
        
        while (std::getline(stream, line)) {
            if (currentLine == lineNumber) {
                break;
            }
            ++currentLine;
        }

        std::ostringstream error;
        
        // Print the error header
        error << GREY << "SyntaxError at line " << lineNumber << ": " << err.what() << "\n";
        
        // Print the line with the error
        error << lineNumber << " | " << line << "\n";

        // Print the caret (^) under the right column
        error << std::string(std::to_string(lineNumber).size(), ' ') << " | "
                << std::string(column - 1, ' ') << "^" << RESET << "\n";

        return error.str();
    }

    static std::string format(const std::string& source, const except::KraitException& err) {
        UNREFERENCED(source);

        std::ostringstream error;
        
        // Print the error header
        /** TODO: Implement backtrace */
        error << GREY << err.what() << RESET << "\n";

        return error.str();
    }
};

#endif // ERROR_PRINTER_HPP
