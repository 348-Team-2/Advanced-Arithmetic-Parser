/**
 * @file lexer.hpp
 * @brief Defines the Lexer class responsible for tokenizing raw string input.
 */

#pragma once

#include <string>
#include <vector>

#include "reg/registry.hpp"
#include "def_token.hpp"

/**
 * @class Lexer
 * @brief Performs lexical analysis on mathematical expressions.
 * @details The Lexer acts as the first stage of the parser pipeline. It takes a raw string
 * of characters from the CLI and groups them into meaningful semantic units called Tokens.
 * Instead of hardcoding language rules, the Lexer delegates validation to the Registry,
 * making it entirely data-driven and easily extensible.
 */
class Lexer {
private:
    std::string source;                 ///< The raw input string from the CLI.
    std::vector<Token> tokens;          ///< The resulting list of parsed tokens.
    const Registry& registry;           ///< Reference to the math engine registry.

    size_t start = 0;                   ///< Start index of the current lexeme being parsed.
    size_t current = 0;                 ///< Current character index being evaluated.
    size_t line = 1;                    ///< Current line number (for error tracking).
    size_t column_start = 1;            ///< Column number where the current lexeme started.

    // ━━ Navigation Helpers ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    /**
     * @brief Checks if the lexer has consumed all characters in the source string.
     * @return true if the current index is at or beyond the string length, false otherwise.
     */
    [[nodiscard]] bool is_at_end() const;

    /**
     * @brief Consumes the current character and advances the lexer to the next one.
     * @return The character that was just consumed.
     */
    char advance();

    /**
     * @brief Looks at the current unconsumed character without advancing the lexer.
     * @return The current character, or the null terminator '\0' if at the end of the string.
     */
    [[nodiscard]] char peek() const;

    /**
     * @brief Looks ahead one character beyond the current unconsumed character.
     * @return The next character, or the null terminator '\0' if at the end of the string.
     */
    [[nodiscard]] char peek_next() const;

    /**
     * @brief Safely advances past all space, tab, carriage return, and newline characters.
     * @details Also increments the internal line counter whenever a newline is encountered
     * to maintain accurate error reporting.
     */
    void skip_whitespace();

    // ━━ Tokenization Subroutines ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    /**
     * @brief The primary dispatcher for reading a single token.
     * @details Evaluates the current character and routes execution to the appropriate
     * specific processing function (e.g., process_number, process_operator).
     */
    void scan_token();

    /**
     * @brief Determines if a specific character is a valid mathematical symbol.
     * @param c The character to check.
     * @return true if the character is a recognized symbol (e.g., '+', '*', '|'), false otherwise.
     */
    static bool is_symbol(char c);

    /**
     * @brief Groups contiguous symbol characters and resolves them against the Registry.
     * @details Utilizes the Maximal Munch algorithm. It builds a string of valid symbol characters
     * and shrinks it from the right until it finds the longest valid operator registered in the engine.
     */
    void process_operator();

    /**
     * @brief Parses contiguous digits and optional decimal points into a numerical Token.
     * @details Safely handles floating-point decimals and converts the extracted substring
     * into a double before encapsulating it in a Value variant.
     */
    void process_number();

    /**
     * @brief Parses contiguous alphanumeric characters to identify variables, constants, or functions.
     * @details Extracts the string and queries the Registry. If the string is a registered function
     * or constant, it creates the corresponding Token. Otherwise, it defaults to a generic identifier.
     */
    void process_identifier();

    /**
     * @brief Constructs a new Token and appends it to the internal tokens vector.
     * @param type The categorical classification of the extracted lexeme.
     * @param literal The encapsulated Value variant (defaults to an empty Value if not a number/constant).
     */
    void add_token(TokenType type, const Value& literal = Value());

public:
    /**
     * @brief Constructs the Lexer with a reference to the active Registry.
     * @param reg The initialized Registry containing the mathematical language rules.
     */
    explicit Lexer(const Registry& reg);

    ~Lexer() = default;

    /**
     * @brief Converts a raw mathematical string into a structured list of tokens.
     * @param input The raw string provided by the user via the CLI.
     * @return A vector of validated Token objects ready to be ingested by the Parser.
     * @note Automatically appends an EOF_TOKEN to the end of the vector to signal completion.
     */
    std::vector<Token> tokenize(const std::string& input);
};