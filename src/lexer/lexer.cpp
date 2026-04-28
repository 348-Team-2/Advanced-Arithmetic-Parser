#include "lexer/lexer.hpp"

#include <cctype>
#include <string>

Lexer::Lexer(const Registry& reg) : registry(reg) {}

std::vector<Token> Lexer::tokenize(const std::string& input) {
    source = input;
    tokens.clear();
    start = 0;
    current = 0;
    line = 1;
    column_start = 1;

    while (!is_at_end()) {
        start = current;
        column_start = current + 1; // 1-indexed column tracking
        scan_token();
    }

    // Append EOF token to signal the Parser that scanning is complete
    tokens.emplace_back(TokenType::EOF_TOKEN, "EOF", Value(), line, current + 1);

    return tokens;
}

void Lexer::scan_token() {
    skip_whitespace();
    if (is_at_end()) return;

    start = current;
    column_start = current + 1;
    const char c = advance();

    // ━━ Structural Delimiters ━━
    if (c == '(') { add_token(TokenType::LPAREN); return; }
    if (c == ')') { add_token(TokenType::RPAREN); return; }
    if (c == ',') { add_token(TokenType::COMMA); return; }

    // ━━ Dynamic Operator Matching ━━
    if (is_symbol(c)) {
        process_operator();
        return;
    }

    // ━━ Numbers & Identifiers ━━
    if (std::isdigit(c) || (c == '.' && std::isdigit(peek()))) {
        process_number();
    } else if (std::isalpha(c)) {
        process_identifier();
    } else {
        // Fallback for unknown characters
        add_token(TokenType::UNKNOWN);
    }
}

// ━━ Subroutines ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

bool Lexer::is_symbol(const char c)
{
    // Defines characters that are allowed to form operators
    return std::string("+-*/%=<>!|&^~.").find(c) != std::string::npos;
}

void Lexer::process_operator() {
    // Group all contiguous symbol characters together
    while (is_symbol(peek())) {
        advance();
    }

    std::string text = source.substr(start, current - start);

    // Maximal Munch Algorithm:
    // Shrink the string from the right until a registered operator is found
    while (!text.empty()) {
        if (registry.is_operator(text)) {
            // Match found! Backtrack the Lexer to the end of the matched operator
            current = start + text.length();
            add_token(TokenType::OPERATOR);
            return;
        }
        text.pop_back(); // Remove the last character and try again
    }

    // Fallback if the user typed a symbol character that isn't registered at all
    current = start + 1;
    add_token(TokenType::UNKNOWN);
}

void Lexer::process_number() {
    // Consume integer part
    while (std::isdigit(peek())) advance();

    // Handle fractional part
    if (peek() == '.' && std::isdigit(peek_next())) {
        advance(); // Consume the '.'
        while (std::isdigit(peek())) advance();
    }

    const std::string num_str = source.substr(start, current - start);
    double val = std::stod(num_str);

    add_token(TokenType::NUMBER, Value(val));
}

void Lexer::process_identifier() {
    // Consume alphanumeric characters for functions/constants/variables
    while (std::isalnum(peek())) advance();

    // Dynamically check the Registry to classify the text
    if (const std::string text = source.substr(start, current - start); registry.is_funct(text)) {
        add_token(TokenType::FUNCTION);
    } else if (registry.is_const(text)) {
        // Retrieve the mathematical value of the constant from the Registry
        const Value const_val = registry.get_constant(text);
        add_token(TokenType::IDENTIFIER, const_val);
    } else {
        // If not registered, treat as a generic identifier/variable
        add_token(TokenType::IDENTIFIER);
    }
}

void Lexer::add_token(TokenType type, const Value& literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line, column_start);
}

// ━━ Internal Navigation Helpers ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

bool Lexer::is_at_end() const {
    return current >= source.length(); 
}

char Lexer::advance() {
    return source[current++];
}

char Lexer::peek() const {
    if (is_at_end()) return '\0';
    return source[current];
}

char Lexer::peek_next() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Lexer::skip_whitespace() {
    while (!is_at_end()) {
        if (const char c = peek(); c == ' ' || c == '\r' || c == '\t') {
            advance();
        } else if (c == '\n') {
            line++;
            advance();
        } else {
            break;
        }
    }
}