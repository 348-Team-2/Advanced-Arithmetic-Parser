#include "registry.hpp"
// Include your new Value definitions here
// #include "Def_Value.hpp"

#include <cmath>
#include <stdexcept>
#include <cstdint>

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
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) == std::get<double>(b)) ? 1.0 : 0.0;
            }
            // TODO: Implement Matrix == Matrix or Vector == Vector deep comparisons
            throw std::runtime_error("Type Error: == requires matching types.");
        }, nullptr
    });

    registry.register_operator({
        "!=", 6, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) < std::get<double>(b)) ? 1.0 : 0.0;
            }
            // TODO: Implement subset checks (Set A < Set B)
            throw std::runtime_error("Type Error: < requires scalar doubles or Sets.");
        }, nullptr
    });

    registry.register_operator({
        "<=", 7, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) <= std::get<double>(b)) ? 1.0 : 0.0;
            }
            throw std::runtime_error("Type Error: <= requires scalar doubles.");
        }, nullptr
    });

    registry.register_operator({
        ">", 7, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) > std::get<double>(b)) ? 1.0 : 0.0;
            }
            // TODO: Implement superset checks
            throw std::runtime_error("Type Error: > requires scalar doubles.");
        }, nullptr
    });

    registry.register_operator({
        ">=", 7, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return (std::get<double>(a) >= std::get<double>(b)) ? 1.0 : 0.0;
            }
            throw std::runtime_error("Type Error: >= requires scalar doubles.");
        }, nullptr
    });

    // ━━ BITWISE SHIFTS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({
        "<<", 8, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return static_cast<double>(static_cast<int64_t>(std::get<double>(a)) << static_cast<int64_t>(std::get<double>(b)));
            }
            throw std::runtime_error("Type Error: << requires scalar doubles.");
        }, nullptr
    });

    registry.register_operator({
        ">>", 8, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                return static_cast<double>(static_cast<int64_t>(std::get<double>(a)) >> static_cast<int64_t>(std::get<double>(b)));
            }
            throw std::runtime_error("Type Error: >> requires scalar doubles.");
        }, nullptr
    });

    // ━━ ADDITION AND SUBTRACTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "+", 9, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
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
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                double denom = std::get<double>(b);
                if (denom == 0.0) throw std::runtime_error("Modulo by zero");
                return std::fmod(std::get<double>(a), denom);
            }
            throw std::runtime_error("Type Error: Invalid types for %");
        }, nullptr
    });

    registry.register_operator({ "//", 10, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
            if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
                double denom = std::get<double>(b);
                if (denom == 0.0) throw std::runtime_error("Division by zero");
                return std::trunc(std::get<double>(a) / denom);
            }
            throw std::runtime_error("Type Error: Invalid types for //");
        }, nullptr
    });

    // ━━ DOT PRODUCT ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ ".", 10, Associativity::LEFT, false,
        [](Value a, Value b) -> Value {
            // TODO: Vector . Vector (Calculate standard sum-product and return a scalar double)
            // TODO: Tensor . Tensor (Generalized contraction)
            throw std::runtime_error("Type Error: Operator '.' expects two Vectors or Tensors.");
        }, nullptr
    });

    // ━━ EXPONENTIATION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "**", 11, Associativity::RIGHT, false,
        [](Value a, Value b) -> Value {
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
        [](Value a) -> Value {
            if (std::holds_alternative<double>(a)) return -std::get<double>(a);
            // TODO: Negate every component in a Vector/Matrix
            throw std::runtime_error("Type Error: Invalid type for unary -");
        }
    });

    registry.register_operator({ "u~", 12, Associativity::NONE, true, nullptr,
        [](Value a) -> Value {
            if (std::holds_alternative<double>(a)) return static_cast<double>(~static_cast<int64_t>(std::get<double>(a)));
            // TODO: Complex Conjugate if 'a' is a complex number
            throw std::runtime_error("Type Error: Invalid type for unary ~");
        }
    });

    registry.register_operator({ "u!", 12, Associativity::NONE, true, nullptr,
        [](Value a) -> Value {
            if (std::holds_alternative<double>(a)) return (std::get<double>(a) == 0.0) ? 1.0 : 0.0;
            throw std::runtime_error("Type Error: Invalid type for unary !");
        }
    });

    // ━━ POSTFIX UNARY OPERATORS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "p!", 13, Associativity::NONE, true, nullptr,
        [](Value a) -> Value {
            if (std::holds_alternative<double>(a)) {
                double val = std::get<double>(a);
                if (val < 0.0 && std::floor(val) == val) throw std::runtime_error("Factorial of negative integer is undefined");
                return std::tgamma(val + 1.0);
            }
            throw std::runtime_error("Type Error: Factorial requires a scalar double.");
        }
    });

    registry.register_operator({ "p%", 13, Associativity::NONE, true, nullptr,
        [](Value a) -> Value {
            if (std::holds_alternative<double>(a)) return std::get<double>(a) / 100.0;
            throw std::runtime_error("Type Error: Percentage requires a scalar double.");
        }
    });

    // TODO: Add "pT" (Matrix Transpose Postfix Operator)
    // registry.register_operator({ "pT", 13, Associativity::NONE, true, nullptr, [](Value a) -> Value { ... } });

    // ━━ IMPLICATION AND EQUIVALENCE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    registry.register_operator({ "->", 1, Associativity::RIGHT, false,
        [](Value p, Value q) -> Value {
            if (std::holds_alternative<double>(p) && std::holds_alternative<double>(q)) {
                bool p_bool = (std::get<double>(p) != 0.0);
                bool q_bool = (std::get<double>(q) != 0.0);
                return (!p_bool || q_bool) ? 1.0 : 0.0;
            }
            throw std::runtime_error("Type Error: -> requires scalar doubles.");
        }, nullptr
    });

    registry.register_operator({ "<->", 1, Associativity::LEFT, false,
        [](Value p, Value q) -> Value {
            if (std::holds_alternative<double>(p) && std::holds_alternative<double>(q)) {
                return ((std::get<double>(p) != 0.0) == (std::get<double>(q) != 0.0)) ? 1.0 : 0.0;
            }
            throw std::runtime_error("Type Error: <-> requires scalar doubles.");
        }, nullptr
    });
}