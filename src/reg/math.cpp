/**
* @file math.cpp
 * @brief Defines and loads all default mathematical operations, functions, and constants.
 * @details This file acts as the standard library for the CLI calculator.
 * It isolates all the actual computational logic (using `<cmath>`) from the parsing
 * pipeline, registering lambdas into the central Registry.
 */

#include "reg/registry.hpp"

#include <cmath>
#include <stdexcept>
#include <cstdint>
#include <numbers>
#include <algorithm>

/**
 * @brief Instructions for adding operations:
 *
 * 1. **Check the types**
 * @code
 * if (std::holds_alternative<Vector>(a) && std::holds_alternative<Vector>(b)) { ... }
 * @endcode
 *
 * 2. **Extract the data**
 * @code
 * const auto& vecA = std::get<Vector>(a);
 * @endcode
 *
 * 3. **Perform the math**
 * Compute the result and return the appropriate `Value`
 * (e.g., `Vector`, `Matrix`, `double`, etc.).
 *
 * 4. **Fallback handling**
 * Always leave the `throw std::runtime_error` at the bottom as a fallback
 * for unsupported type combinations (for example, attempting to add a
 * `Set` to a `Matrix`).
 */

void load_standard_math(Registry& registry) {

    // ━━ LOGICAL OR ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "||", 1, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) != 0.0 || std::get<double>(b) != 0.0) ? 1.0 : 0.0;
            }
            // TODO: Implement element-wise logical OR for Tensors/Vectors (Broadcasting)
            throw std::runtime_error("Type Error: || requires scalar doubles.");
        }, nullptr
    });

    // ━━ LOGICAL AND ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "&&", 2, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) != 0.0 && std::get<double>(b) != 0.0) ? 1.0 : 0.0;
            }
            // TODO: Implement element-wise logical AND for Tensors/Vectors
            throw std::runtime_error("Type Error: && requires scalar doubles.");
        }, nullptr
    });

    // ━━ BITWISE OR ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "|", 3, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return static_cast<double>(static_cast<int64_t>(std::get<double>(a)) | static_cast<int64_t>(std::get<double>(b)));
            }
            // TODO: Implement set union if a and b are Sets
            throw std::runtime_error("Type Error: | requires scalar doubles or Sets.");
        }, nullptr
    });

    // ━━ BITWISE XOR ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "^", 4, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return static_cast<double>(static_cast<int64_t>(std::get<double>(a)) ^ static_cast<int64_t>(std::get<double>(b)));
            }
            // TODO: Implement symmetric difference for Sets
            throw std::runtime_error("Type Error: ^ requires scalar doubles or Sets.");
        }, nullptr
    });

    // ━━ BITWISE AND ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "&", 5, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return static_cast<double>(static_cast<int64_t>(std::get<double>(a)) & static_cast<int64_t>(std::get<double>(b)));
            }
            // TODO: Implement set intersection if a and b are Sets
            throw std::runtime_error("Type Error: & requires scalar doubles or Sets.");
        }, nullptr
    });

    // ━━ EQUALITY / INEQUALITY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "==", 6, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) == std::get<double>(b)) ? 1.0 : 0.0;
            }
            // TODO: Implement Matrix == Matrix or Vector == Vector deep comparisons
            throw std::runtime_error("Type Error: == requires matching types.");
        }, nullptr
    });

    registry.register_operator({
        "!=", 6, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) != std::get<double>(b)) ? 1.0 : 0.0;
            }
            // TODO: Implement deep comparison negations for complex structures
            throw std::runtime_error("Type Error: != requires matching types.");
        }, nullptr
    });

    // ━━ RELATIONAL (COMPARISON) ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "<", 7, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) < std::get<double>(b)) ? 1.0 : 0.0;
            }
            // TODO: Implement subset checks (Set A < Set B)
            throw std::runtime_error("Type Error: < requires scalar doubles or Sets.");
        }, nullptr
    });

    registry.register_operator({
        "<=", 7, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) <= std::get<double>(b)) ? 1.0 : 0.0;
            }
            throw std::runtime_error("Type Error: <= requires scalar doubles.");
        }, nullptr
    });

    registry.register_operator({
        ">", 7, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) > std::get<double>(b)) ? 1.0 : 0.0;
            }
            // TODO: Implement superset checks
            throw std::runtime_error("Type Error: > requires scalar doubles.");
        }, nullptr
    });

    registry.register_operator({
        ">=", 7, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) >= std::get<double>(b)) ? 1.0 : 0.0;
            }
            throw std::runtime_error("Type Error: >= requires scalar doubles.");
        }, nullptr
    });

    // ━━ BITWISE SHIFTS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "<<", 8, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return static_cast<double>(static_cast<int64_t>(std::get<double>(a)) << static_cast<int64_t>(std::get<double>(b)));
            }
            throw std::runtime_error("Type Error: << requires scalar doubles.");
        }, nullptr
    });

    registry.register_operator({
        ">>", 8, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return static_cast<double>(static_cast<int64_t>(std::get<double>(a)) >> static_cast<int64_t>(std::get<double>(b)));
            }
            throw std::runtime_error("Type Error: >> requires scalar doubles.");
        }, nullptr
    });

    // ━━ ADDITION AND SUBTRACTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "+", 9, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return std::get<double>(a) + std::get<double>(b);
            }
            // TODO: Vector + Vector (Element-wise)
            // TODO: Matrix + Matrix (Element-wise)
            // TODO: Scalar + Matrix (Broadcasting)
            throw std::runtime_error("Type Error: Invalid types for +");
        }, nullptr
    });

    registry.register_operator({ "-", 9, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return std::get<double>(a) - std::get<double>(b);
            }
            // TODO: Vector - Vector
            // TODO: Matrix - Matrix
            throw std::runtime_error("Type Error: Invalid types for -");
        }, nullptr
    });

    // ━━ MULTIPLICATION, DIVISION, AND MODULO ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "*", 10, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return std::get<double>(a) * std::get<double>(b);
            }
            // TODO: Scalar * Vector (Scaling)
            // TODO: Scalar * Matrix (Scaling)
            // TODO: Matrix * Matrix (Standard inner-product matrix multiplication)
            // TODO: Vector * Vector (Cross Product for 3D vectors)
            throw std::runtime_error("Type Error: Invalid types for *");
        }, nullptr
    });

    registry.register_operator({ "/", 10, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                double denom = std::get<double>(b);
                if (denom == 0.0) throw std::runtime_error("Division by zero");
                return std::get<double>(a) / denom;
            }
            // TODO: Vector / Scalar (Element-wise division)
            // TODO: Matrix / Scalar (Element-wise division)
            throw std::runtime_error("Type Error: Invalid types for /");
        }, nullptr
    });

    registry.register_operator({ "%", 10, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                double denom = std::get<double>(b);
                if (denom == 0.0) throw std::runtime_error("Modulo by zero");
                return std::fmod(std::get<double>(a), denom);
            }
            throw std::runtime_error("Type Error: Invalid types for %");
        }, nullptr
    });

    registry.register_operator({ "//", 10, Associativity::LEFT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                double denom = std::get<double>(b);
                if (denom == 0.0) throw std::runtime_error("Division by zero");
                return std::trunc(std::get<double>(a) / denom);
            }
            throw std::runtime_error("Type Error: Invalid types for //");
        }, nullptr
    });

    // ━━  TODO: DOT PRODUCT ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // registry.register_operator({ ".", 10, Associativity::LEFT, false,
    //     [](Value a, Value b) -> Value {
    //         // TODO: Vector . Vector (Calculate standard sum-product and return a scalar double)
    //         // TODO: Tensor . Tensor (Generalized contraction)
    //         throw std::runtime_error("Type Error: Operator '.' expects two Vectors or Tensors.");
    //     }, nullptr
    // });

    // ━━ EXPONENTIATION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "**", 11, Associativity::RIGHT, false,
        [](const Value& a, const Value& b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return std::pow(std::get<double>(a), std::get<double>(b));
            }
            // TODO: Matrix ** Scalar (Matrix exponentiation A^n)
            throw std::runtime_error("Type Error: Invalid types for **");
        }, nullptr
    });

    // ━━ PREFIX UNARY OPERATORS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "u+", 12, Associativity::NONE, true, nullptr,
        [](Value a) -> Value {
            if (std::holds_alternative<double>(a)) return std::get<double>(a);
            // TODO: Return Vector/Matrix unchanged
            throw std::runtime_error("Type Error: Invalid type for unary +");
        }
    });

    registry.register_operator({ "u-", 12, Associativity::NONE, true, nullptr,
        [](const Value& a) -> Value {
            if (std::holds_alternative<double>(a)) return -std::get<double>(a);
            // TODO: Negate every component in a Vector/Matrix
            throw std::runtime_error("Type Error: Invalid type for unary -");
        }
    });

    registry.register_operator({ "u~", 12, Associativity::NONE, true, nullptr,
        [](const Value& a) -> Value {
            if (std::holds_alternative<double>(a)) return static_cast<double>(~static_cast<int64_t>(std::get<double>(a)));
            // TODO: Complex Conjugate if 'a' is a complex number
            throw std::runtime_error("Type Error: Invalid type for unary ~");
        }
    });

    registry.register_operator({ "u!", 12, Associativity::NONE, true, nullptr,
        [](const Value& a) -> Value {
            if (std::holds_alternative<double>(a)) return (std::get<double>(a) == 0.0) ? 1.0 : 0.0;
            throw std::runtime_error("Type Error: Invalid type for unary !");
        }
    });

    // ━━ POSTFIX UNARY OPERATORS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "p!", 13, Associativity::NONE, true, nullptr,
        [](const Value& a) -> Value {
            if (std::holds_alternative<double>(a)) {
                const double val = std::get<double>(a);
                if (val < 0.0 && std::floor(val) == val) throw std::runtime_error("Factorial of negative integer is undefined");
                return std::tgamma(val + 1.0);
            }
            throw std::runtime_error("Type Error: Factorial requires a scalar double.");
        }
    });

    registry.register_operator({ "p%", 13, Associativity::NONE, true, nullptr,
        [](const Value& a) -> Value {
            if (std::holds_alternative<double>(a)) return std::get<double>(a) / 100.0;
            throw std::runtime_error("Type Error: Percentage requires a scalar double.");
        }
    });

    // TODO: Add "pT" (Matrix Transpose Postfix Operator)
    // registry.register_operator({ "pT", 13, Associativity::NONE, true, nullptr, [](Value a) -> Value { ... } });

    // ━━ IMPLICATION AND EQUIVALENCE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "->", 1, Associativity::RIGHT, false,
        [](const Value& p, const Value& q) -> Value {
            if (std::holds_alternative<double>(p) && std::holds_alternative<double>(q)) {
                const bool p_bool = (std::get<double>(p) != 0.0);
                const bool q_bool = (std::get<double>(q) != 0.0);
                return (!p_bool || q_bool) ? 1.0 : 0.0;
            }
            throw std::runtime_error("Type Error: -> requires scalar doubles.");
        }, nullptr
    });

    registry.register_operator({ "<->", 1, Associativity::LEFT, false,
        [](const Value& p, const Value& q) -> Value {
            if (std::holds_alternative<double>(p) && std::holds_alternative<double>(q)) {
                return ((std::get<double>(p) != 0.0) == (std::get<double>(q) != 0.0)) ? 1.0 : 0.0;
            }
            throw std::runtime_error("Type Error: <-> requires scalar doubles.");
        }, nullptr
    });
}

