#ifndef LEXER_POSITION_HPP
#define LEXER_POSITION_HPP

#include <stack>
#include <cstddef>

class LexerPosition {
public:
    LexerPosition() : line(1), column(1), position(0) {}

    size_t line = 1;
    size_t column = 1;
    size_t position = 0;
    std::stack<size_t> lineLengths;

    void forwards(char c) {
        column++;
        position++;
        if (c == '\n') {
            lineLengths.push(column);
            line++;
            column = 1;
        }
    }

    void backwards() {
        if (position == 0) return;
        position--;
        column--;

        if (column == 0) {
            if (lineLengths.empty()) return;
            line--;
            column = lineLengths.top();
            lineLengths.pop();
        }
    }
};

#endif // LEXER_POSITION_HPP