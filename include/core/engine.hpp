/**
 * @file engine.hpp
 * @brief Defines the Engine class that orchestrates the execution pipeline.
 * @details This header acts as the primary facade for the CLI calculator, managing
 * the lifecycle and data flow between the Lexer, Parser, and Evaluator modules.
 */

#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "lexer/lexer.hpp"
#include "parse/parser.hpp"
#include "reg/registry.hpp"
#include "reg/math.hpp"
#include "err/debug.hpp"

// ━━ ENGINE CORE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @class Engine
 * @brief The central orchestrator for evaluating mathematical expressions.
 * @details The Engine class encapsulates the entire compilation and execution
 * pipeline. It holds the master Registry and handles the sequential passing of
 * data from raw string input -> Lexer (Tokens) -> Parser (AST) -> Evaluator (Value).
 */
class Engine {
private:
    Registry registry; ///< The master repository of mathematical rules, constants, and functions.

public:
    /**
     * @brief Constructs the Engine and initializes the mathematical environment.
     * @details Automatically calls `load_registry()` to populate the internal
     * Registry with standard operators, trigonometric functions, and math constants
     * before any execution begins.
     */
    Engine() {
        // Initialize the math engine with all operators, constants, and functions
        load_registry(registry);
    }

    /**
     * @brief Processes a raw string input through the entire mathematical pipeline.
     * @details This method acts as the main entry point for computation. It routes
     * the data through the following stages:
     * 1. **Tokenization:** Uses the Lexer to convert the string into Tokens.
     * 2. **Verification:** Ensures the generated token stream is not empty.
     * 3. **Parsing:** Uses the Parser to construct an Abstract Syntax Tree (AST).
     * 4. **Debugging:** Hooks into the error/debug module to print the token stream,
     * AST structure, and Graphviz visualizer data.
     * 5. **Evaluation:** (TODO) Traverses the AST to compute the final result.
     * * @param input The raw user string representing a mathematical expression (e.g., "-5! + sin(pi)").
     * @return The final evaluated mathematical Value (currently returning a 0.0 placeholder).
     * @throws std::runtime_error If the parsed expression is completely empty.
     * @throws ParseError (Propagated) If syntax rules are violated during AST construction.
     */
    Value execute(const std::string& input) {

        Lexer lexer(registry);
        std::vector<Token> tokens = lexer.tokenize(input);

        print_token_stream(tokens);

        if (tokens.empty() || tokens.front().type == TokenType::EOF_TOKEN) {
            throw std::runtime_error("Empty expression.");
        }

        Parser parser(tokens, registry);
        auto ast = parser.parse();

        print_ast(ast.get());
        print_ast_graphviz(ast.get());

        // TODO: Pass 'ast' to the Evaluator class

        return 0.0;
    };
};