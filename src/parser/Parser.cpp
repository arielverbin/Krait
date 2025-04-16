#include "Parser.hpp"
#include "exceptions/exceptions.hpp"

#include "core/Integer.hpp"
#include "core/String.hpp"
#include "core/Boolean.hpp"
#include "core/None.hpp"

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

semantics::BinaryOpType Parser::mapBinaryOp(lexer::TokenType type) const {
    switch (type) {
        case lexer::TokenType::PLUS: return semantics::BinaryOpType::Sum;
        case lexer::TokenType::MINUS: return semantics::BinaryOpType::Sub;
        case lexer::TokenType::STAR: return semantics::BinaryOpType::Mult;
        case lexer::TokenType::SLASH: return semantics::BinaryOpType::Div;
        case lexer::TokenType::EQ: return semantics::BinaryOpType::Equal;
        //case lexer::TokenType::NEQ: return semantics::BinaryOpType::NotEqual;
        //case lexer::TokenType::LT: return semantics::BinaryOpType::Less;
        case lexer::TokenType::LTE: return semantics::BinaryOpType::LesserEqual;
        //case lexer::TokenType::GT: return semantics::BinaryOpType::Greater;
        case lexer::TokenType::GTE: return semantics::BinaryOpType::GreaterEqual;
        default:
            throw except::SyntaxError("Unexpected token type for binary operator",
                tokens_[current_].line(), tokens_[current_].column());
    }
}

int Parser::getPrecedence(lexer::TokenType type) const {
    switch (type) {
        // Logical OR (lowest of binary ops)
        case lexer::TokenType::OR: return 1;

        // Logical AND
        case lexer::TokenType::AND: return 2;

        // Equality
        case lexer::TokenType::EQ:
        case lexer::TokenType::NEQ: return 3;

        // Comparison
        case lexer::TokenType::LT:
        case lexer::TokenType::LTE:
        case lexer::TokenType::GT:
        case lexer::TokenType::GTE: return 4;

        // Addition/Subtraction
        case lexer::TokenType::PLUS:
        case lexer::TokenType::MINUS: return 5;

        // Multiplication/Division
        case lexer::TokenType::STAR:
        case lexer::TokenType::SLASH: return 6;

        // Unary operators typically have higher precedence (handled separately if needed)
        // Function calls, indexing, attribute access, etc., would be even higher

        default: return -1; // Not an operator or not part of an expression
    }
}

std::shared_ptr<semantics::ASTNode> Parser::parse() {
    std::vector<std::shared_ptr<semantics::ASTNode>> statements;
    while (!check(lexer::TokenType::END_OF_FILE)) {
        statements.push_back(parseStatement());
    }
    return std::make_shared<semantics::Code>(std::move(statements));
}

std::shared_ptr<semantics::ASTNode> Parser::parseExprStatement() {
    auto expr = parseExpression();

    if (match(lexer::TokenType::ASSIGN)) {
        auto value = parseExprStatement(); // recursive call

        // Attempt to cast expr to AssignableASTNode
        auto assignableExpr = std::dynamic_pointer_cast<semantics::AssignableASTNode>(expr);
        if (!assignableExpr) {
            throw except::SyntaxError("Left-hand side of assignment must be an assignable expression",
                tokens_[current_].line(), tokens_[current_].column());
        }
        return std::make_shared<semantics::Assign>(std::move(assignableExpr), std::move(value));
    }

    // could potentially have more options other than assignment (function calling for instance)

    expect(lexer::TokenType::NEWLINE, "Unexpected token after expression");
    return expr;
}

std::shared_ptr<semantics::ASTNode> Parser::parseStatement() {
    // statements starting with a keyword
    if (match(lexer::TokenType::IF)) return parseIf();
    if (match(lexer::TokenType::WHILE)) return parseWhile();
    if (match(lexer::TokenType::DEF)) return parseFunctionDef();
    if (match(lexer::TokenType::PRINT)) return parsePrint();

    return parseExprStatement();
}

