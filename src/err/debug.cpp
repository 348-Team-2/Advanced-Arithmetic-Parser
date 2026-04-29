#include "err/debug.hpp"

#include <sstream>
#include <iomanip>

// ━━ TOKEN STREAM PRINTER ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

std::string token_type_to_string(const TokenType type) {
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

// ━━ AST PRINTER ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// Helper to stringify the Value variant for the LiteralNode
std::string format_ast_value(const Value& val) {
    if (std::holds_alternative<double>(val)) {
        double d = std::get<double>(val);
        // Clean up trailing zeros for whole numbers
        if (d == std::floor(d)) {
            return std::to_string(static_cast<long long>(d));
        }
        return std::to_string(d);
    }
    if (std::holds_alternative<std::complex<double>>(val)) {
        auto comp = std::get<std::complex<double>>(val);
        return std::to_string(comp.real()) + " + " + std::to_string(comp.imag()) + "i";
    }
    // Expand this later if we want vectors/matrices to print inline
    return "[Complex Type]";
}

// Recursive function to build the ASCII tree
void build_ast_string(const ASTNode* node, const std::string& prefix, bool is_last, bool is_root, std::ostringstream& oss) {
    if (!node) return;

    // Print the branch prefix (unless it's the very top of the tree)
    oss << prefix;
    if (!is_root) {
        // Standard ASCII branch rendering
        oss << (is_last ? "\\-- " : "+-- ");
    }

    // Calculate the prefix for the children of THIS node
    // Using a standard pipe '|' for vertical lines
    std::string next_prefix = prefix + (is_root ? "" : (is_last ? "    " : "|   "));

    // ━━ DETERMINE NODE TYPE & RECURSE ━━
    if (const auto* bin = dynamic_cast<const BinaryOpNode*>(node)) {
        oss << "BinaryOp: " << bin->op << "\n";
        build_ast_string(bin->left.get(), next_prefix, false, false, oss);
        build_ast_string(bin->right.get(), next_prefix, true, false, oss);

    } else if (const auto* un = dynamic_cast<const UnaryOpNode*>(node)) {
        oss << "UnaryOp: " << un->op << "\n";
        // Unary only has one child, so it's always the 'last' branch
        build_ast_string(un->operand.get(), next_prefix, true, false, oss);

    } else if (const auto* lit = dynamic_cast<const LiteralNode*>(node)) {
        oss << "Literal: " << format_ast_value(lit->value) << "\n";
        // Literals have no children, so we don't recurse further.

    } else if (const auto* func = dynamic_cast<const FunctionCallNode*>(node)) {
        oss << "Function: " << func->name << "()\n";
        for (size_t i = 0; i < func->args.size(); ++i) {
            bool is_last_arg = (i == func->args.size() - 1);
            build_ast_string(func->args[i].get(), next_prefix, is_last_arg, false, oss);
        }
    } else {
        oss << "Unknown Node Type\n";
    }
}

void print_ast(const ASTNode* root) {
    if (!root) {
        CLI::print("AST is empty (nullptr).");
        return;
    }

    std::ostringstream oss;
    oss << "\n================= ABSTRACT SYNTAX TREE =================\n";

    // Kick off the recursion
    build_ast_string(root, "", true, true, oss);

    oss << "========================================================\n\n";

    // Pass the fully built string to the CLI module
    CLI::print(oss.str());
}

// ━━ GRAPHVIZ STYLE AST PRINTER ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// Helper function to generate Graphviz DOT syntax
void build_dot_string(const ASTNode* node, std::ostringstream& oss) {
    if (!node) return;

    // Use the memory address of the pointer as a unique ID for the node
    std::string node_id = "node" + std::to_string(reinterpret_cast<uintptr_t>(node));

    // Define the node's label based on its type
    if (const auto* bin = dynamic_cast<const BinaryOpNode*>(node)) {
        oss << "    " << node_id << " [label=\"Op: " << bin->op << "\"];\n";

        if (bin->left) {
            std::string left_id = "node" + std::to_string(reinterpret_cast<uintptr_t>(bin->left.get()));
            oss << "    " << node_id << " -> " << left_id << ";\n";
            build_dot_string(bin->left.get(), oss);
        }
        if (bin->right) {
            std::string right_id = "node" + std::to_string(reinterpret_cast<uintptr_t>(bin->right.get()));
            oss << "    " << node_id << " -> " << right_id << ";\n";
            build_dot_string(bin->right.get(), oss);
        }

    } else if (const auto* un = dynamic_cast<const UnaryOpNode*>(node)) {
        oss << "    " << node_id << " [label=\"Unary: " << un->op << "\"];\n";
        if (un->operand) {
            std::string op_id = "node" + std::to_string(reinterpret_cast<uintptr_t>(un->operand.get()));
            oss << "    " << node_id << " -> " << op_id << ";\n";
            build_dot_string(un->operand.get(), oss);
        }

    } else if (const auto* lit = dynamic_cast<const LiteralNode*>(node)) {
        oss << "    " << node_id << " [label=\"Val: " << format_ast_value(lit->value) << "\", shape=box];\n";

    } else if (const auto* func = dynamic_cast<const FunctionCallNode*>(node)) {
        oss << "    " << node_id << " [label=\"Func: " << func->name << "()\"];\n";
        for (const auto& arg : func->args) {
            if (arg) {
                std::string arg_id = "node" + std::to_string(reinterpret_cast<uintptr_t>(arg.get()));
                oss << "    " << node_id << " -> " << arg_id << ";\n";
                build_dot_string(arg.get(), oss);
            }
        }
    }
}

// The public function you can call from Engine
void print_ast_graphviz(const ASTNode* root) {
    if (!root) return;

    std::ostringstream oss;
    oss << "digraph AST {\n";
    oss << "    node [fontname=\"Helvetica,Arial\", shape=ellipse];\n";

    build_dot_string(root, oss);

    oss << "}\n";

    CLI::print("Copy the following into a Graphviz visualizer:\n");
    CLI::print(oss.str());
}