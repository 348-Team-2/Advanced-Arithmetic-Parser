/**
* @file registry.cpp
 * @brief Implementation of the Registry class methods.
 * @details This file defines the behavior for storing, querying, and executing
 * mathematical operations, functions, and constants. It relies on `std::unordered_map`
 * lookups to provide O(1) average time complexity for the Lexer, Parser, and Evaluator.
 */

#include "reg/registry.hpp"

void Registry::register_operator(const OperatorStruct& def)
{
    operators[def.symbol] = def;
};

void Registry::register_constant(const ConstantStruct& def) {
    constants[def.name] = def;
}

void Registry::register_function(const FunctionStruct& def) {
    functions[def.name] = def;
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

bool Registry::is_operator(const std::string& symbol) const {
    // Direct check (for things like + or **)
    if (operators.contains(symbol)) return true; // TODO: EXPLAIN THIS IN DOCS

    // Unary prefix check (is there a 'u' version?)
    if (operators.contains("u" + symbol)) return true;

    // Postfix check (is there a 'p' version?)
    if (operators.contains("p" + symbol)) return true;

    return false;
}

bool Registry::is_const(const std::string& name) const
{
    return constants.contains(name);
}

bool Registry::is_funct(const std::string& name) const
{
    return functions.contains(name);
}

bool Registry::is_binary(const std::string& symbol) const {
    // An operator is binary if it exists AND its binary std::function contains a target
    if (const auto it = operators.find(symbol); it != operators.end()) {
        return static_cast<bool>(it->second.evaluate_binary);
    }

    return false;
}

bool Registry::is_unary(const std::string& symbol) const {
    // An operator is unary if it exists AND the boolean flag is true
    if (const auto it = operators.find(symbol); it != operators.end()) {
        return it->second.is_unary;
    }

    return false;
}

int Registry::get_precedence(const std::string& symbol) const {
    const auto it = operators.find(symbol);
    if (it == operators.end()) {
        throw std::invalid_argument("Syntax Error: Unknown operator '" + symbol + "'");
    }
    return it->second.precedence;
}

Associativity Registry::get_associativity(const std::string& symbol) const {
    const auto it = operators.find(symbol);
    if (it == operators.end()) {
        throw std::invalid_argument("Syntax Error: Unknown operator '" + symbol + "'");
    }
    return it->second.associativity;
}

 // ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Value Registry::evaluate_binary(const std::string& symbol, const Value& left, const Value& right) const {
    const auto it = operators.find(symbol);

    if (it == operators.end()) {
        throw std::invalid_argument("Evaluation Error: Unknown binary operator '" + symbol + "'");
    }

    // Ensure the Parser didn't accidentally pass a unary operator here.
    if (it->second.is_unary && !it->second.evaluate_binary) {
         throw std::invalid_argument("Evaluation Error: Attempted to execute unary operator '" + symbol + "' as binary.");
    }

    return it->second.evaluate_binary(left, right);
}

// Changed parameter to const Value& and return type to Value
Value Registry::evaluate_unary(const std::string& symbol, const Value& operand) const {
    const auto it = operators.find(symbol);

    if (it == operators.end()) {
        throw std::invalid_argument("Evaluation Error: Unknown unary operator '" + symbol + "'");
    }

    if (!it->second.is_unary) {
        throw std::invalid_argument("Evaluation Error: Attempted to execute binary operator '" + symbol + "' as unary.");
    }

    return it->second.evaluate_unary(operand);
}

Value Registry::evaluate_function(const std::string& name, const std::vector<Value>& args) const {
    const auto it = functions.find(name);

    if (it == functions.end()) {
        throw std::invalid_argument("Evaluation Error: Unknown function '" + name + "'");
    }

    // Strict enforcement of arity before executing the lambda.
    if (args.size() != static_cast<size_t>(it->second.expected_args)) {
        throw std::invalid_argument("Evaluation Error: Function '" + name + "' expected " +
                                    std::to_string(it->second.expected_args) + " arguments.");
    }

    return it->second.evaluate(args);
}

Value Registry::get_constant(const std::string& name) const {
    const auto it = constants.find(name);

    if (it == constants.end()) {
        throw std::invalid_argument("Evaluation Error: Unknown constant '" + name + "'");
    }

    return it->second.value;
}