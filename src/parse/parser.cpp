#include "parse/parser.hpp"
#include "parse/def_ast.hpp"
#include "reg/registry.hpp"

// ━━ PUBLIC ENTRY POINT ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

std::unique_ptr<ASTNode> Parser::parse() {
    auto ast = expression();
    if (!is_at_end()) {
        throw ParseError("Unexpected tokens found after complete expression.", error_token());
    }
    return ast;
}

// ━━ RECURSIVE DESCENT PIPELINE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

std::unique_ptr<ASTNode> Parser::expression() {
    return parse_binary(0);
}

std::unique_ptr<ASTNode> Parser::parse_binary(int min_prec) {
    auto left = unary();

    while (!is_at_end() && peek()->type == TokenType::OPERATOR) {
        std::string op = peek()->lexeme;

        // Skip operators that aren't registered as binary (e.g., standalone prefix)
        if (!registry.is_binary(op)) break;

        const int prec = registry.get_precedence(op);
        if (prec < min_prec) break;

        const Associativity assoc = registry.get_associativity(op);
        const int next_min_prec = prec + (assoc == Associativity::LEFT ? 1 : 0);

        advance(); // Consume operator
        auto right = parse_binary(next_min_prec);

        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::unary() {
    if (peek() && peek()->type == TokenType::OPERATOR) {
        std::string op = peek()->lexeme;

        // Contextual Translation: Map standard symbols to their prefix variants
        if (op == "-") op = "u-";
        else if (op == "+") op = "u+";
        else if (op == "!") op = "u!"; // Prefix logical NOT

        if (registry.is_unary(op)) {
            advance(); // Consume the operator token
            auto operand = unary(); // Recurse to allow stacked prefix (e.g., -!5)
            return std::make_unique<UnaryOpNode>(op, std::move(operand));
        }
    }

    return postfix();
}

std::unique_ptr<ASTNode> Parser::postfix() {
    auto expr = primary();

    while (peek() && peek()->type == TokenType::OPERATOR) {
        std::string op = peek()->lexeme;

        // Contextual Translation: Map standard symbols to their postfix variants
        if (op == "!") op = "p!"; // Postfix factorial

        if (registry.is_unary(op)) {
            advance(); // Consume the postfix operator
            // Wrap the existing expression as the operand
            expr = std::make_unique<UnaryOpNode>(op, std::move(expr));
        } else {
            break; // Not a postfix operator, kick back up to parse_binary
        }
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::primary() {
    // ━━ NUMBERS & CONSTANTS ━━
    if (peek()->type == TokenType::NUMBER) {
        const Token* token = advance();
        return std::make_unique<LiteralNode>(token->literal);
    }

    if (peek()->type == TokenType::IDENTIFIER) {
        const Token* token = advance();
        // Since Lexer already fetches the Value for constants, treat it as a literal
        if (registry.is_const(token->lexeme)) {
            return std::make_unique<LiteralNode>(token->literal);
        }
        throw ParseError("Unknown identifier or unregistered variable: " + token->lexeme, *token);
    }

    // ━━ FUNCTIONS ━━
    if (peek()->type == TokenType::FUNCTION) {
        const Token* token = advance();
        std::string name = token->lexeme;

        if (!match(TokenType::LPAREN)) {
            throw ParseError("Expected '(' after function name.", error_token());
        }

        std::vector<std::unique_ptr<ASTNode>> args;

        if (!match(TokenType::RPAREN)) {
            do {
                args.push_back(expression());
            } while (match(TokenType::COMMA));

            if (!match(TokenType::RPAREN)) {
                throw ParseError("Expected ')' after function arguments.", error_token());
            }
        }

        return std::make_unique<FunctionCallNode>(name, std::move(args));
    }

    // ━━ GROUPING ━━
    if (match(TokenType::LPAREN)) {
        auto expr = expression();
        if (!match(TokenType::RPAREN)) {
            throw ParseError("Mismatched parentheses, expected closing ')'.", error_token());
        }
        return expr;
    }

    // ━━ FALLBACK ERROR ━━
    throw ParseError("Unexpected token encountered during parsing.", error_token());
}

// ━━ INTERNAL NAVIGATION HELPERS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

const Token* Parser::peek() const {
    if (is_at_end()) return &tokens.back(); // Safely return EOF token if out of bounds
    return &tokens[current];
}

const Token* Parser::advance() {
    if (!is_at_end()) current++;
    return &tokens[current - 1];
}

bool Parser::match(TokenType type) {
    if (is_at_end() || peek()->type != type) {
        return false;
    }
    advance();
    return true;
}

bool Parser::is_at_end() const {
    // Ensures we don't access out of bounds, and stops at the Lexer's EOF token
    return current >= tokens.size() || tokens[current].type == TokenType::EOF_TOKEN;
}

Token Parser::error_token() const {
    // Utility to grab the offending token for the exception system
    if (current >= tokens.size()) return tokens.back(); // Usually EOF
    return tokens[current];
}