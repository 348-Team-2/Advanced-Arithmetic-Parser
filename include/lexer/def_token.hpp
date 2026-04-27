/**
 * @file token.hpp
 * @brief Defines the foundational lexical units for the parsing pipeline.
 * @details This file contains the data structures used by the Lexer to categorize 
 * raw string input into structured, semantic units. These Tokens are then passed 
 * to the Parser to construct the Abstract Syntax Tree (AST).
 */

#pragma once

#include <string>
#include <utility>
#include "../reg/def_value.hpp"

/**
 * @enum TokenType
 * @brief Defines the structural and semantic classification of a lexical unit.
 * @details The Lexer assigns a TokenType to every extracted chunk of text. 
 * The Parser relies on these categories to enforce grammatical rules (e.g., 
 * ensuring an operation is followed by a number, or matching parentheses).
 */
enum class TokenType {
    // ━━ Literals & Variables ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    
    NUMBER,      ///< A numeric literal (e.g., "3.14", "-42").
    IDENTIFIER,  ///< A named variable or constant (e.g., "pi", "x").

    // ━━ Operations ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    
    OPERATOR,    ///< A mathematical operator verified by the Registry (e.g., "+", "**").
    FUNCTION,    ///< A mathematical function verified by the Registry (e.g., "sin").

    // ━━ Structural Delimiters ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    
    LPAREN,      ///< Left parenthesis `(` for grouping and function calls.
    RPAREN,      ///< Right parenthesis `)` for closing groups.
    COMMA,       ///< Comma `,` used to separate arguments in multi-arity functions.

    // ━━ Complex Type Delimiters (Future Scaling) ━━━━━━━━━━━━━━━━━━━━━━━━
    
    LBRACKET,    ///< Left bracket `[` for Vector/Matrix definition.
    RBRACKET,    ///< Right bracket `]`.
    LBRACE,      ///< Left curly brace `{` for Set definition.
    RBRACE,      ///< Right curly brace `}`.

    // ━━ Control States ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    
    EOF_TOKEN,   ///< End Of File/String. Signals the Parser to finish tree construction.
    UNKNOWN      ///< Fallback for invalid characters. Triggers a Lexer error.
};

/**
 * @struct Token
 * @brief Represents a single atomic unit of the user's mathematical expression.
 * @details Tokens act as the transport vehicle between the Lexer and Parser. 
 * They encapsulate the raw user input, its categorized type, and spatial metadata 
 * used to generate highly accurate error messages if parsing fails.
 */
struct Token {
    
    TokenType type;        ///< The categorical type of the token (e.g., NUMBER, OPERATOR).
    std::string lexeme;    ///< The exact raw string extracted from the user's input.
    Value literal;         ///< The parsed numerical value (populated if type is NUMBER).

    int line;              ///< The line number where the token appeared (for multi-line input).
    int column;            ///< The character index where the token starts (for precise error pointers).

    /**
     * @brief Constructs a fully populated Token.
     * @param t The TokenType classification.
     * @param lex The raw string representation.
     * @param val The encapsulated Value (defaults to a generic double if not a literal).
     * @param l The line number for error tracking.
     * @param c The column index for error tracking.
     */
    Token(TokenType t, std::string lex, Value val, int l, int c)
        : type(t), lexeme(std::move(lex)), literal(std::move(val)), line(l), column(c) {}
};