void load_functions(Registry& registry)
{
    // ━━ TRIGONOMETRIC FUNCTIONS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_function(FunctionStruct{
        "sin", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                return std::sin(std::get<double>(args[0]));
            }
            // TODO: Apply element-wise for Vectors/Tensors
            // TODO: Matrix Sine (advanced linear algebra, not just element-wise)
            throw std::runtime_error("Type Error: 'sin' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "cos", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                return std::cos(std::get<double>(args[0]));
            }
            // TODO: Apply element-wise for Vectors/Tensors
            throw std::runtime_error("Type Error: 'cos' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "tan", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                return std::tan(std::get<double>(args[0]));
            }
            // TODO: Apply element-wise for Vectors/Tensors
            throw std::runtime_error("Type Error: 'tan' requires a scalar double.");
        }
    });

    // ━━ INVERSE TRIGONOMETRIC ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_function(FunctionStruct{
        "asin", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                const double val = std::get<double>(args[0]);
                if (val < -1.0 || val > 1.0) throw std::runtime_error("Domain Error: asin input must be between -1 and 1");
                return std::asin(val);
            }
            throw std::runtime_error("Type Error: 'asin' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "acos", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                const double val = std::get<double>(args[0]);
                if (val < -1.0 || val > 1.0) throw std::runtime_error("Domain Error: acos input must be between -1 and 1");
                return std::acos(val);
            }
            throw std::runtime_error("Type Error: 'acos' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "atan", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) return std::atan(std::get<double>(args[0]));
            throw std::runtime_error("Type Error: 'atan' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "atan2", 2, // Notice: 2 arguments
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0]) && std::holds_alternative<double>(args[1])) {
                return std::atan2(std::get<double>(args[0]), std::get<double>(args[1]));
            }
            throw std::runtime_error("Type Error: 'atan2' requires two scalar doubles.");
        }
    });

    // ━━ EXPONENTIAL & LOGARITHMIC ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_function(FunctionStruct{
        "exp", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                return std::exp(std::get<double>(args[0]));
            }
            // TODO: Matrix Exponential (e^A), which is critical for solving differential equations
            throw std::runtime_error("Type Error: 'exp' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "ln", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                const double val = std::get<double>(args[0]);
                if (val <= 0.0) throw std::runtime_error("Domain Error: ln requires a strictly positive number");
                // TODO: If val < 0, return std::complex<double> instead of throwing an error
                return std::log(val);
            }
            throw std::runtime_error("Type Error: 'ln' requires a scalar double.");
        }
    });

    // ━━ GENERALIZED LOGARITHM ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // Syntax: log(value, base)
    registry.register_function(FunctionStruct{
        "log", 2,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0]) && std::holds_alternative<double>(args[1])) {
                const double val = std::get<double>(args[0]);
                const double base = std::get<double>(args[1]);

                // Strict mathematical domain constraints
                if (val <= 0.0) throw std::runtime_error("Domain Error: logarithm value must be strictly positive");
                if (base <= 0.0 || base == 1.0) throw std::runtime_error("Domain Error: logarithm base must be strictly positive and not equal to 1");

                // Change of base formula: log_b(x) = ln(x) / ln(b)
                return std::log(val) / std::log(base);
            }
            // TODO: Element-wise logarithm for Tensors/Vectors
            throw std::runtime_error("Type Error: 'log' requires two scalar doubles (value, base).");
        }
    });

    // ━━ ROOTS & ABSOLUTE VALUE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_function(FunctionStruct{
        "sqrt", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                const double val = std::get<double>(args[0]);
                if (val < 0.0) {
                    // Return a complex number for negative roots
                    return std::complex<double>(0.0, std::sqrt(-val));
                }
                return std::sqrt(val);
            }
            // TODO: Matrix Square Root
            throw std::runtime_error("Type Error: 'sqrt' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "abs", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) {
                return std::abs(std::get<double>(args[0]));
            }
            if (std::holds_alternative<std::complex<double>>(args[0])) {
                return std::abs(std::get<std::complex<double>>(args[0])); // Returns double (magnitude)
            }
            // TODO: If Vector, calculate the Euclidean norm (magnitude)
            // TODO: If Matrix, calculate the Determinant or Frobenius norm
            throw std::runtime_error("Type Error: 'abs' requires a scalar or complex double.");
        }
    });

    // ━━ ROUNDING & UTILITY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_function(FunctionStruct{
        "floor", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) return std::floor(std::get<double>(args[0]));
            throw std::runtime_error("Type Error: 'floor' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "ceil", 1,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0])) return std::ceil(std::get<double>(args[0]));
            throw std::runtime_error("Type Error: 'ceil' requires a scalar double.");
        }
    });

    registry.register_function(FunctionStruct{
        "max", 2,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0]) && std::holds_alternative<double>(args[1])) {
                return std::max(std::get<double>(args[0]), std::get<double>(args[1]));
            }
            throw std::runtime_error("Type Error: 'max' requires two scalar doubles.");
        }
    });

    registry.register_function(FunctionStruct{
        "min", 2,
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0]) && std::holds_alternative<double>(args[1])) {
                return std::min(std::get<double>(args[0]), std::get<double>(args[1]));
            }
            throw std::runtime_error("Type Error: 'min' requires two scalar doubles.");
        }
    });

    // ━━ BOUNDING & CLAMPING ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    // Syntax: clamp(value, min, max)
    registry.register_function(FunctionStruct{
        "clamp", 3, // Notice the 3 arguments!
        [](const std::vector<Value>& args) -> Value {
            if (std::holds_alternative<double>(args[0]) &&
                std::holds_alternative<double>(args[1]) &&
                std::holds_alternative<double>(args[2])) {

                const double val = std::get<double>(args[0]);
                const double min_val = std::get<double>(args[1]);
                const double max_val = std::get<double>(args[2]);

                if (min_val > max_val) {
                    throw std::runtime_error("Domain Error: clamp 'min' cannot be greater than 'max'");
                }

                return std::clamp(val, min_val, max_val);
            }
            // TODO: Element-wise clamping for Matrices/Vectors (e.g., bounding pixel colors)
            throw std::runtime_error("Type Error: 'clamp' requires three scalar doubles.");
        }
    });
}

void load_constants(Registry& registry)
{
    registry.register_constant(ConstantStruct{
        "pi", std::numbers::pi
    });

    registry.register_constant(ConstantStruct{
        "e", std::numbers::e
    });

    registry.register_constant(ConstantStruct{
        "phi", std::numbers::phi
    });

    registry.register_constant(ConstantStruct{
        "tau", std::numbers::pi * 2.0
    });

    registry.register_constant(ConstantStruct{
        "sqrt2", std::numbers::sqrt2
    });
}

void load_registry(Registry& registry)
{
    load_standard_math(registry);
    load_functions(registry);
    load_constants(registry);
}