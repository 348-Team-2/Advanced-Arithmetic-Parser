#include "eval/evaluator.hpp"

// ━━ PUBLIC ENTRY POINT ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Value Evaluator::evaluate(const ASTNode& node) {
    // Dispatch to the correct handler based on the node's runtime type.
    if (const auto* n = dynamic_cast<const LiteralNode*>(&node))
        return eval_literal(*n);

    if (const auto* n = dynamic_cast<const BinaryOpNode*>(&node))
        return eval_binary(*n);

    if (const auto* n = dynamic_cast<const UnaryOpNode*>(&node))
        return eval_unary(*n);

    if (const auto* n = dynamic_cast<const FunctionCallNode*>(&node))
        return eval_function(*n);

    throw EvalError("Unknown AST node type encountered during evaluation.");
}

// ━━ NODE HANDLERS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Value Evaluator::eval_literal(const LiteralNode& node) {
    // Base case: the Value was already resolved by the Lexer or Parser.
    return node.value;
}

Value Evaluator::eval_binary(const BinaryOpNode& node) {
    // Post-order: evaluate both subtrees before applying the operator.
    const Value lhs = evaluate(*node.left);
    const Value rhs = evaluate(*node.right);

    try {
        return registry.evaluate_binary(node.op, lhs, rhs);
    } catch (const std::runtime_error& e) {
        throw EvalError(e.what()); // Re-wrap Registry errors (e.g., division by zero).
    }
}

Value Evaluator::eval_unary(const UnaryOpNode& node) {
    const Value operand = evaluate(*node.operand);

    try {
        return registry.evaluate_unary(node.op, operand);
    } catch (const std::runtime_error& e) {
        throw EvalError(e.what());
    }
}

Value Evaluator::eval_function(const FunctionCallNode& node) {
    // Evaluate each argument independently before passing to the Registry.
    std::vector<Value> args;
    args.reserve(node.args.size());

    for (const auto& arg : node.args) {
        args.push_back(evaluate(*arg));
    }

    try {
        return registry.evaluate_function(node.name, args);
    } catch (const std::runtime_error& e) {
        throw EvalError(e.what()); // Catches arity mismatches and domain errors.
    }
}