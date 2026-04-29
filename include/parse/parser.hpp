/**
 * @file parser.hpp
 * @brief Defines the Parser class.
 * @details This header provides the recursive descent parser responsible for converting
 * a flat stream of tokens from the Lexer into an executable tree structure. It respects
 * mathematical precedence, associativity, and differentiates between prefix, postfix,
 * and binary operators dynamically via the Registry.
 */

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

#include "def_ast.hpp"
#include "../lexer/lexer.hpp"
#include "../reg/registry.hpp"

// ━━ PARSER EXCEPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @class ParseError
 * @brief Custom exception thrown when a syntax error is encountered.
 * @details Captures the exact token that caused the failure to allow the CLI
 * to pinpoint the error's line and column for the user.
 */
class ParseError : public std::runtime_error {
public:
    Token token; ///< The specific token where the syntax error occurred.
    
    ParseError(const std::string& message, const Token& t)
        : std::runtime_error(message), token(t) {}
};

// ━━ PARSER ENGINE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @class Parser
 * @brief Consumes Lexer tokens to construct an executable Abstract Syntax Tree.
 */
class Parser {
private:
    const std::vector<Token>& tokens; ///< Constant reference to the immutable token stream.
    size_t current = 0;               ///< Current index in the token stream.
    Registry& registry;               ///< Reference to the math logic engine.

public:
    /**
     * @brief Constructs a new Parser instance for a single evaluation run.
     * @param t The generated token stream from the Lexer.
     * @param r The system Registry to dynamically query math rules.
     */
    Parser(const std::vector<Token>& t, Registry& r)
        : tokens(t), registry(r) {}

    /**
     * @brief Begins the recursive descent parsing process.
     * @return A unique pointer to the root node of the AST.
     * @throws ParseError If the grammar rules are violated.
     */
    std::unique_ptr<ASTNode> parse();

private:
    // Recursive Descent Tiers
    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> parse_binary(int min_precedence);
    std::unique_ptr<ASTNode> unary();
    std::unique_ptr<ASTNode> postfix();
    std::unique_ptr<ASTNode> primary();

    // Navigation and Matching
    const Token* peek() const;
    const Token* advance();
    bool match(TokenType type);
    bool is_at_end() const;
    Token error_token() const;
};