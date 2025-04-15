#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include "Token.hpp"
#include "exceptions/exceptions.hpp"
#include "semantics/ASTNode.hpp"

namespace parser {
class Parser {
    public:
        Parser(const std::vector<lexer::Token>& tokens)
            : tokens_(tokens), current_(0) {}
    
        std::unique_ptr<semantics::ASTNode> parse(); // entry point
    
    private:
        const std::vector<lexer::Token>& tokens_;
        size_t current_;
    
        std::unique_ptr<semantics::ASTNode> parseStatement(); // entry point for online interpreter
        std::unique_ptr<semantics::ASTNode> parseExpression();
        std::unique_ptr<semantics::ASTNode> parseBlock();
    
        // Specific statements
        std::unique_ptr<semantics::ASTNode> parseIf();
        std::unique_ptr<semantics::ASTNode> parseWhile();
        std::unique_ptr<semantics::ASTNode> parseFunctionDef();
        std::unique_ptr<semantics::ASTNode> parseAssignment();
        std::unique_ptr<semantics::ASTNode> parsePrint();
    
        // Expression components
        std::unique_ptr<semantics::ASTNode> parseEquality();
        std::unique_ptr<semantics::ASTNode> parseComparison();
        std::unique_ptr<semantics::ASTNode> parseTerm();      // +, -
        std::unique_ptr<semantics::ASTNode> parseFactor();    // *, /
        std::unique_ptr<semantics::ASTNode> parsePrimary();   // literals, identifiers, grouping
    
        // Helpers
        const lexer::Token& advance();
        const lexer::Token& peek() const;
        bool match(lexer::TokenType type);
        bool check(lexer::TokenType type) const;
        void expect(lexer::TokenType type, const std::string& msg);
    };

} // namespace parser
#endif // PARSER_HPP
    