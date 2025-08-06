#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <string>
#include <span>
#include "lexer/Token.hpp"
#include "exceptions/exceptions.hpp"
#include "semantics/ASTNode.hpp"
#include "semantics/operation_semantics/BinaryOp.hpp"
#include "semantics/operation_semantics/UnaryOp.hpp"
#include "parser/ParserContext.hpp"

namespace parser {

enum class Associativity { LEFT, RIGHT };

/**
 * The order of operator evaluation is primarily determined by their precedence.
 * If two operators have the same precedence, their associativity determines the
 * order in which they are evaluated. 
 */
struct OperatorInfo {
    int precedence;
    Associativity associativity;
};

class Parser {
public:
    Parser();

    // Top-level entry: parse all statements into a Code node
    std::shared_ptr<semantics::ASTNode> parse(std::vector<lexer::Token>& tokens);

    // Dispatch on keywords or fallback to expression
    std::shared_ptr<semantics::ASTNode> parseStatement();

private:
    // Parsing Rules - Precedence of operators and associativity
    std::unordered_map<lexer::TokenType, OperatorInfo> infixTable_;
    std::unordered_map<lexer::TokenType, int> prefixTable_;

    ParserContext context_;

    // Expression parsing (Pratt / precedence‑climbing)
    std::shared_ptr<semantics::ASTNode> parseExpression(int minBp = 0);
    std::shared_ptr<semantics::ASTNode> parsePrimary();
    std::shared_ptr<semantics::ASTNode> parseInfix(
        std::shared_ptr<semantics::ASTNode> left,
        const lexer::Token& op,
        int lbp
    );
    std::shared_ptr<semantics::ASTNode> parsePostfix(std::shared_ptr<semantics::ASTNode> left);

    // Prase statements starting with a keyword
    std::shared_ptr<semantics::ASTNode> parseWhile();
    std::shared_ptr<semantics::ASTNode> parseFunctionDef();
    std::shared_ptr<semantics::ASTNode> parseClassDef();
    std::shared_ptr<semantics::ASTNode> parsePrint();
    std::shared_ptr<semantics::ASTNode> parseIf();
    std::shared_ptr<semantics::ASTNode> parseReturn();
    std::shared_ptr<semantics::ASTNode> parseBreak();
    std::shared_ptr<semantics::ASTNode> parsePass();
    std::shared_ptr<semantics::ASTNode> parseContinue();

    // Token utilities
    bool isAtEnd() const;
    const lexer::Token& peek() const;
    const lexer::Token& previous() const;
    const lexer::Token& advance();
    bool check(const lexer::TokenType& type) const;
    bool match(const lexer::TokenType& type);
    void expect(const lexer::TokenType& type, const std::string& msg);

    // Binding power / precedence & associativity
    int getBinaryPrecedence(const lexer::TokenType& type) const;
    int getUnaryPrecedence(const lexer::TokenType& type) const;
    bool isRightAssociative(const lexer::Token& type) const;

    // Map between tokens and operator types
    semantics::BinaryOpType mapBinaryOp(const lexer::Token& type) const;
    semantics::UnaryOpType mapUnaryOp(const lexer::Token& type) const;
};

} // namespace parser

#endif // PARSER_HPP