// primary is a single component, like a variable, a number, or a string, or an expression in parentheses
std::shared_ptr<semantics::ASTNode> Parser::parsePrimary() {
    if (match(lexer::TokenType::IDENTIFIER)) {
        auto varName = tokens_[current_ - 1].value();
        return std::make_shared<semantics::Variable>(varName);
    }

    if (match(lexer::TokenType::INT)) {
        auto value = tokens_[current_ - 1].value();
        return std::make_shared<semantics::Const>(std::make_shared<core::Integer>(std::stoi(value)));
    }

    if (match(lexer::TokenType::STRING)) {
        auto value = tokens_[current_ - 1].value();
        return std::make_shared<semantics::Const>(std::make_shared<core::String>(value));
    }

    expect(lexer::TokenType::LPAREN, "Expected '(' before expression (got: " + tokens_[current_ - 1].value() + ")");
    auto expr = parseExpression();
    expect(lexer::TokenType::RPAREN, "Expected ')' after expression (got: " + tokens_[current_ - 1].value() + ")");
    return expr;
}

//
// parseUnary: handle unary operators such as NOT, or a unary '-' if you support it.
//
std::shared_ptr<semantics::ASTNode> Parser::parseUnary() {
    if (match(lexer::TokenType::NOT)) {
        // Unary operator: NOT a
        auto operand = parseUnary();
        return std::make_shared<semantics::UnaryOp>(semantics::UnaryOpType::Not, std::move(operand));
    }
    // Optionally, if you have a unary minus:
    if (match(lexer::TokenType::MINUS)) {
        auto operand = parseUnary();
        return std::make_shared<semantics::UnaryOp>(semantics::UnaryOpType::Neg, std::move(operand));
    }
    // Otherwise, fallback to primary expressions.
    return parsePrimary();
}

//
// parseExpression: uses precedence climbing.
// The parameter minPrecedence indicates the minimum binding power required to continue parsing.
std::shared_ptr<semantics::ASTNode> Parser::parseExpression(int minPrecedence) {
    // Start by parsing the left operand, which may be a unary expression.
    auto left = parseUnary();

    // Continue as long as there's an operator with sufficient precedence.
    while (!isAtEnd()) {
        lexer::Token op = peek();
        int prec = getPrecedence(op.type());

        // If the current operator's precedence is lower than what we require,
        // break out and return what we've parsed so far.
        if (prec < minPrecedence)
            break;
        
        // Consume the operator.
        advance();

        // Parse the right-hand side operand: note that we add 1
        // to enforce left associativity for binary operators.
        auto right = parseExpression(prec + 1);

        // Combine the left and right-hand sides into a new binary expression node.
        left = std::make_shared<semantics::BinaryOp>(mapBinaryOp(op.type()), std::move(left), std::move(right));
    }

    return left;
}

//
// Overload of parseExpression that starts with minimal precedence (zero).
//
std::shared_ptr<semantics::ASTNode> Parser::parseExpression() {
    return parseExpression(0);
}


std::shared_ptr<semantics::ASTNode> Parser::parseWhile() {
    auto condition = parseExpression();
    expect(lexer::TokenType::COLON, "Expected ':' after while condition");
    expect(lexer::TokenType::NEWLINE, "Expected newline after ':'");
    expect(lexer::TokenType::INDENT, "Expected indent after newline");

    std::shared_ptr<semantics::Code> body = std::make_shared<semantics::Code>();
    while (!match(lexer::TokenType::DEDENT) && !check(lexer::TokenType::END_OF_FILE)) {
        body->statements.push_back(parseStatement());
    }

    return std::make_shared<semantics::While>(std::move(condition), std::move(body));
}

