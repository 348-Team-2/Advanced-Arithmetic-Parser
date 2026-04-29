/**
* @file def_value.hpp
 * @brief Defines the core data types and the universal Value variant for the system.
 * @details This file establishes the polymorphic type system used across the
 * entire pipeline. By utilizing a std::variant, the Evaluator can handle
 * scalars, complex numbers, and linear algebra structures (Vectors, Matrices, Tensors)
 * through a single, unified interface.
 */

#pragma once

#include <vector>
#include <variant>
#include <complex>
#include <stdexcept>
#include <string>
#include <sstream>

/// @brief Represents a 1D Vector for spatial math and dot/cross products.
struct Vector {
    std::vector<double> components;
};

/// @brief Represents a 2D Matrix for linear algebra operations.
struct Matrix {
    size_t rows;
    size_t cols;
    std::vector<double> data; // Flattened 1D array for O(1) cache-friendly access
};

/// @brief Represents an N-dimensional Tensor for arbitrary multidimensional math.
struct Tensor {
    std::vector<size_t> shape;
    std::vector<double> data;
};

/// @brief Represents a finite set of discrete elements.
struct Set {
    std::vector<double> elements;
};

/// @brief The universal data unit for the Evaluator and Registry.
/// @note Replaces raw 'double' to support multiple mathematical types dynamically.
using Value = std::variant<double, std::complex<double>, Vector, Matrix, Tensor, Set>;

// ━━ TYPE IDENTIFICATION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/// @brief Safely extracts a string representation of a Value's type for error logging.
inline std::string get_type_name(const Value& v) {
    if (std::holds_alternative<double>(v)) return "Scalar";
    if (std::holds_alternative<std::complex<double>>(v)) return "Complex";
    if (std::holds_alternative<Vector>(v)) return "Vector";
    if (std::holds_alternative<Matrix>(v)) return "Matrix";
    if (std::holds_alternative<Tensor>(v)) return "Tensor";
    if (std::holds_alternative<Set>(v)) return "Set";
    return "Unknown";
}
