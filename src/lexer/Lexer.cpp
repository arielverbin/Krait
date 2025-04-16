#include "Lexer.hpp"
#include <cctype>
#include <iostream>
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
    {"True", TokenType::TRU},
    {"False", TokenType::FALS},
    {"None", TokenType::NONE},
};

Lexer::Lexer(const std::string& source) : source_(source) {}


Token Lexer::peek() {
    return next();  // for now, no lookahead
}

void Lexer::init() {
    lexerPosition_ = LexerPosition();
    pendingTokens_.clear();
    indentLevel_ = 0;
    groupingLevel_ = 0;

    if(std::isspace(peekChar())) {
        handleNewlineAndIndent();
    }
}

Token Lexer::next() {
    if (!pendingTokens_.empty()) {
        return emitPending();
    }

    if (isAtEnd()) {
        emitCleanups();        
        return emitPending();
    }

    // ignore newlines if we are inside parentheses
    skipWhitespace(groupingLevel_ != 0);

    char c = nextChar();

    if (std::isdigit(c)) {
        lexerPosition_.backwards();
        return number();
    }

    if (std::isalpha(c) || c == '_') {
        lexerPosition_.backwards();
        return identifier();
    }

    if (c == '\'' || c == '"') {
        return string();
    }

    return handleOperatorOrPunct(c);
}

Token Lexer::handleOperatorOrPunct(char c) {
    LexerPosition& pos = lexerPosition_;

    switch (c) {
        case '+': return Token(TokenType::PLUS, "+", pos);
        case '-': return Token(TokenType::MINUS, "-", pos);
        case '*': return Token(TokenType::STAR, "*", pos);
        case '/': return Token(TokenType::SLASH, "/", pos);

        case '(': groupingLevel_++; return Token(TokenType::LPAREN, "(", pos);
        case ')': groupingLevel_--; return Token(TokenType::RPAREN, ")", pos);
        case ':': return Token(TokenType::COLON, ":", pos);

        case '=': return match('=') ? Token(TokenType::EQ, "==", pos) : Token(TokenType::ASSIGN, "=", pos);
        case '!': return match('=') ? Token(TokenType::NEQ, "!=", pos) : Token(TokenType::NOT, "!", pos);
        case '<': return match('=') ? Token(TokenType::LTE, "<=", pos) : Token(TokenType::LT, "<", pos);
        case '>': return match('=') ? Token(TokenType::GTE, ">=", pos) : Token(TokenType::GT, ">", pos);

        case ',': return Token(TokenType::COMMA, ",", pos);
        case '.': return Token(TokenType::DOT, ".", pos);

        case '"': return string();
        case '\'': return string();

        case '\n': return handleNewlineAndIndent();
        default: 
            throw except::LexicalError("Unexpected character (" + std::string(1, c) + ")",
                lexerPosition_.line, lexerPosition_.column);
    }
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
    lexerPosition_.forwards(c);
    return c;
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[lexerPosition_.position] != expected) return false;
    lexerPosition_.forwards(expected);
    return true;
}

void Lexer::skipWhitespace(bool skipNewLine) {
    while (!isAtEnd()) {
        char c = peekChar();
        if (c == ' ' || c == '\t' || c == '\r' || (skipNewLine && c == '\n')) {
            nextChar();
        } else {
            break;
        }
    }
}

Token Lexer::handleNewlineAndIndent() {
    size_t indent = 0;
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

    indent = indent / 4;

    if (indent > indentLevel_) {
        for (size_t i = indentLevel_; i < indent; ++i) {
            pendingTokens_.push_back(Token(TokenType::INDENT, "", lexerPosition_));
        }
    } else {
        for (size_t i = indentLevel_; i > indent; --i) {
            pendingTokens_.push_back(Token(TokenType::DEDENT, "", lexerPosition_));
        }
    }
    indentLevel_ = indent;

    return Token(TokenType::NEWLINE, "\\n", lexerPosition_);
}

Token Lexer::identifier() {
    size_t start = lexerPosition_.position;
    while (std::isalnum(peekChar()) || peekChar() == '_') nextChar();
    std::string text = source_.substr(start, lexerPosition_.position - start);

    // try to resolve it as a keyword
    auto it = keywords_.find(text);
    if (it != keywords_.end()) {
        return Token(it->second, text, lexerPosition_);
    }
    // fallback - its a normal identifier
    return Token(TokenType::IDENTIFIER, text, lexerPosition_);
}

Token Lexer::number() {
    size_t start = lexerPosition_.position;
    while (std::isdigit(peekChar())) nextChar();
    std::string digits = source_.substr(start, lexerPosition_.position - start);
    return Token(TokenType::INT, digits, lexerPosition_);
}

Token Lexer::string() {
    char quote = source_[lexerPosition_.position - 1];  // opening quote
    std::string result;
    size_t startingLine = lexerPosition_.line;

    while (true) {
        if (isAtEnd()) {
            throw except::LexicalError("Unterminated string literal", startingLine, lexerPosition_.column);
        }

        char c = nextChar();

        if (c == '\n') {
            throw except::LexicalError("Unterminated string literal before newline", startingLine, lexerPosition_.column);
        }

        if (c == quote) {
            LexerPosition loc = lexerPosition_;
            // Check if there's another string following
            skipWhitespace(true);
            if (!isAtEnd() && (peekChar() == '"' || peekChar() == '\'')) {
                // Start new string continuation
                quote = nextChar(); // consume opening quote
                continue;
            } else {
                // Restore previous location.
                lexerPosition_ = loc;
                break; // string ended normally
            }
        }

        if (c == '\\') {
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

    return Token(TokenType::STRING, result, lexerPosition_);
}

std::vector<Token> Lexer::tokenize(std::string source) {
    Lexer lexer(source);
    lexer.init();
    std::vector<Token> tokens;
    do {
        tokens.push_back(lexer.next());
    } while (tokens.back().type() != TokenType::END_OF_FILE);
    return tokens;
}

void Lexer::emitCleanups() {
    // For indentation
    for (size_t i = 0; i < indentLevel_; ++i) {
        pendingTokens_.push_back(Token(TokenType::DEDENT, "", lexerPosition_));
    }

    pendingTokens_.push_back(Token(TokenType::END_OF_FILE, "", lexerPosition_));
}

Token Lexer::emitPending() {
    if (pendingTokens_.empty()) {
        return Token(TokenType::END_OF_FILE, "", lexerPosition_);
    }
    Token t = pendingTokens_.back();
    pendingTokens_.pop_back();
    return t;
}
