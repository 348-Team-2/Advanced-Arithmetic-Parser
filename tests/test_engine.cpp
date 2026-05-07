#include <gtest/gtest.h>
#include "core/engine.hpp"

class EngineTest : public ::testing::Test {
protected:
    Engine engine;
};

// ━━ 1. Basic Arithmetic ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EngineTest, EvaluatesAddition) {
    Value result = engine.execute("1+2");
    EXPECT_DOUBLE_EQ(std::get<double>(result), 3.0);
}

TEST_F(EngineTest, EvaluatesMultiplication) {
    Value result = engine.execute("3*4");
    EXPECT_DOUBLE_EQ(std::get<double>(result), 12.0);
}

TEST_F(EngineTest, EvaluatesExponentiation) {
    Value result = engine.execute("2**8");
    EXPECT_DOUBLE_EQ(std::get<double>(result), 256.0);
}

// ━━ 2. Full Pipeline ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EngineTest, EvaluatesComplexExpression) {
    // (2 + 3) * 4 - 1 = 19
    Value result = engine.execute("(2+3)*4-1");
    EXPECT_DOUBLE_EQ(std::get<double>(result), 19.0);
}

TEST_F(EngineTest, EvaluatesWithFunctionAndConstant) {
    // sin(pi) ≈ 0
    Value result = engine.execute("sin(pi)");
    EXPECT_NEAR(std::get<double>(result), 0.0, 1e-9);
}

TEST_F(EngineTest, EvaluatesUnaryNegation) {
    Value result = engine.execute("-7");
    EXPECT_DOUBLE_EQ(std::get<double>(result), -7.0);
}

TEST_F(EngineTest, EvaluatesNestedFunctions) {
    // sqrt(abs(-16)) = 4
    Value result = engine.execute("sqrt(abs(-16))");
    EXPECT_DOUBLE_EQ(std::get<double>(result), 4.0);
}

// ━━ 3. Error Handling ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EngineTest, ThrowsOnEmptyExpression) {
    EXPECT_THROW(engine.execute(""), std::runtime_error);
}

TEST_F(EngineTest, ThrowsOnSyntaxError) {
    EXPECT_THROW(engine.execute("1+"), ParseError);
}

TEST_F(EngineTest, ThrowsOnDivisionByZero) {
    EXPECT_THROW(engine.execute("5/0"), EvalError);
}
