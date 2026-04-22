#include "registry.hpp"

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
    return operators.contains(symbol);
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

// Changed vector type to Value and return type to Value
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

// Changed return type to Value
Value Registry::get_constant(const std::string& name) const {
    const auto it = constants.find(name);

    if (it == constants.end()) {
        throw std::invalid_argument("Evaluation Error: Unknown constant '" + name + "'");
    }

    return it->second.value;
}