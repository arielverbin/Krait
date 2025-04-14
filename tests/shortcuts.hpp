#ifndef TESTS_SHORTCUTS_HPP
#define TESTS_SHORTCUTS_HPP

#include <vector>
#include "semantics/ASTNode.hpp"

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

#endif // TESTS_SHORTCUTS_HPP
