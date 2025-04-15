#ifndef TESTS_UTILS_HPP
#define TESTS_UTILS_HPP

#include <vector>
#include "semantics/ASTNode.hpp"
#include "lexer/Lexer.hpp"

std::string tokenTypeToString(lexer::TokenType type) {
    switch (type) {
        case lexer::TokenType::INT: return "NUMBER";
        case lexer::TokenType::STRING: return "STRING";
        case lexer::TokenType::IDENTIFIER: return "IDENTIFIER";
        case lexer::TokenType::ASSIGN: return "ASSIGN";
        case lexer::TokenType::EQ: return "EQUAL";
        case lexer::TokenType::NEQ: return "NOTEQUAL";
        case lexer::TokenType::LT: return "LESS";
        case lexer::TokenType::LTE: return "LESSEQUAL";
        case lexer::TokenType::GT: return "GREATER";
        case lexer::TokenType::GTE: return "GREATEREQUAL";
        case lexer::TokenType::AND: return "AND";
        case lexer::TokenType::OR: return "OR";
        case lexer::TokenType::NOT: return "NOT";
        case lexer::TokenType::COLON: return "COLON";
        case lexer::TokenType::NEWLINE: return "NEWLINE";
        case lexer::TokenType::INDENT: return "INDENT";
        case lexer::TokenType::DEDENT: return "DEDENT";
        case lexer::TokenType::LPAREN: return "LPAREN";
        case lexer::TokenType::RPAREN: return "RPAREN";
        case lexer::TokenType::END_OF_FILE: return "EOF";
        case lexer::TokenType::DEF: return "DEF";
        case lexer::TokenType::RETURN: return "RETURN";
        case lexer::TokenType::IF: return "IF";
        case lexer::TokenType::ELSE: return "ELSE";
        case lexer::TokenType::WHILE: return "WHILE";
        case lexer::TokenType::PRINT: return "PRINT";
        case lexer::TokenType::PASS: return "PASS";
        case lexer::TokenType::BREAK: return "BREAK";
        case lexer::TokenType::CONTINUE: return "CONTINUE";
        case lexer::TokenType::PLUS: return "PLUS";
        case lexer::TokenType::MINUS: return "MINUS";
        case lexer::TokenType::STAR: return "STAR";
        case lexer::TokenType::SLASH: return "SLASH";
        case lexer::TokenType::COMMA: return "COMMA";
        case lexer::TokenType::DOT: return "DOT";
        case lexer::TokenType::TRUE: return "TRUE";
        case lexer::TokenType::FALSE: return "FALSE";
        case lexer::TokenType::NONE: return "NONE";
        default: return "UNKNOWN";
    }
}

void streamTokens(lexer::Lexer& lex) {
    while (true) {
        lexer::Token t = lex.next();
        std::cout << "Token(" << tokenTypeToString(t.type()).c_str() << ": " << t.value() << ")\n";
        if (t.type() == lexer::TokenType::END_OF_FILE) {
            break;
        }
    }
}

#endif // TESTS_UTILS_HPP
