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
        position++;
        if (c == '\n') {
            lineLengths.push(column);
            line++; column = 1;
        } else { column++; }
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

    void reset() {
        line = column = 1;
        position = 0;
        lineLengths = {};
    }

    void append() {
        position = 0;
    }
};

#endif // LEXER_POSITION_HPP