/**
 * @file def_ast.hpp
 * @brief Defines the pure data structures for the Abstract Syntax Tree (AST).
 * @details These nodes simply represent the grammar of the parsed mathematical
 * expression. They contain no execution logic, leaving evaluation to a dedicated
 * Evaluator or Compiler module.
 */

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cmath>

#include "../reg/def_value.hpp"

// ━━ AST BASE CLASS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @struct ASTNode
 * @brief The polymorphic base class for all nodes in the AST.
 */
struct ASTNode {
    virtual ~ASTNode() = default;

    /**
     * @brief Serializes the AST into a flat list (useful for debugging/LISP-style output).
     * @return A vector of strings representing the tree structure.
     */
    virtual std::vector<std::string> to_list() const = 0;
};

// ━━ AST NODE TYPES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/// @brief Represents a raw numeric or mathematically constant value.
struct LiteralNode final : public ASTNode {
    Value value;

    explicit LiteralNode(Value v) : value(std::move(v)) {}

    [[nodiscard]] std::vector<std::string> to_list() const override {
        if (std::holds_alternative<double>(value)) {
            const double d = std::get<double>(value);
            if (d == std::floor(d)) {
                return {std::to_string(static_cast<long long>(d))};
            }
            return {std::to_string(d)};
        }
        return {"[literal]"};
    }
};

/// @brief Represents an operation with a Left-Hand Side and Right-Hand Side.
struct BinaryOpNode final : public ASTNode {
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(std::string o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}

    [[nodiscard]] std::vector<std::string> to_list() const override {
        auto l = left->to_list();
        auto r = right->to_list();
        l.insert(l.end(), r.begin(), r.end());
        l.push_back(op);
        return l;
    }
};

/// @brief Represents an operation requiring only a single operand (Prefix or Postfix).
struct UnaryOpNode final : public ASTNode {
    std::string op;
    std::unique_ptr<ASTNode> operand;

    UnaryOpNode(std::string o, std::unique_ptr<ASTNode> opnd)
        : op(std::move(o)), operand(std::move(opnd)) {}

    [[nodiscard]] std::vector<std::string> to_list() const override {
        auto o = operand->to_list();
        o.push_back(op);
        return o;
    }
};

/// @brief Represents an n-ary mathematical function call (e.g., max, sin).
struct FunctionCallNode final : public ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> args;

    FunctionCallNode(std::string n, std::vector<std::unique_ptr<ASTNode>> a)
        : name(std::move(n)), args(std::move(a)) {}

    [[nodiscard]] std::vector<std::string> to_list() const override {
        std::vector<std::string> result;
        for (const auto& arg : args) {
            auto arg_list = arg->to_list();
            result.insert(result.end(), arg_list.begin(), arg_list.end());
        }
        result.push_back(name);
        return result;
    }
};