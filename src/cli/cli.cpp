/**
* @file cli.cpp
 * @brief Manages all user input and output from the CLI.
 * @details This file is responsible for reading user input and printing results or error messages.
 */

#include "cli/def_cli.hpp"
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
    std::erase_if(input, ::isspace);
    
    // add "\n" to end input
    input += "\n";
    return input;
}
