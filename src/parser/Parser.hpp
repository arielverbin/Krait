#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include "lexer/Token.hpp"
#include "exceptions/exceptions.hpp"
#include "semantics/ASTNode.hpp"
#include "semantics/operation_semantics/BinaryOp.hpp"

namespace parser {
class Parser {
    public:
        Parser(const std::vector<lexer::Token>& tokens)
            : tokens_(tokens), current_(0) {}
    
        std::shared_ptr<semantics::ASTNode> parse(); // entry point
    
    private:
        const std::vector<lexer::Token>& tokens_;
        size_t current_;
    
        std::shared_ptr<semantics::ASTNode> parseStatement(); // entry point for online interpreter
        std::shared_ptr<semantics::ASTNode> parseExpression();
        std::shared_ptr<semantics::ASTNode> parseExpression(int minPrecedence);
        std::shared_ptr<semantics::ASTNode> parseExprStatement();
        std::shared_ptr<semantics::ASTNode> parseBlock();
    
        // Specific statements
        std::shared_ptr<semantics::ASTNode> parseIf();
        std::shared_ptr<semantics::ASTNode> parseWhile();
        std::shared_ptr<semantics::ASTNode> parseFunctionDef();
        std::shared_ptr<semantics::ASTNode> parseAssignment();
        std::shared_ptr<semantics::ASTNode> parsePrint();
    
        // Expression components
        std::shared_ptr<semantics::ASTNode> parseEquality();
        std::shared_ptr<semantics::ASTNode> parseComparison();
        std::shared_ptr<semantics::ASTNode> parseTerm();      // +, -
        std::shared_ptr<semantics::ASTNode> parseFactor();    // *, /
        std::shared_ptr<semantics::ASTNode> parsePrimary();   // literals, identifiers, grouping
        std::shared_ptr<semantics::ASTNode> parseUnary();
    
        // Helpers
        bool isAtEnd() const;
        const lexer::Token& advance();
        const lexer::Token& peek() const;
        const lexer::Token& previous() const;

        bool match(lexer::TokenType type);
        bool check(lexer::TokenType type) const;
        void expect(lexer::TokenType type, const std::string& msg);
        int getPrecedence(lexer::TokenType type) const;
        semantics::BinaryOpType mapBinaryOp(lexer::TokenType type) const;
    };

} // namespace parser
#endif // PARSER_HPP
    