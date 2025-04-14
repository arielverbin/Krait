#include "Lexer.hpp"
#include <cctype>
#include "exceptions/exceptions.hpp"
using namespace lexer;

const std::unordered_map<std::string, TokenType> Lexer::keywords_ = {
    {"def", TokenType::DEF},
    {"return", TokenType::RETURN},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"pass", TokenType::PASS},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"print", TokenType::PRINT},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT},
    {"True", TokenType::TRUE},
    {"False", TokenType::FALSE},
    {"None", TokenType::NONE},
};

const std::unordered_map<char, Lexer::TokenHandler> Lexer::tokenHandlers_ = {
    {'\n', [](Lexer& l) { return l.handleNewlineAndIndent(); }},
    {'(', [](Lexer& l) { return l.makeToken(TokenType::LPAREN, "("); }},
    {')', [](Lexer& l) { return l.makeToken(TokenType::RPAREN, ")"); }},
    {'+', [](Lexer& l) { return l.makeToken(TokenType::PLUS, "+"); }},
    {'-', [](Lexer& l) { return l.makeToken(TokenType::MINUS, "-"); }},
    {'*', [](Lexer& l) { return l.makeToken(TokenType::STAR, "*"); }},
    {'/', [](Lexer& l) { return l.makeToken(TokenType::SLASH, "/"); }},
    {':', [](Lexer& l) { return l.makeToken(TokenType::COLON, ":"); }},
    {'=', [](Lexer& l) { return l.match('=') ? l.makeToken(TokenType::EQ, "==") : l.makeToken(TokenType::ASSIGN, "="); }},
    {'!', [](Lexer& l) { return l.match('=') ? l.makeToken(TokenType::NEQ, "!=") : l.makeToken(TokenType::END_OF_FILE, "!"); }},
    {'<', [](Lexer& l) { return l.match('=') ? l.makeToken(TokenType::LTE, "<=") : l.makeToken(TokenType::LT, "<"); }},
    {'>', [](Lexer& l) { return l.match('=') ? l.makeToken(TokenType::GTE, ">=") : l.makeToken(TokenType::GT, ">"); }},
    {'"', [](Lexer& l) { return l.string(); }},
    {'\'', [](Lexer& l) { return l.string(); }},
    {',', [](Lexer& l) { return l.makeToken(TokenType::COMMA, ","); }},
    {'.', [](Lexer& l) { return l.makeToken(TokenType::DOT, "."); }},
};

Lexer::Lexer(const std::string& source) : source_(source) {}


Token Lexer::peek() {
    return next();  // for now, no lookahead
}

Token Lexer::next() {
    if (!m_bufferedTokens.empty()) {
        Token t = m_bufferedTokens.back();
        m_bufferedTokens.pop_back();
        return t;
    }

    skipWhitespace();

    if (isAtEnd()) {
        return makeToken(TokenType::END_OF_FILE, "");
    }

    char c = nextChar();

    auto it = tokenHandlers_.find(c);
    if (it != tokenHandlers_.end()) {
        return it->second(*this);
    }

    if (std::isdigit(c)) {
        lexerPosition_.prevChar();
        return number();
    }

    if (std::isalpha(c) || c == '_') {
        lexerPosition_.prevChar();
        return identifier();
    }

    throw except::LexicalError("Unexpected character (" + std::string(1, c) + ")",
        lexerPosition_.line, lexerPosition_.column);
}


bool Lexer::isAtEnd() const {
    return lexerPosition_.position >= source_.size();
}

char Lexer::peekChar() const {
    return isAtEnd() ? '\0' : source_[lexerPosition_.position];
}

char Lexer::nextChar() {
    if (isAtEnd()) return '\0';
    char c = source_[lexerPosition_.position];
    lexerPosition_.nextChar(c);
    return c;
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[lexerPosition_.position] != expected) return false;
    lexerPosition_.nextChar(expected);
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peekChar();
        if (c == ' ' || c == '\t' || c == '\r') {
            nextChar();
        } else {
            break;
        }
    }
}

Token Lexer::handleNewlineAndIndent() {
    int indent = 0;
    while (!isAtEnd()) {
        char ch = peekChar();
        if (ch == ' ') { nextChar(); indent++; }
        else if (ch == '\n') { nextChar(); indent = 0; }
        else if (std::isspace(ch)) {
            throw except::LexicalError("Unexpected whitespace (only spaces are allowed)",
                lexerPosition_.line, lexerPosition_.column);
        } else {
            break;
        };
    }

    if (indent % 4 != 0) {
        throw except::LexicalError("Indentation must be a multiple of 4 spaces",
            lexerPosition_.line, lexerPosition_.column);
    }

    if (indent > m_indentStack.back()) {
        m_indentStack.push_back(indent);
        m_bufferedTokens.push_back(makeToken(TokenType::INDENT, ""));
    } else {
        while (indent < m_indentStack.back()) {
            m_indentStack.pop_back();
            m_bufferedTokens.push_back(makeToken(TokenType::DEDENT, ""));
        }
    }

    return makeToken(TokenType::NEWLINE, "\\n");
}


Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token{type, value, lexerPosition_.line, lexerPosition_.column};
}

Token Lexer::identifier() {
    size_t start = lexerPosition_.position;
    while (std::isalnum(peekChar()) || peekChar() == '_') nextChar();
    std::string text = source_.substr(start, lexerPosition_.position - start);

    // try to resolve it as a keyword
    auto it = keywords_.find(text);
    if (it != keywords_.end()) {
        return makeToken(it->second, text);
    }
    // fallback - its a normal identifier
    return makeToken(TokenType::IDENTIFIER, text);
}

Token Lexer::number() {
    size_t start = lexerPosition_.position;
    while (std::isdigit(peekChar())) nextChar();
    std::string digits = source_.substr(start, lexerPosition_.position - start);
    return makeToken(TokenType::INT, digits);
}

Token Lexer::string() {
    char quote = source_[lexerPosition_.position - 1];  // ' or "
    std::string result;
    while (!isAtEnd() && peekChar() != quote) {
        char c = nextChar();
        if (c == '\\') {  // Handle escape characters
            char next = nextChar();
            switch (next) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case '\\': result += '\\'; break;
                case '"': result += '"'; break;
                case '\'': result += '\''; break;
                default: result += next; break;
            }
        } else {
            result += c;
        }
    }

    if (!isAtEnd()) nextChar();  // consume closing quote
    return makeToken(TokenType::STRING, result);
}

