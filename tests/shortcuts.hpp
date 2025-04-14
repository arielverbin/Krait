#ifndef TESTS_SHORTCUTS_HPP
#define TESTS_SHORTCUTS_HPP

#include <vector>
#include "semantics/ASTNode.hpp"
#include "lexer/Lexer.hpp"

#define p(OBJECT) std::make_shared<OBJECT>

template<typename... T>
std::vector<std::shared_ptr<semantics::ASTNode>> make_ast_vector(T&&... args) {
    return { std::forward<T>(args)... };
}

#define ASSIGNVAR(name, val)            p(Assign)(p(Variable)(name), val)
#define INT(n)                          p(Const)(p(Integer)(n))
#define STR(s)                          p(Const)(p(String)(s))
#define BOOL(b)                         p(Const)(Boolean::get(b))
#define VAR(name)                       p(Variable)(name)
#define VAR_SET(name, val)              p(Assign)(p(Variable)(name), val)
#define PRINT(expr)                     p(Print)(expr)
#define IF(cond, t, f)                  p(If)(cond, t, f)
#define WHILE(cond, body)               p(While)(cond, body)
#define PASS()                          p(Pass)()
#define CODE(...)                       p(Code)(make_ast_vector(__VA_ARGS__))
#define NOT(expr)                       p(UnaryOp)(UnaryOpType::Not, expr)
#define GEQ(lhs, rhs)                   p(BinaryOp)(BinaryOpType::GreaterEqual, lhs, rhs)
#define EQ(lhs, rhs)                    p(BinaryOp)(BinaryOpType::Equal, lhs, rhs)   
#define ADD(a, b)                       p(BinaryOp)(BinaryOpType::Sum, a, b)
#define SUB(a, b)                       p(BinaryOp)(BinaryOpType::Sub, a, b)
#define MULT(a, b)                      p(BinaryOp)(BinaryOpType::Mult, a, b)
#define FUNC(funcName, params, body)    p(semantics::Function)(funcName, params, body)
#define STRARR(...)                     std::vector<std::string>{__VA_ARGS__}
#define CALL(func, args)                p(Call)(func, args)
#define ARGS(...)                       make_ast_vector(__VA_ARGS__)
#define RETURN(expr)                    p(Return)(expr)
#define BREAK()                         p(Break)()
#define CONTINUE()                      p(Continue)()

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
        std::cout << "Token(" << tokenTypeToString(t.type).c_str() << ": " << t.value << ")\n";
        if (t.type == lexer::TokenType::END_OF_FILE) {
            break;
        }
    }
}

#endif // TESTS_SHORTCUTS_HPP
