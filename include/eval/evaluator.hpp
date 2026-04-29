/**
 * @file evaluator.hpp
 * @brief Defines the Evaluator class and its associated EvalError exception.
 * @details The Evaluator is the final stage of the pipeline. It performs a recursive
 * post-order traversal of the AST, delegating all mathematical execution to the
 * Registry to produce a single Value result.
 */
#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "parse/def_ast.hpp"
#include "reg/registry.hpp"
#include "reg/def_value.hpp"

// ━━ EVALUATOR EXCEPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @class EvalError
 * @brief Custom exception thrown when the Evaluator encounters an invalid state.
 * @details Mirrors ParseError's design. Registry math errors (e.g., division by zero,
 * domain violations) are caught and re-thrown as EvalErrors so the CLI handles one
 * exception type per pipeline stage.
 */
class EvalError : public std::runtime_error {
public:
    /**
     * @brief Constructs an EvalError with a descriptive failure message.
     * @param message A human-readable explanation of what went wrong.
     */
    explicit EvalError(const std::string& message)
        : std::runtime_error(message) {}
};

// ━━ EVALUATOR ENGINE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @class Evaluator
 * @brief Recursively traverses the AST and computes a final Value result.
 * @details All mathematical logic is delegated to the Registry. The Evaluator
 * contains no hardcoded arithmetic — it is purely a tree walker.
 */
class Evaluator
{
private:
    Registry& registry; ///< Non-owning reference to the active math Registry.

public:
    /**
     * @brief Constructs an Evaluator bound to an active Registry instance.
     * @param r The same Registry instance used by the Lexer and Parser.
     */
    explicit Evaluator(Registry& r) : registry(r) {}

    /**
     * @brief Evaluates the full AST rooted at the given node.
     * @param node The root ASTNode returned by Parser::parse().
     * @return The computed result as a Value.
     * @throws EvalError If a node type is unrecognized or the Registry reports an invalid state.
     */
    Value evaluate(const ASTNode& node);

private:
    /**
     * @brief Extracts the immediate value from a literal leaf node.
     * @param node The AST node containing a raw numeric or boolean value.
     * @return The literal value stored within the node.
     */
    Value eval_literal(const LiteralNode& node);

    /**
     * @brief Evaluates a binary operation by visiting left and right children.
     * @details Recursively evaluates the operands, then queries the Registry
     * to perform the operation (e.g., +, -, *, /) on the resulting Values.
     * @param node The AST node representing the binary operator and its operands.
     * @return The result of the operation.
     * @throws EvalError If the Registry does not recognize the operator symbol.
     */
    Value eval_binary(const BinaryOpNode& node);

    /**
     * @brief Evaluates a unary operation (e.g., negation) on a single child node.
     * @details Recursively evaluates the single operand and delegates the
     * transformation to the Registry.
     * @param node The AST node representing the unary operator and its operand.
     * @return The result of the unary operation.
     * @throws EvalError If the operator is undefined or the operand type is invalid.
     */
    Value eval_unary(const UnaryOpNode& node);

    /**
     * @brief Evaluates a function call by processing its argument list.
     * @details Performs an evaluation on every argument in the node's vector
     * of children, then passes the resulting vector of Values to the Registry
     * to execute the named function.
     * @param node The AST node containing the function name and argument nodes.
     * @return The Value returned by the registered function.
     * @throws EvalError If the function name is unknown or the argument count is incorrect.
     */
    Value eval_function(const FunctionCallNode& node);
};