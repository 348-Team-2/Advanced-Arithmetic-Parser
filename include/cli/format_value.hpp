/**
 * @file format_value.hpp
 * @brief String formatting utilities for the universal Value type.
 * @details Provides `to_string` overloads for all mathematical structures
 * (Vectors, Matrices, Tensors, Sets) and a unified interface for the `Value` variant.
 * These functions are primarily used for CLI output, debugging, and error logging.
 */

//TODO: SPLIT INTO FILE

#pragma once

#include <string>
#include <sstream>
#include <variant>
#include <complex>

#include "reg/def_value.hpp"

// ━━ STRING FORMATTING (PRINTING) ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @brief Formats a Vector as a comma-separated list enclosed in brackets.
 * @param v The Vector to format.
 * @return A string in the format: "[v0, v1, ...]"
 */
inline std::string to_string(const Vector& v) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.components.size(); ++i) {
        oss << v.components[i] << (i + 1 < v.components.size() ? ", " : "");
    }
    oss << "]";
    return oss.str();
}

/**
 * @brief Formats a Matrix as a 2D grid of values.
 * @details Elements are grouped by row, with newlines separating each row
 * for easier visual inspection of the matrix structure.
 * @param m The Matrix to format.
 * @return A multi-line string representing the matrix layout.
 */
inline std::string to_string(const Matrix& m) {
    std::ostringstream oss;
    oss << "[\n";
    for (size_t r = 0; r < m.rows; ++r) {
        oss << "  [";
        for (size_t c = 0; c < m.cols; ++c) {
            oss << m.data[r * m.cols + c] << (c + 1 < m.cols ? ", " : "");
        }
        oss << "]" << (r + 1 < m.rows ? ",\n" : "\n");
    }
    oss << "]";
    return oss.str();
}

/**
 * @brief Formats a Tensor showing both its dimensionality (shape) and flat data.
 * @param t The Tensor to format.
 * @return A string in the format: "Tensor(shape=[...], data=[...])"
 */
inline std::string to_string(const Tensor& t) {
    std::ostringstream oss;
    oss << "Tensor(shape=[";
    for (size_t i = 0; i < t.shape.size(); ++i) {
        oss << t.shape[i] << (i + 1 < t.shape.size() ? ", " : "");
    }
    oss << "], data=[";
    for (size_t i = 0; i < t.data.size(); ++i) {
        oss << t.data[i] << (i + 1 < t.data.size() ? ", " : "");
    }
    oss << "])";
    return oss.str();
}

/**
 * @brief Formats a mathematical Set as a comma-separated list enclosed in braces.
 * @param s The Set to format.
 * @return A string in the format: "{e0, e1, ...}"
 */
inline std::string to_string(const Set& s) {
    std::ostringstream oss;
    oss << "{";
    for (size_t i = 0; i < s.elements.size(); ++i) {
        oss << s.elements[i] << (i + 1 < s.elements.size() ? ", " : "");
    }
    oss << "}";
    return oss.str();
}

// ━━ VARIANT VISITOR ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/**
 * @struct ValueStringVisitor
 * @brief A visitor struct designed for use with std::visit to handle all Value variants dynamically.
 * @details Overloads the function call operator `operator()` for every type held
 * by the `Value` variant, routing them to the appropriate string formatting logic.
 */
struct ValueStringVisitor {

    /** @brief Formats a standard double-precision scalar. */
    std::string operator()(double d) const {
        std::ostringstream oss;
        oss << d;
        return oss.str();
    }

    /** @brief Formats a complex number in standard a+bi notation. */
    std::string operator()(const std::complex<double>& c) const {
        std::ostringstream oss;
        oss << c.real() << (c.imag() >= 0 ? "+" : "") << c.imag() << "i";
        return oss.str();
    }

    /** @brief Delegates to the overloaded Vector to_string function. */
    std::string operator()(const Vector& v) const { return to_string(v); }

    /** @brief Delegates to the overloaded Matrix to_string function. */
    std::string operator()(const Matrix& m) const { return to_string(m); }

    /** @brief Delegates to the overloaded Tensor to_string function. */
    std::string operator()(const Tensor& t) const { return to_string(t); }

    /** @brief Delegates to the overloaded Set to_string function. */
    std::string operator()(const Set& s) const { return to_string(s); }
};

/**
 * @brief Universal formatting function for the Value variant.
 * @details Uses std::visit and `ValueStringVisitor` to dynamically resolve
 * the underlying type of the variant at runtime and format it.
 * @param val The Value variant to format.
 * @return A formatted string representation of the underlying data.
 */
inline std::string to_string(const Value& val) {
    return std::visit(ValueStringVisitor{}, val);
}