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
    // statements starting with a keyword
    if (match(TokenType::IF)) return parseIf();
    if (match(TokenType::WHILE)) return parseWhile();
    if (match(TokenType::DEF)) return parseFunctionDef();
    if (match(TokenType::PRINT)) return parsePrint();

    return parseExprStatement();
}

std::unique_ptr<ASTNode> Parser::parseExprStatement() {
    auto expr = parseExpression();

    if (match(TokenType::ASSIGN)) {
        auto value = parseExprStatement(); // recursive call
        return std::make_unique<AssignNode>(std::move(expr), std::move(value));
    }

    // could potentially have more options other than assignment (function calling for instance)

    expect(TokenType::NEWLINE, "Unexpected token after expression");
    return expr;
}


// primary is a single component, like a variable, a number, or a string, or an expression in parentheses
std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match(TokenType::IDENTIFIER)) {
        auto varName = tokens_[current_ - 1].value();
        return std::make_unique<semantics::Variable>(varName);
    }

    if (match(TokenType::INT)) {
        auto value = tokens_[current_ - 1].value();
        return std::make_unique<semantics::Const>(std::make_shared<core::Integer>(std::stoi(value)));
    }

    if (match(TokenType::STRING)) {
        auto value = tokens_[current_ - 1].value();
        return std::make_unique<semantics::Const>(std::make_shared<core::String>(value));
    }

    expect(TokenType::LPAREN, "Expected '(' after expression");
    auto expr = parseExpression();
    expect(TokenType::RPAREN, "Expected ')' after expression");
    return expr;
}

//
// parseUnary: handle unary operators such as NOT, or a unary '-' if you support it.
//
std::unique_ptr<ASTNode> Parser::parseUnary() {
    if (match(TokenType::NOT)) {
        // Unary operator: NOT a
        auto operand = parseUnary();
        return std::make_unique<UnaryOpNode>(UnaryOpType::Not, std::move(operand));
    }
    // Optionally, if you have a unary minus:
    if (match(TokenType::MINUS)) {
        auto operand = parseUnary();
        return std::make_unique<UnaryOpNode>(UnaryOpType::Neg, std::move(operand));
    }
    // Otherwise, fallback to primary expressions.
    return parsePrimary();
}

//
// parseExpression: uses precedence climbing.
// The parameter minPrecedence indicates the minimum binding power required to continue parsing.
std::unique_ptr<ASTNode> Parser::parseExpression(int minPrecedence) {
    // Start by parsing the left operand, which may be a unary expression.
    auto left = parseUnary();

    // Continue as long as there's an operator with sufficient precedence.
    while (!isAtEnd()) {
        Token op = peek();
        int prec = getPrecedence(op.type);

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
        left = std::make_unique<semantics::BinaryOp>(mapBinaryOp(op.type), std::move(left), std::move(right));
    }

    return left;
}

BinaryOpType mapBinaryOp(TokenType type) {
    switch (type) {
        case TokenType::PLUS: return BinaryOpType::Sum;
        case TokenType::MINUS: return BinaryOpType::Sub;
        case TokenType::STAR: return BinaryOpType::Mult;
        case TokenType::SLASH: return BinaryOpType::Div;
        case TokenType::EQ: return BinaryOpType::Equal;
        case TokenType::NEQ: return BinaryOpType::NotEqual;
        case TokenType::LT: return BinaryOpType::Less;
        case TokenType::LTE: return BinaryOpType::LessEqual;
        case TokenType::GT: return BinaryOpType::Greater;
        case TokenType::GTE: return BinaryOpType::GreaterEqual;
        default:
            throw except::SyntaxError("Unexpected token type for binary operator");
    }
}

int getPrecedence(TokenType type) {
    switch (type) {
        // Logical OR (lowest of binary ops)
        case TokenType::OR: return 1;

        // Logical AND
        case TokenType::AND: return 2;

        // Equality
        case TokenType::EQ:
        case TokenType::NEQ: return 3;

        // Comparison
        case TokenType::LT:
        case TokenType::LTE:
        case TokenType::GT:
        case TokenType::GTE: return 4;

        // Addition/Subtraction
        case TokenType::PLUS:
        case TokenType::MINUS: return 5;

        // Multiplication/Division
        case TokenType::STAR:
        case TokenType::SLASH: return 6;

        // Unary operators typically have higher precedence (handled separately if needed)
        // Function calls, indexing, attribute access, etc., would be even higher

        default: return -1; // Not an operator or not part of an expression
    }
}

