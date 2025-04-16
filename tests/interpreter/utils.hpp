#ifdef KRAIT_TESTING
#ifndef TESTS_INTERPRETER_UTILS_HPP
#define TESTS_INTERPRETER_UTILS_HPP

#include <vector>
#include "semantics/ASTNode.hpp"
#include "lexer/Lexer.hpp"

#define ms(OBJECT) std::make_shared<OBJECT>

template<typename... T>
std::vector<std::shared_ptr<semantics::ASTNode>> make_ast_vector(T&&... args) {
    return { std::forward<T>(args)... };
}

#define ASSIGNVAR(name, val)            ms(Assign)(ms(Variable)(name), val)
#define INT(n)                          ms(Const)(ms(Integer)(n))
#define STR(s)                          ms(Const)(ms(String)(s))
#define BOOL(b)                         ms(Const)(Boolean::get(b))
#define VAR(name)                       ms(Variable)(name)
#define PRINT(expr)                     ms(Print)(expr)
#define IF(cond, t, f)                  ms(If)(cond, t, f)
#define WHILE(cond, body)               ms(While)(cond, body)
#define PASS()                          ms(Pass)()
#define CODE(...)                       ms(Code)(make_ast_vector(__VA_ARGS__))
#define NOT(expr)                       ms(UnaryOp)(UnaryOpType::Not, expr)
#define GEQ(lhs, rhs)                   ms(BinaryOp)(BinaryOpType::GreaterEqual, lhs, rhs)
#define LEQ(lhs, rhs)                   ms(BinaryOp)(BinaryOpType::LesserEqual, lhs, rhs)
#define LT(lhs, rhs)                    NOT(GEQ(lhs, rhs))
#define GT(lhs, rhs)                    NOT(LEQ(lhs, rhs))
#define EQ(lhs, rhs)                    ms(BinaryOp)(BinaryOpType::Equal, lhs, rhs)   
#define OR(lhs, rhs)                    ms(BinaryOp)(BinaryOpType::Or, lhs, rhs)
#define AND(lhs, rhs)                   ms(BinaryOp)(BinaryOpType::And, lhs, rhs)
#define NEG(expr)                       ms(UnaryOp)(UnaryOpType::Neg, expr)
#define ADD(a, b)                       ms(BinaryOp)(BinaryOpType::Sum, a, b)
#define SUB(a, b)                       ms(BinaryOp)(BinaryOpType::Sub, a, b)
#define MULT(a, b)                      ms(BinaryOp)(BinaryOpType::Mult, a, b)
#define DIV(a, b)                       ms(BinaryOp)(BinaryOpType::Div, a, b)
#define MOD(a, b)                       ms(BinaryOp)(BinaryOpType::Mod, a, b)
#define FUNC(funcName, params, body)    ms(semantics::Function)(funcName, params, body)
#define STRARR(...)                     std::vector<std::string>{__VA_ARGS__}
#define CALL(func, args)                ms(Call)(func, args)
#define ARGS(...)                       make_ast_vector(__VA_ARGS__)
#define RETURN(expr)                    ms(Return)(expr)
#define BREAK()                         ms(Break)()
#define CONTINUE()                      ms(Continue)()

#endif // TESTS_INTERPRETER_UTILS_HPP
#endif // KRAIT_TESTING
