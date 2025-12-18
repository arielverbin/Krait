#include "Parser.hpp"
#include "exceptions/exceptions.hpp"

#include "core/gc/GarbageCollector.hpp"

#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/Float.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/None.hpp"

#include "semantics/Const.hpp"
#include "semantics/Variable.hpp"
#include "semantics/Print.hpp"

#include "semantics/flow_semantics/If.hpp"
#include "semantics/flow_semantics/While.hpp"
#include "semantics/flow_semantics/Code.hpp"
#include "semantics/flow_semantics/Pass.hpp"
#include "semantics/flow_semantics/Call.hpp"

#include "semantics/define_semantics/ClassDef.hpp"
#include "semantics/define_semantics/FunctionDef.hpp"
#include "semantics/define_semantics/Decorator.hpp"
#include "semantics/define_semantics/Assign.hpp"

#include "semantics/signal_semantics/Break.hpp"
#include "semantics/signal_semantics/Return.hpp"
#include "semantics/signal_semantics/Continue.hpp"

#include "semantics/operation_semantics/UnaryOp.hpp"
#include "semantics/operation_semantics/BinaryOp.hpp"
#include "semantics/operation_semantics/AccessProperty.hpp"

namespace parser {

Parser::Parser() {
    // Binary (infix) operators
    infixTable_ = {
        // Lowest precedence: assignment (right‑associative).
        // e.g. `a = b = c` parses as `a = (b = c)`
        {lexer::TokenType::ASSIGN,  {1, Associativity::RIGHT}},

        // Logical operators: AND binds tighter than OR,
        // e.g. `a or b and c` parses as `a or (b and c)`
        {lexer::TokenType::OR,      {2, Associativity::LEFT}},
        {lexer::TokenType::AND,     {3, Associativity::LEFT}},

        // Equality operators: bind tighter than logical operators.
        // e.g. `a == b or c` parses as `(a == b) or c`
        {lexer::TokenType::EQ,      {4, Associativity::LEFT}},
        {lexer::TokenType::NEQ,     {4, Associativity::LEFT}},

        // Comparison operators: bind tighter than equality operators.
        // e.g. `a <= b == False` parses as `(a <= b) == False`
        {lexer::TokenType::LT,      {5, Associativity::LEFT}},
        {lexer::TokenType::LTE,     {5, Associativity::LEFT}},
        {lexer::TokenType::GT,      {5, Associativity::LEFT}},
        {lexer::TokenType::GTE,     {5, Associativity::LEFT}},

        // Arithmetic operators: bind tighter than comparison operators.
        // e.g. `a + b >= c` parses as `(a + b) >= c`
        {lexer::TokenType::PLUS,    {6, Associativity::LEFT}},
        {lexer::TokenType::MINUS,   {6, Associativity::LEFT}},

        // Multiplication and division bind tighter than addition and subtraction.
        // e.g. `a + b * c + d` parses as `a + (b * c) + d`
        {lexer::TokenType::STAR,    {7, Associativity::LEFT}},
        {lexer::TokenType::SLASH,   {7, Associativity::LEFT}},
        {lexer::TokenType::MOD,     {7, Associativity::LEFT}},
    };

    // Prefix (unary) operators
    prefixTable_ = {
        // Negation: binds tighter than multiplication or division.
        // e.g. `-a * -b` parses as `(-a) * (-b)`
        // e.g. `-a + b` parses as `(-a) + b`
        {lexer::TokenType::MINUS, 8},

        // Logical Not operator: binds looser than everything.
        // e.g. `not a == b` parses as `not (a == b)`
        // e.g. `not a * b` parses as `not (a * b)`
        // e.g. `not (a + b) == c` parses as `not ((a + b) == c)`
        // e.g. `not a = b` parses as `not (a = b)`
        {lexer::TokenType::NOT,   0},
    };
}

std::shared_ptr<semantics::ASTNode> Parser::parse(std::vector<lexer::Token>& tokens) {
    context_.load(tokens);

    std::vector<std::shared_ptr<semantics::ASTNode>> statements;
    while (!isAtEnd()) {
        auto statement = parseStatement();
        statements.push_back(statement);
    }
    return std::make_shared<semantics::Code>(std::move(statements));
}

std::shared_ptr<semantics::ASTNode> Parser::parseStatement() {
    if (match(lexer::TokenType::IF))       return parseIf();
    if (match(lexer::TokenType::WHILE))    return parseWhile();
    if (match(lexer::TokenType::DEF))      return parseFunctionDef();
    if (match(lexer::TokenType::CLASS))    return parseClassDef();
    if (match(lexer::TokenType::PRINT))    return parsePrint();
    if (match(lexer::TokenType::RETURN))   return parseReturn();
    if (match(lexer::TokenType::PASS))     return parsePass();
    if (match(lexer::TokenType::BREAK))    return parseBreak();
    if (match(lexer::TokenType::CONTINUE)) return parseContinue();
    if (match(lexer::TokenType::AT))       return parseDecorator();

    auto expr = parseExpression(0);
    expect(lexer::TokenType::NEWLINE, "Expected newline after statement");
    return expr;
}

std::shared_ptr<semantics::ASTNode> Parser::parseExpression(int minBp) {
    auto left = parsePrimary();

    // Postfix operators (like function calling or member lookup)
    // This layout keeps posfix operators at the correct precedence -- tighter
    // than any binary operator but looser than grouping​.
    left = parsePostfix(left);

    // While there’s an infix whose lbp >= minBp
    while (!isAtEnd()) {
        auto tok = peek();
        int lbp = getBinaryPrecedence(tok.type());
        if (lbp <= minBp) break;

        advance(); // consume operator
        left = parseInfix(left, tok, lbp);
    }
    return left;
}

std::shared_ptr<semantics::ASTNode> Parser::parsePrimary() {
    if (match(lexer::TokenType::INT)) {
        const auto& number = previous().value();
        return std::make_shared<semantics::Const>(createConst(gc::make_tracked<core::Integer>(std::stoi(number))));
    }

    if (match(lexer::TokenType::FLOAT)) {
        const auto& number = previous().value();
        return std::make_shared<semantics::Const>(createConst(gc::make_tracked<core::Float>(std::stod(number))));
    }

    if (match(lexer::TokenType::TRU)) {
        return std::make_shared<semantics::Const>(core::Boolean::get(true));
    }

    if (match(lexer::TokenType::FALS)) {
        return std::make_shared<semantics::Const>(core::Boolean::get(false));
    }

    if (match(lexer::TokenType::NONE)) {
        return std::make_shared<semantics::Const>(core::None::getNone());
    }

    if (match(lexer::TokenType::STRING)) {
        const auto& varName = previous().value();
        return std::make_shared<semantics::Const>(createConst(gc::make_tracked<core::String>(varName)));
    }

    if (match(lexer::TokenType::IDENTIFIER)) {
        const auto& varName = previous().value();
        return std::make_shared<semantics::Variable>(varName);
    }
    if (match(lexer::TokenType::LPAREN)) {
        auto expr = parseExpression();
        expect(lexer::TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }

    // Prefix/unary operators
    if (match(lexer::TokenType::MINUS) || match(lexer::TokenType::NOT)) {
        auto op = previous();
        int r_bp = getUnaryPrecedence(op.type());
        auto rhs = parseExpression(r_bp);
        return std::make_shared<semantics::UnaryOp>(mapUnaryOp(op), rhs);
    }

    throw except::SyntaxError("Unexpected token in expression", peek().line(), peek().column());
}

std::shared_ptr<semantics::ASTNode> Parser::parseInfix(
    std::shared_ptr<semantics::ASTNode> left,
    const lexer::Token& op,
    int lbp
) {
    // Right‑assoc: reduce next call’s minBp by 1
    int nextMinBp = lbp - (isRightAssociative(op) ? 1 : 0);
    auto right = parseExpression(nextMinBp);

    // Handle special binary operators (like assignment) here
    if (op.type() == lexer::TokenType::ASSIGN) {
        auto assignableExpr = std::dynamic_pointer_cast<semantics::AssignableASTNode>(left);
        if (!assignableExpr) {
            throw except::SyntaxError("Left-hand side of assignment must be an assignable expression",
                op.line(), op.column());
        }
        return std::make_shared<semantics::Assign>(assignableExpr, right);
    }
    return std::make_shared<semantics::BinaryOp>(mapBinaryOp(op), left, right);
}

std::shared_ptr<semantics::ASTNode> Parser::parsePostfix(std::shared_ptr<semantics::ASTNode> left) {
    while (true) {
        if (match(lexer::TokenType::LPAREN)) {
            // Function call
            std::vector<std::shared_ptr<semantics::ASTNode>> args;
            if (!check(lexer::TokenType::RPAREN)) {
                do {
                    args.push_back(parseExpression(0));
                } while (match(lexer::TokenType::COMMA));
            }
            expect(lexer::TokenType::RPAREN, "Expect ')' after arguments");
            left = std::make_shared<semantics::Call>(left, std::move(args));
        }

        else if (match(lexer::TokenType::DOT)) {
            // Member access
            expect(lexer::TokenType::IDENTIFIER, "Expect property name after '.'");
            left = std::make_shared<semantics::AccessProperty>(left, previous().value());
        }

        else {
            break;
        }
    }
    return left;
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
    auto funcName = previous().value();
    expect(lexer::TokenType::LPAREN, "Expected '(' after function name");

    std::vector<std::string> params;
    if (!check(lexer::TokenType::RPAREN)) {
        // parse first param (and then any that follow after commas)
        do {
            expect(lexer::TokenType::IDENTIFIER, "Expected parameter name");
            params.push_back(previous().value());
        } while (match(lexer::TokenType::COMMA));
    }

    expect(lexer::TokenType::RPAREN, "Expected ')' after function parameters");
    expect(lexer::TokenType::COLON, "Expected ':' after function parameters");
    expect(lexer::TokenType::NEWLINE, "Expected newline after ':'");
    expect(lexer::TokenType::INDENT, "Expected indent after newline");

    std::shared_ptr<semantics::Code> body = std::make_shared<semantics::Code>();
    while (!match(lexer::TokenType::DEDENT) && !check(lexer::TokenType::END_OF_FILE)) {
        body->statements.push_back(parseStatement());
    }

    return std::make_shared<semantics::FunctionDef>(funcName, params, std::move(body));
}

std::shared_ptr<semantics::ASTNode> Parser::parseClassDef() {
    expect(lexer::TokenType::IDENTIFIER, "Expected class name after 'class'");
    auto className = previous().value();

    expect(lexer::TokenType::COLON, "Expected ':' after class name");
    expect(lexer::TokenType::NEWLINE, "Expected newline after ':'");
    expect(lexer::TokenType::INDENT, "Expected indent after newline");

    std::shared_ptr<semantics::Code> body = std::make_shared<semantics::Code>();
    while (!match(lexer::TokenType::DEDENT) && !check(lexer::TokenType::END_OF_FILE)) {
        body->statements.push_back(parseStatement());
    }

    return std::make_shared<semantics::ClassDef>(className, std::move(body));
}

std::shared_ptr<semantics::ASTNode> Parser::parseDecorator() {
    auto decorator = parseExpression();
    expect(lexer::TokenType::NEWLINE, "Expected newline after decorator");
    auto decorated = parseStatement();

    auto bindableDecorated = std::dynamic_pointer_cast<semantics::BindableASTNode>(decorated);
    if (!bindableDecorated) {
        throw except::SyntaxError("Decorator can only be applied to bindables (functions or classes)",
            peek().line(), peek().column());
    }
    return std::make_shared<semantics::Decorator>(decorator, bindableDecorated);
}

std::shared_ptr<semantics::ASTNode> Parser::parsePrint() {
    auto expression = parseExpression();
    expect(lexer::TokenType::NEWLINE, "Expected newline after print statement");
    return std::make_shared<semantics::Print>(std::move(expression));
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

std::shared_ptr<semantics::ASTNode> Parser::parseReturn() {
    if (match(lexer::TokenType::NEWLINE))
        return std::make_shared<semantics::Return>();

    auto returnValue = parseExpression();
    expect(lexer::TokenType::NEWLINE, "Expected newline after statement");

    return std::make_shared<semantics::Return>(returnValue);
}

std::shared_ptr<semantics::ASTNode> Parser::parseBreak() {
    expect(lexer::TokenType::NEWLINE, "Expected newline after statement");

    return std::make_shared<semantics::Break>();
}

std::shared_ptr<semantics::ASTNode> Parser::parsePass() {
    expect(lexer::TokenType::NEWLINE, "Expected newline after statement");

    return std::make_shared<semantics::Pass>();
}

std::shared_ptr<semantics::ASTNode> Parser::parseContinue() {
    expect(lexer::TokenType::NEWLINE, "Expected newline after statement");

    return std::make_shared<semantics::Continue>();
}

bool Parser::isAtEnd() const {
    return context_.current >= context_.tokens.size() || 
        (context_.tokens[context_.current].type() == lexer::TokenType::END_OF_FILE);
}

const lexer::Token& Parser::peek() const {
    return context_.tokens[context_.current];
}

const lexer::Token& Parser::advance() {
    if (!isAtEnd()) context_.current++;
    return previous();
}

const lexer::Token& Parser::previous() const {
    return context_.tokens[context_.current - 1];
}

bool Parser::check(const lexer::TokenType& type) const {
    if (isAtEnd()) return false;
    return context_.tokens[context_.current].type() == type;
}

bool Parser::match(const lexer::TokenType& type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(const lexer::TokenType& type, const std::string& msg) {
    if (check(type)) {
        advance();
    } else {
        throw except::SyntaxError(msg, peek().line(), peek().column());
    }
}

int Parser::getBinaryPrecedence(const lexer::TokenType& type) const {
    auto it = infixTable_.find(type);
    return it != infixTable_.end() ? it->second.precedence : -1;
}

int Parser::getUnaryPrecedence(const lexer::TokenType& type) const {
    auto it = prefixTable_.find(type);
    return it != prefixTable_.end()? it->second : -1;
}

bool Parser::isRightAssociative(const lexer::Token& token) const {
    auto it = infixTable_.find(token.type());
    if (it == infixTable_.end()) {
        throw except::SyntaxError("Incorrect use of operator", token.line(), token.column());
    }
    return it->second.associativity == Associativity::RIGHT;
}

semantics::BinaryOpType Parser::mapBinaryOp(const lexer::Token& token) const {
    switch (token.type()) {
        case lexer::TokenType::OR: return semantics::BinaryOpType::Or;
        case lexer::TokenType::AND: return semantics::BinaryOpType::And;

        case lexer::TokenType::EQ: return semantics::BinaryOpType::Equal;
        case lexer::TokenType::NEQ: return semantics::BinaryOpType::NotEqual;

        case lexer::TokenType::LT: return semantics::BinaryOpType::LesserThan;
        case lexer::TokenType::LTE: return semantics::BinaryOpType::LesserEqual;
        case lexer::TokenType::GT: return semantics::BinaryOpType::GreaterThan;
        case lexer::TokenType::GTE: return semantics::BinaryOpType::GreaterEqual;

        case lexer::TokenType::PLUS: return semantics::BinaryOpType::Sum;
        case lexer::TokenType::MINUS: return semantics::BinaryOpType::Sub;

        case lexer::TokenType::STAR: return semantics::BinaryOpType::Mult;
        case lexer::TokenType::SLASH: return semantics::BinaryOpType::Div;
        case lexer::TokenType::MOD: return semantics::BinaryOpType::Mod;

        default:
            throw except::SyntaxError("Unexpected token type for binary operator", token.line(), token.column());
    }
}

semantics::UnaryOpType Parser::mapUnaryOp(const lexer::Token& token) const {
    switch (token.type()) {
        case lexer::TokenType::NOT: return semantics::UnaryOpType::Not;
        case lexer::TokenType::MINUS: return semantics::UnaryOpType::Neg;
        default:
            throw except::SyntaxError("Unexpected token type for unary operator", token.line(), token.column());
    }
}

core::Object* Parser::createConst(core::Object* obj) {
    gc::GCPool::instance().add(obj);
    return obj;
}

} // namespace parser