//
// Overload of parseExpression that starts with minimal precedence (zero).
//
std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseExpression(0);
}


std::unique_ptr<ASTNode> Parser::parseWhile() {
    auto condition = parseExpression();
    expect(TokenType::COLON, "Expected ':' after while condition");
    expect(TokenType::NEWLINE, "Expected newline after ':'");
    expect(TokenType::INDENT, "Expected indent after newline");

    std::unique_ptr<semantics::Code> body = std::make_unique<semantics::Code>();
    while (!match(TokenType::DEDENT) && !check(TokenType::END_OF_FILE)) {
        body->statements.push_back(parseStatement());
    }

    return std::make_unique<semantics::While>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseFunctionDef() {
    expect(TokenType::IDENTIFIER, "Expected function name after 'def'");
    auto funcName = tokens_[current_ - 1].value();
    expect(TokenType::LPAREN, "Expected '(' after function name");

    std::vector<std::string> params;
    while (!match(TokenType::RPAREN)) {
        if (check(TokenType::IDENTIFIER)) {
            params.push_back(tokens_[current_].value());
            advance(); // consume identifier
        } else {
            throw except::SyntaxError("Expected parameter name");
        }
        if (!match(TokenType::COMMA)) break; // consume comma
    }

    expect(TokenType::COLON, "Expected ':' after function parameters");
    expect(TokenType::NEWLINE, "Expected newline after ':'");
    expect(TokenType::INDENT, "Expected indent after newline");

    std::unique_ptr<semantics::Code> body = std::make_unique<semantics::Code>();
    while (!match(TokenType::DEDENT) && !check(TokenType::END_OF_FILE)) {
        body->statements.push_back(parseStatement());
    }

    return std::make_unique<semantics::Function>(funcName, params, std::move(body));
}

std::unique_ptr<ASTNode> Parser::parsePrint() {
    auto expression = parseExpression();
    expect(TokenType::NEWLINE, "Expected newline after print statement");
    return std::make_unique<semantics::Print>(std::move(expression));
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
    auto varName = tokens_[current_].value();
    advance(); // consume identifier
    expect(TokenType::ASSIGN, "Expected '=' after variable name");
    auto value = parseExpression();
    expect(TokenType::NEWLINE, "Expected newline after assignment");
    return std::make_unique<semantics::Assign>(std::make_unique<semantics::Variable>(varName), std::move(value));
}

std::unique_ptr<ASTNode> Parser::parseIf() {
    auto condition = parseExpression();
    expect(TokenType::COLON, "Expected ':' after if condition");
    expect(TokenType::NEWLINE, "Expected newline after ':'");
    expect(TokenType::INDENT, "Expected indent after newline");

    std::unique_ptr<semantics::Code> thenBranch = std::make_unique<semantics::Code>();
    while (!match(TokenType::DEDENT) && !check(TokenType::END_OF_FILE)) {
        thenBranch->statements.push_back(parseStatement());
    }

    std::unique_ptr<semantics::Code> elseBranch = std::make_unique<semantics::Code>();
    if (match(TokenType::ELSE)) {
        expect(TokenType::COLON, "Expected ':' after else");
        expect(TokenType::NEWLINE, "Expected newline after ':'");
        expect(TokenType::INDENT, "Expected indent after newline");

        while (!match(TokenType::DEDENT) && !check(TokenType::END_OF_FILE)) {
            elseBranch->statements.push_back(parseStatement());
        }
    }

    return std::make_unique<semantics::If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

bool Parser::isAtEnd() const {
    return current_ >= tokens_.size() || tokens_[current_].type == TokenType::EOF;
}

const Token& Parser::peek() const {
    return tokens_[current_];
}

const Token& Parser::advance() {
    if (!isAtEnd()) current_++;
    return previous();
}

const Token& Parser::previous() const {
    return tokens_[current_ - 1];
}