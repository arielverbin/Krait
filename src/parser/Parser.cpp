#include "Parser.hpp"
#include "exceptions/exceptions.hpp"

#include "semantics/Const.hpp"
#include "semantics/Variable.hpp"
#include "semantics/Print.hpp"

#include "semantics/flow_semantics/If.hpp"
#include "semantics/flow_semantics/While.hpp"
#include "semantics/flow_semantics/Code.hpp"
#include "semantics/flow_semantics/Pass.hpp"
#include "semantics/flow_semantics/Call.hpp"

#include "semantics/define_semantics/Function.hpp"
#include "semantics/define_semantics/Assign.hpp"

#include "semantics/signal_semantics/Break.hpp"
#include "semantics/signal_semantics/Return.hpp"
#include "semantics/signal_semantics/Continue.hpp"

#include "semantics/operation_semantics/UnaryOp.hpp"
#include "semantics/operation_semantics/BinaryOp.hpp"
using namespace parser;

std::uniqe_ptr<semantics::ASTNode> Parser::parse() {
    std::vector<std::uniqe_ptr<semantics::ASTNode>> statements;
    while (!check(TokenType::END_OF_FILE)) {
        statements.push_back(parseStatement());
    }
    return std::make_unique<semantics::Code>(std::move(statements));
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (match(TokenType::IF)) return parseIf();
    if (match(TokenType::WHILE)) return parseWhile();
    if (match(TokenType::DEF)) return parseFunctionDef();
    if (match(TokenType::PRINT)) return parsePrint();
    if (check(TokenType::IDENTIFIER) && m_tokens[m_current + 1].type == TokenType::ASSIGN)
        return parseAssignment();
    throw except::SyntaxError("Unexpected statement", tokens_[current_].line(), tokens_[current_].column());
}

std::unique_ptr<ASTNode> Parser::parseIf() {
    auto condition = parseExpression();
    expect(TokenType::COLON, "Expected ':' after if condition");
    expect(TokenType::NEWLINE, "Expected newline after ':'");
    expect(TokenType::INDENT, "Expected indent after newline");

    std::vector<std::unique_ptr<ASTNode>> thenBranch;
    while (!match(TokenType::DEDENT) && !check(TokenType::END_OF_FILE)) {
        thenBranch.push_back(parseStatement());
    }

    std::vector<std::unique_ptr<ASTNode>> elseBranch;
    if (match(TokenType::ELSE)) {
        expect(TokenType::COLON, "Expected ':' after else");
        expect(TokenType::NEWLINE, "Expected newline after ':'");
        expect(TokenType::INDENT, "Expected indent after newline");

        while (!match(TokenType::DEDENT) && !check(TokenType::END_OF_FILE)) {
            elseBranch.push_back(parseStatement());
        }
    }

    return std::make_unique<semantics::If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}