std::shared_ptr<semantics::ASTNode> Parser::parseFunctionDef() {
    expect(lexer::TokenType::IDENTIFIER, "Expected function name after 'def'");
    auto funcName = tokens_[current_ - 1].value();
    expect(lexer::TokenType::LPAREN, "Expected '(' after function name");

    std::vector<std::string> params;
    while (!match(lexer::TokenType::RPAREN)) {
        if (check(lexer::TokenType::IDENTIFIER)) {
            params.push_back(tokens_[current_].value());
            advance(); // consume identifier
        } else {
            throw except::SyntaxError("Expected parameter name", 
                tokens_[current_].line(), tokens_[current_].column());
        }
        if (!match(lexer::TokenType::COMMA)) break; // consume comma
    }

    expect(lexer::TokenType::COLON, "Expected ':' after function parameters");
    expect(lexer::TokenType::NEWLINE, "Expected newline after ':'");
    expect(lexer::TokenType::INDENT, "Expected indent after newline");

    std::shared_ptr<semantics::Code> body = std::make_shared<semantics::Code>();
    while (!match(lexer::TokenType::DEDENT) && !check(lexer::TokenType::END_OF_FILE)) {
        body->statements.push_back(parseStatement());
    }

    return std::make_shared<semantics::Function>(funcName, params, std::move(body));
}

std::shared_ptr<semantics::ASTNode> Parser::parsePrint() {
    auto expression = parseExpression();
    expect(lexer::TokenType::NEWLINE, "Expected newline after print statement");
    return std::make_shared<semantics::Print>(std::move(expression));
}

std::shared_ptr<semantics::ASTNode> Parser::parseAssignment() {
    auto varName = tokens_[current_].value();
    advance(); // consume identifier
    expect(lexer::TokenType::ASSIGN, "Expected '=' after variable name");
    auto value = parseExpression();
    expect(lexer::TokenType::NEWLINE, "Expected newline after assignment");
    return std::make_shared<semantics::Assign>(std::make_shared<semantics::Variable>(varName), std::move(value));
}

std::shared_ptr<semantics::ASTNode> Parser::parseIf() {
    auto condition = parseExpression();
    expect(lexer::TokenType::COLON, "Expected ':' after if condition");
    expect(lexer::TokenType::NEWLINE, "Expected newline after ':'");
    expect(lexer::TokenType::INDENT, "Expected indent after newline");

    std::shared_ptr<semantics::Code> thenBranch = std::make_shared<semantics::Code>();
    while (!match(lexer::TokenType::DEDENT) && !check(lexer::TokenType::END_OF_FILE)) {
        thenBranch->statements.push_back(parseStatement());
    }

    std::shared_ptr<semantics::Code> elseBranch = std::make_shared<semantics::Code>();
    if (match(lexer::TokenType::ELSE)) {
        expect(lexer::TokenType::COLON, "Expected ':' after else");
        expect(lexer::TokenType::NEWLINE, "Expected newline after ':'");
        expect(lexer::TokenType::INDENT, "Expected indent after newline");

        while (!match(lexer::TokenType::DEDENT) && !check(lexer::TokenType::END_OF_FILE)) {
            elseBranch->statements.push_back(parseStatement());
        }
    }

    return std::make_shared<semantics::If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

bool Parser::isAtEnd() const {
    return current_ >= tokens_.size() || tokens_[current_].type() == lexer::TokenType::END_OF_FILE;
}

const lexer::Token& Parser::peek() const {
    return tokens_[current_];
}

const lexer::Token& Parser::advance() {
    if (!isAtEnd()) current_++;
    return previous();
}

const lexer::Token& Parser::previous() const {
    return tokens_[current_ - 1];
}

bool Parser::check(lexer::TokenType type) const {
    if (isAtEnd()) return false;
    return tokens_[current_].type() == type;
}

bool Parser::match(lexer::TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(lexer::TokenType type, const std::string& msg) {
    if (check(type)) {
        advance();
    } else {
        throw except::SyntaxError(msg, tokens_[current_].line(), tokens_[current_].column());
    }
}