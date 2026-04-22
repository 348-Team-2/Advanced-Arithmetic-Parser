#include <gtest/gtest.h>
#include "reg/registry.hpp"

extern void load_registry(Registry& registry);


TEST(SanityCheck, DoesGTestWork) {
    EXPECT_EQ(1, 1);
}

class RegistryTest : public ::testing::Test {
protected:
    Registry reg;

    void SetUp() override {
        load_registry(reg);
    }
};

// ━━ 1. Test Lookups & Metadata ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(RegistryTest, IdentifiesOperatorsCorrectly) {
    EXPECT_TRUE(reg.is_operator("+"));
    EXPECT_TRUE(reg.is_operator("**"));
    EXPECT_FALSE(reg.is_operator("not_an_op"));
}

TEST_F(RegistryTest, ReturnsCorrectPrecedence) {
    // Exponentiation should evaluate before Addition
    EXPECT_GT(reg.get_precedence("**"), reg.get_precedence("+"));

    // Unary minus should evaluate before Exponentiation
    EXPECT_GT(reg.get_precedence("u-"), reg.get_precedence("**"));
}

// ━━ 2. Test Binary Math ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(RegistryTest, EvaluatesAddition) {
    Value a = 5.0;
    Value b = 3.5;

    Value result = reg.evaluate_binary("+", a, b);

    ASSERT_TRUE(std::holds_alternative<double>(result));
    EXPECT_DOUBLE_EQ(std::get<double>(result), 8.5);
}

TEST_F(RegistryTest, EvaluatesExponentiation) {
    Value a = 2.0;
    Value b = 3.0;

    Value result = reg.evaluate_binary("**", a, b);
    EXPECT_DOUBLE_EQ(std::get<double>(result), 8.0);
}

// ━━ 3. Test Functions & Arity ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(RegistryTest, EvaluatesMathFunctions) {
    std::vector<Value> args = { reg.get_constant("pi") }; // Pass Pi as an argument

    Value result = reg.evaluate_function("sin", args);

    // sin(pi) should be extremely close to 0
    EXPECT_NEAR(std::get<double>(result), 0.0, 1e-9);
}

// ━━ 4. Test Errors & Type Safety (The Sad Path) ━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(RegistryTest, ThrowsOnDivisionByZero) {
    Value a = 10.0;
    Value b = 0.0;

    EXPECT_THROW(reg.evaluate_binary("/", a, b), std::runtime_error);
}

TEST_F(RegistryTest, ThrowsOnInvalidFunctionArity) {
    // 'atan2' expects 2 arguments, we are only giving it 1
    std::vector<Value> bad_args = { 5.0 };

    EXPECT_THROW(reg.evaluate_function("atan2", bad_args), std::invalid_argument);
}

TEST_F(RegistryTest, ThrowsOnInvalidTypes) {
    Value num = 5.0;
    Value vec = Vector{{1.0, 2.0, 3.0}}; // Assume Vector is defined in your types

    // We haven't implemented Scalar + Vector yet, so this should trigger your fallback!
    EXPECT_THROW(reg.evaluate_binary("+", num, vec), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}