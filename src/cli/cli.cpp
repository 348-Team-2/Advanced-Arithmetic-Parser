/**
* @file cli.cpp
 * @brief Manages all user input and output from the CLI.
 * @details This file is responsible for reading user input and printing results or error messages.
 */

#include "cli/cli.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

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

void CLI::print_error(const ParseError& e) {
    // TODO (CLI Dev): Add terminal color codes (e.g., Red) or formatting here.
    print("Syntax Error: " + std::string(e.what()));
    print("  At token: '" + e.token.lexeme +
                     "' (Line " + std::to_string(e.token.line) +
                     ", Col " + std::to_string(e.token.column) + ")");
}

void CLI::print_value(const Value& val) {
    print("");
}

// TODO: Take a look at err/debug. Do you want to move that here? If so, make sure to edit the Engine.hpp file in
// core/engine to use the correct functions.