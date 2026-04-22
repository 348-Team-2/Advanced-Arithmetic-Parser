/**
* @file registry.hpp
 * @brief Defines the Registry class which acts as the central repository for language rules.
 * @details This header provides the interface for the Lexer, Parser, and Evaluator to
 * interact with the mathematical engine. It manages the lifecycle and access of
 * operators, functions, and constants.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <vector>

#include "def_const.hpp"
#include "def_funct.hpp"
#include "def_operator.hpp"
#include "def_value.hpp"

/**
 * @class Registry
 * @brief Centralized storage and execution engine for all language rules and mathematical operations.
 * @details The Registry follows a Singleton-like access pattern for the parser pipeline.
 * It maps string symbols (e.g., "+", "sin", "pi") to their respective precedence rules,
 * associativity, and executable lambda functions.
 */
class Registry {

private:
    std::unordered_map<std::string, OperatorStruct> operators; ///< Hash map of supported mathematical operators.
    std::unordered_map<std::string, ConstantStruct> constants; ///< Hash map of built-in mathematical constants.
    std::unordered_map<std::string, FunctionStruct> functions; ///< Hash map of standard math functions.

public:
    Registry() = default;
    ~Registry() = default;

    /**
     * @name Registration Methods
     * Methods used during system initialization to populate the dictionaries.
     */
    ///@{

    /**
     * @brief Loads a new operator definition into the system memory.
     * @param def The populated OperatorStruct containing precedence, associativity, and execution logic.
     */
    void register_operator(const OperatorStruct& def);

    /**
     * @brief Loads a built-in mathematical constant into the system memory.
     * @param def The populated ConstantStruct containing the immutable value.
     */
    void register_constant(const ConstantStruct& def);

    /**
     * @brief Loads a standard mathematical function into the system memory.
     * @param def The populated FunctionStruct dictating arity and execution logic.
     */
    void register_function(const FunctionStruct& def);

    ///@}


    /**
     * @name Parsing Methods
     * Methods utilized by the Lexer and Parser to build the Abstract Syntax Tree.
     */
    ///@{

    /**
     * @brief Checks if a parsed token string matches any registered operator.
     * @param symbol The raw string from the token (e.g., "+", "**").
     * @return true if the operator exists in the registry, false otherwise.
     */
    bool is_operator(const std::string& symbol) const;

    /**
     * @brief Retrieves the priority level of an operator to determine evaluation order.
     * @param symbol The operator string to query.
     * @return An integer representing precedence (higher evaluates first).
     * @throws std::invalid_argument If the requested operator symbol is not registered.
     */
    int get_precedence(const std::string& symbol) const;

    /**
     * @brief Retrieves the binding direction to handle precedence ties.
     * @param symbol The operator string to query.
     * @return The Associativity enum value (LEFT, RIGHT, NONE).
     * @throws std::invalid_argument If the requested operator symbol is not registered.
     */
    Associativity get_associativity(const std::string& symbol) const;

    ///@}


    /**
     * @name Evaluation Methods
     * Methods utilized by the Evaluator to compute actual mathematical results.
     */
    ///@{

    /**
     * @brief Executes the stored mathematical logic for a specific binary operator.
     * @param symbol The operator string dictating the math logic (e.g., "+").
     * @param left The evaluated Left-Hand Side (LHS) operand.
     * @param right The evaluated Right-Hand Side (RHS) operand.
     * @return The calculated result encapsulated in a Value type.
     * @throws std::invalid_argument If the symbol is not registered or is strictly unary.
     * @throws std::runtime_error If the math reaches an invalid state (e.g., mismatched types, division by zero).
     */
    Value evaluate_binary(const std::string& symbol, const Value& left, const Value& right) const;

    /**
     * @brief Executes the stored mathematical logic for a specific unary operator.
     * @param symbol The operator string dictating the math logic (e.g., "u-").
     * @param operand The single evaluated operand.
     * @return The calculated result encapsulated in a Value type.
     * @throws std::invalid_argument If the operator symbol is not registered or isn't flagged as unary.
     * @throws std::runtime_error If the math reaches an invalid state.
     */
    Value evaluate_unary(const std::string& symbol, const Value& operand) const;

    /**
     * @brief Executes the stored mathematical logic for an n-ary function.
     * @param name The function string identifier (e.g., "sin", "max").
     * @param args A vector of evaluated numeric arguments.
     * @return The calculated result encapsulated in a Value type.
     * @throws std::invalid_argument If the function is unknown or the argument count (arity) is incorrect.
     * @throws std::runtime_error If the math reaches an invalid state (e.g., domain errors).
     */
    Value evaluate_function(const std::string& name, const std::vector<Value>& args) const;

    /**
     * @brief Retrieves the numeric value of a stored mathematical constant.
     * @param name The string identifier of the constant (e.g., "pi").
     * @return The precise value encapsulated in a Value type.
     * @throws std::invalid_argument If the constant name is not registered.
     */
    Value get_constant(const std::string& name) const;

    ///@}
};