/**
* @file def_cli.hpp
 * @brief Declares the CLI class which manages user input and output.
 * @details This header defines the interface for the CLI component of the calculator.
 */


 /**
 * @class CLI
 * @brief Manages all user input and output from the CLI.
 * @details This class is responsible for reading user input and printing results or error messages.
 */
#pragma once

#include <string>

class CLI {
public:

    /// @brief Prints a message to the command line.
    static void print(const std::string& message);

    /// @brief Reads input from the user and removes all whitespace.
    static std::string read_input();
};
