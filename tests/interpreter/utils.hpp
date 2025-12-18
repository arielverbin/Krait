#ifdef KRAIT_TESTING
#ifndef TESTS_INTERPRETER_UTILS_HPP
#define TESTS_INTERPRETER_UTILS_HPP

#include <vector>

#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"

#include "semantics/define_semantics/Assign.hpp"
#include "semantics/define_semantics/ClassDef.hpp"
#include "semantics/define_semantics/FunctionDef.hpp"
#include "semantics/flow_semantics/Call.hpp"
#include "semantics/Print.hpp"
#include "semantics/Const.hpp"
#include "semantics/Variable.hpp"
#include "semantics/operation_semantics/AccessProperty.hpp"
#include "semantics/operation_semantics/BinaryOp.hpp"
#include "semantics/operation_semantics/UnaryOp.hpp"
#include "semantics/flow_semantics/If.hpp"
#include "semantics/flow_semantics/While.hpp"
#include "semantics/flow_semantics/Pass.hpp"
#include "semantics/flow_semantics/Code.hpp"
#include "semantics/signal_semantics/Return.hpp"
#include "semantics/signal_semantics/Break.hpp"
#include "semantics/signal_semantics/Continue.hpp"

#include "semantics/ASTNode.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "lexer/Lexer.hpp"

#define ms(OBJECT) std::make_shared<OBJECT>

template<typename... T>
std::vector<std::shared_ptr<semantics::ASTNode>> make_ast_vector(T&&... args) {
    return { std::forward<T>(args)... };
}

#define ASSIGNVAR(name, val)            ms(Assign)(ms(Variable)(name), val)
#define ASSIGN(assignable, val)         ms(Assign)(assignable, val)
#define INT(n)                          ms(Const)(gc::make_guarded<Integer>(n))
#define STR(s)                          ms(Const)(gc::make_guarded<String>(s))
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
#define LT(lhs, rhs)                    ms(BinaryOp)(BinaryOpType::LesserThan, lhs, rhs)
#define GT(lhs, rhs)                    ms(BinaryOp)(BinaryOpType::GreaterThan, lhs, rhs)
#define EQ(lhs, rhs)                    ms(BinaryOp)(BinaryOpType::Equal, lhs, rhs)   
#define OR(lhs, rhs)                    ms(BinaryOp)(BinaryOpType::Or, lhs, rhs)
#define AND(lhs, rhs)                   ms(BinaryOp)(BinaryOpType::And, lhs, rhs)
#define NEG(expr)                       ms(UnaryOp)(UnaryOpType::Neg, expr)
#define ADD(a, b)                       ms(BinaryOp)(BinaryOpType::Sum, a, b)
#define SUB(a, b)                       ms(BinaryOp)(BinaryOpType::Sub, a, b)
#define MULT(a, b)                      ms(BinaryOp)(BinaryOpType::Mult, a, b)
#define DIV(a, b)                       ms(BinaryOp)(BinaryOpType::Div, a, b)
#define MOD(a, b)                       ms(BinaryOp)(BinaryOpType::Mod, a, b)
#define FUNC(funcName, params, body)    ms(semantics::FunctionDef)(funcName, params, body)
#define STRARR(...)                     std::vector<std::string>{__VA_ARGS__}
#define CALL(func, args)                ms(Call)(func, args)
#define ARGS(...)                       make_ast_vector(__VA_ARGS__)
#define RETURN(expr)                    ms(Return)(expr)
#define BREAK()                         ms(Break)()
#define CONTINUE()                      ms(Continue)()
#define CLASS(clsName, body)            ms(semantics::ClassDef)(clsName, body)
#define PROPERTY(a, attName)            ms(semantics::AccessProperty)(a, attName)

#endif // TESTS_INTERPRETER_UTILS_HPP
#endif // KRAIT_TESTING
