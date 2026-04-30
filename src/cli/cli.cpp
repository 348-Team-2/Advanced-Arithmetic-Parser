/**
* @file cli.cpp
 * @brief Manages all user input and output from the CLI.
 * @details This file is responsible for reading user input and printing results or error messages.
 */

#include "cli/cli.hpp"
#include "cli/format_value.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>

namespace Color {
    const std::string RED = "\033[31m";
    const std::string RESET = "\033[0m";
}

void CLI::print(const std::string& message) {
    std::cout << message << std::endl;
}

// reads input and removes all whitespace characters
std::string CLI::read_input() {
    std::string input;
    std::getline(std::cin, input);
    
    // add "\n" to end input
    input += "\n";
    return input;
}

// ━━ CLI DEVELOPER STUBS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void CLI::print_header() {
    // TODO (CLI Dev): Feel free to replace this with elaborate ASCII art or whatever works
    print("        CLI Calculator Engine           ");
    print( "Type an expression to evaluate." );
}

void CLI::print_footer() {
    print("Exiting");
}

void CLI::print_error(const ParseError& e, const std::string* input) {
    print("Syntax Error: " + std::string(e.what()));

    print("  At token: '" + e.token.lexeme +
                     "' (Line " + std::to_string(e.token.line) +
                     ", Col " + std::to_string(e.token.column) + ")");
    
    if (input == nullptr) {
        return; // No input provided
    }

    // Handle optional error position feature
    std::istringstream stream(*input);
    std::string line;
    int current_line = 1;
    while (std::getline(stream, line)) {
        if (current_line == e.token.line) {
            print("  " + line); // Print the line with the error
            std::string carrot_line(
                static_cast<size_t>(std::max(e.token.column - 1, 0)), ' '); // Spaces before the carrot
            carrot_line += "^"; 
            print("  " + carrot_line); // Print the carrot pointing to the error column
            break;
        }
        current_line++;
    }
}

void CLI::print_value(const Value& val) {
    print(to_string(val));
}