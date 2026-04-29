/**
 * @file parser.hpp
 * @brief Defines the Parser class.
 * @details This header provides the recursive descent parser responsible for converting
 * a flat stream of tokens from the Lexer into an executable tree structure. It respects
 * mathematical precedence, associativity, and differentiates between prefix, postfix,
 * and binary operators dynamically via the Registry.
 */

#pragma once

#include <utility>
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
    
    ParseError(const std::string& message, Token t)
        : std::runtime_error(message), token(std::move(t)) {}
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
     * @details Acts as the main entry point. Expects to consume the entire token stream
     * for a valid expression.
     * @return A unique pointer to the root node of the fully constructed AST.
     * @throws ParseError If the grammar rules are violated or unparsed tokens remain.
     */
    std::unique_ptr<ASTNode> parse();

private:
    // ━━ Recursive Descent Tiers ━━

    /**
     * @brief Parses a top-level expression.
     * @details Acts as a wrapper around the binary parser, starting at the lowest precedence.
     * @return A unique pointer to the parsed expression subtree.
     */
    std::unique_ptr<ASTNode> expression();

    /**
     * @brief Parses binary operations using operator precedence parsing.
     * @details Loops continuously to bind operators and operands based on their relative precedence
     * and associativity rules queried from the Registry.
     * @param min_precedence The minimum binding power required to consume the next operator.
     * @return A unique pointer to the resulting binary operation node or lower-tier AST node.
     */
    std::unique_ptr<ASTNode> parse_binary(int min_precedence);

    /**
     * @brief Parses unary prefix operations.
     * @details Contextually maps standard symbols (like `-`, `+`, `!`) to their specific prefix
     * identifiers (`u-`, `u+`, `u!`) before checking the registry.
     * @return A unique pointer to a UnaryOpNode, or falls through to postfix/primary.
     */
    std::unique_ptr<ASTNode> unary();

    /**
     * @brief Parses unary postfix operations.
     * @details Contextually translates trailing operators (like `!`) into their postfix
     * identifiers (e.g., `p!`) and wraps the underlying expression.
     * @return A unique pointer to the wrapped expression, or the raw primary expression.
     */
    std::unique_ptr<ASTNode> postfix();

    /**
     * @brief Parses primary language constructs (numbers, constants, functions, grouping).
     * @details The most granular tier. Handles literals, verifies registry constants, parses
     * function calls with arguments, and processes parenthetical groupings.
     * @return A unique pointer to a leaf node (LiteralNode), FunctionCallNode, or grouped ASTNode.
     * @throws ParseError On unexpected tokens, unregistered identifiers, or mismatched parentheses.
     */
    std::unique_ptr<ASTNode> primary();

    // ━━ Navigation and Matching ━━

    /**
     * @brief Inspects the current token without consuming it.
     * @return A constant pointer to the token currently under the parser's cursor.
     */
    [[nodiscard]] const Token* peek() const;

    /**
     * @brief Consumes the current token and advances the internal stream pointer.
     * @return A constant pointer to the just-consumed token.
     */
    const Token* advance();

    /**
     * @brief Conditionally consumes a token if it matches the expected type.
     * @param type The TokenType to look for.
     * @return True if the token matched and was consumed, false otherwise.
     */
    bool match(TokenType type);

    /**
     * @brief Checks if the parser has exhausted the token stream.
     * @details Identifies boundaries by checking against the stream size or explicit EOF tokens.
     * @return True if there are no more actionable tokens to parse.
     */
    [[nodiscard]] bool is_at_end() const;

    /**
     * @brief Retrieves the token where the parser currently sits, typically for error reporting.
     * @return A copy of the token responsible for causing an exception.
     */
    [[nodiscard]] Token error_token() const;
};