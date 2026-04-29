/**
* @file def_cli.hpp
 * @brief Declares the CLI class which manages user input and output.
 * @details This header defines the interface for the CLI component of the calculator.
 */

#pragma once

#include <string>

#include "parse/parser.hpp"
#include "reg/def_value.hpp"

 /**
 * @class CLI
 * @brief Manages all user input and output from the CLI.
 * @details This class is responsible for reading user input and printing results or error messages.
 */
class CLI {
public:

    /// @brief Prints a message to the command line.
    static void print(const std::string& message);

    /// @brief Reads input from the user and removes all whitespace.
    static std::string read_input();

    // ━━ DELEGATED UI METHODS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    /// @brief Prints the introductory banner and instructions.
    static void print_header();

    /// @brief Prints the closing or exit message.
    static void print_footer();

    /// @brief Prints an error message to the console.
    /// @param e The error from the Engine/Parser.
    static void print_error(const ParseError& e); // TODO: Replace with Error generic?

    /// @brief Unpacks and prints the final mathematical Value.
    /// @param val The polymorphic result variant from the Evaluator.
    static void print_value(const Value& val);
};

