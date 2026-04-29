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
    /**
     * @brief Virtual destructor for safe polymorphic cleanup.
     */
    virtual ~ASTNode() = default;

    /**
     * @brief Serializes the AST subtree into a flat list.
     * @details Primarily used for debugging, logging, or generating a LISP-style
     * postfix representation of the expression tree.
     * @return A vector of strings representing the tree structure.
     */
    [[nodiscard]] virtual std::vector<std::string> to_list() const = 0;
};

// ━━ AST NODE TYPES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @struct LiteralNode
 * @brief Represents a raw numeric or mathematically constant value.
 */
struct LiteralNode final : public ASTNode {
    Value value; ///< The underlying concrete value (e.g., a double or string).

    /**
     * @brief Constructs a LiteralNode with a specific value.
     * @param v The variant value to be stored in this node.
     */
    explicit LiteralNode(Value v) : value(std::move(v)) {}

    /**
     * @brief Serializes the literal value into a string.
     * @details If the value is a floating-point number representing a whole integer,
     * it strips the decimal portion for cleaner output.
     * @return A single-element vector containing the string representation of the value.
     */
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

/**
 * @struct BinaryOpNode
 * @brief Represents an operation with a Left-Hand Side and Right-Hand Side.
 */
struct BinaryOpNode final : public ASTNode {
    std::string op;                  ///< The string representation of the binary operator (e.g., "+", "*").
    std::unique_ptr<ASTNode> left;   ///< The left-hand operand expression.
    std::unique_ptr<ASTNode> right;  ///< The right-hand operand expression.

    /**
     * @brief Constructs a BinaryOpNode.
     * @param o The binary operator string.
     * @param l A unique pointer to the left child node.
     * @param r A unique pointer to the right child node.
     */
    BinaryOpNode(std::string o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}

    /**
     * @brief Serializes the binary operation in postfix notation.
     * @return A vector of strings structured as `[left_expr, right_expr, operator]`.
     */
    [[nodiscard]] std::vector<std::string> to_list() const override {
        auto l = left->to_list();
        auto r = right->to_list();
        l.insert(l.end(), r.begin(), r.end());
        l.push_back(op);
        return l;
    }
};

/**
 * @struct UnaryOpNode
 * @brief Represents an operation requiring only a single operand (Prefix or Postfix).
 */
struct UnaryOpNode final : public ASTNode {
    std::string op;                    ///< The string representation of the unary operator (e.g., "u-", "p!").
    std::unique_ptr<ASTNode> operand;  ///< The target operand expression.

    /**
     * @brief Constructs a UnaryOpNode.
     * @param o The unary operator string.
     * @param opnd A unique pointer to the operand node.
     */
    UnaryOpNode(std::string o, std::unique_ptr<ASTNode> opnd)
        : op(std::move(o)), operand(std::move(opnd)) {}

    /**
     * @brief Serializes the unary operation in postfix notation.
     * @return A vector of strings structured as `[operand_expr, operator]`.
     */
    [[nodiscard]] std::vector<std::string> to_list() const override {
        auto o = operand->to_list();
        o.push_back(op);
        return o;
    }
};

/**
 * @struct FunctionCallNode
 * @brief Represents an n-ary mathematical function call (e.g., max, sin).
 */
struct FunctionCallNode final : public ASTNode {
    std::string name;                           ///< The name of the function being called.
    std::vector<std::unique_ptr<ASTNode>> args; ///< A list of argument expressions provided to the function.

    /**
     * @brief Constructs a FunctionCallNode.
     * @param n The string identifier for the function.
     * @param a A vector of unique pointers representing the function's arguments.
     */
    FunctionCallNode(std::string n, std::vector<std::unique_ptr<ASTNode>> a)
        : name(std::move(n)), args(std::move(a)) {}

    /**
     * @brief Serializes the function call and its arguments in postfix notation.
     * @return A vector of strings structured as `[arg1_expr, arg2_expr, ..., function_name]`.
     */
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