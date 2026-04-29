/**
* @file engine.hpp
 * @brief Defines the Engine class that orchestrates the execution pipeline.
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

class Engine {
private:
    Registry registry;

public:
    Engine() {
        // Initialize the math engine with all operators, constants, and functions
        load_registry(registry);
    }

    /**
     * @brief Processes a raw string input through the entire mathematical pipeline.
     * @param input The raw user string (e.g., "-5! + sin(pi)")
     * @return The final evaluated mathematical Value.
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

        // TODO: Pass 'ast' to the  Evaluator class

        return 0.0;
    };
};