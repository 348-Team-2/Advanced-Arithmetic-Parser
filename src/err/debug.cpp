#include "err/debug.hpp"

#include <sstream>
#include <iomanip>

std::string token_type_to_string(const TokenType type) {
    // ... (This function remains exactly the same as before) ...
    switch (type) {
        case TokenType::NUMBER:      return "NUMBER";
        case TokenType::IDENTIFIER:  return "IDENTIFIER";
        case TokenType::OPERATOR:    return "OPERATOR";
        case TokenType::FUNCTION:    return "FUNCTION";
        case TokenType::LPAREN:      return "LPAREN";
        case TokenType::RPAREN:      return "RPAREN";
        case TokenType::COMMA:       return "COMMA";
        case TokenType::LBRACKET:    return "LBRACKET";
        case TokenType::RBRACKET:    return "RBRACKET";
        case TokenType::LBRACE:      return "LBRACE";
        case TokenType::RBRACE:      return "RBRACE";
        case TokenType::EOF_TOKEN:   return "EOF";
        case TokenType::UNKNOWN:     return "UNKNOWN";
        default:                     return "UNDEFINED";
    }
}

void print_token_stream(const std::vector<Token>& tokens) {
    std::ostringstream oss; // Build the string in memory

    oss << "\n======================= TOKEN STREAM =======================\n";
    oss << std::left
        << std::setw(10) << "Pos [L:C]"
        << std::setw(15) << "Type"
        << std::setw(15) << "Lexeme"
        << "Extracted Value\n";
    oss << "------------------------------------------------------------\n";

    for (const auto& t : tokens) {
        std::string position = "[" + std::to_string(t.line) + ":" + std::to_string(t.column) + "]";
        oss << std::left << std::setw(10) << position;
        oss << std::setw(15) << token_type_to_string(t.type);
        oss << std::setw(15) << ("'" + t.lexeme + "'");

        if (t.type == TokenType::NUMBER || (t.type == TokenType::IDENTIFIER && std::holds_alternative<double>(t.literal))) {
            if (std::holds_alternative<double>(t.literal)) {
                oss << std::get<double>(t.literal);
            }
            else if (std::holds_alternative<std::complex<double>>(t.literal)) {
                auto comp = std::get<std::complex<double>>(t.literal);
                oss << comp.real() << " + " << comp.imag() << "i";
            }
        }

        oss << "\n";
    }
    oss << "============================================================\n\n";

    // Pass the fully built string to CLI module to handle the actual printing
    CLI::print(oss.str());
}