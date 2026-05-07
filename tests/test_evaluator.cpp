#include <gtest/gtest.h>
#include "eval/evaluator.hpp"
#include "parse/parser.hpp"
#include "lexer/lexer.hpp"
#include "reg/registry.hpp"

extern void load_registry(Registry& registry);

class EvaluatorTest : public ::testing::Test {
protected:
    Registry reg;

    void SetUp() override {
        load_registry(reg);
    }

    double eval(const std::string& input) {
        Lexer lexer(reg);
        auto tokens = lexer.tokenize(input);
        Parser parser(tokens, reg);
        auto ast = parser.parse();
        Evaluator evaluator(reg);
        Value result = evaluator.evaluate(*ast);
        return std::get<double>(result);
    }
};

// ━━ 1. Arithmetic ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EvaluatorTest, EvaluatesAddition) {
    EXPECT_DOUBLE_EQ(eval("2+3"), 5.0);
}

TEST_F(EvaluatorTest, EvaluatesSubtraction) {
    EXPECT_DOUBLE_EQ(eval("10-4"), 6.0);
}

TEST_F(EvaluatorTest, EvaluatesMultiplication) {
    EXPECT_DOUBLE_EQ(eval("3*4"), 12.0);
}

TEST_F(EvaluatorTest, EvaluatesDivision) {
    EXPECT_DOUBLE_EQ(eval("10/4"), 2.5);
}

TEST_F(EvaluatorTest, EvaluatesExponentiation) {
    EXPECT_DOUBLE_EQ(eval("2**10"), 1024.0);
}

TEST_F(EvaluatorTest, EvaluatesModulo) {
    EXPECT_DOUBLE_EQ(eval("10%3"), 1.0);
}

// ━━ 2. Unary Operations ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EvaluatorTest, EvaluatesUnaryMinus) {
    EXPECT_DOUBLE_EQ(eval("-5"), -5.0);
}

TEST_F(EvaluatorTest, EvaluatesUnaryPlus) {
    EXPECT_DOUBLE_EQ(eval("+5"), 5.0);
}

TEST_F(EvaluatorTest, EvaluatesFactorial) {
    EXPECT_DOUBLE_EQ(eval("5!"), 120.0);
}

// ━━ 3. Precedence & Grouping ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EvaluatorTest, RespectsOperatorPrecedence) {
    EXPECT_DOUBLE_EQ(eval("2+3*4"), 14.0);
}

TEST_F(EvaluatorTest, RespectsParentheses) {
    EXPECT_DOUBLE_EQ(eval("(2+3)*4"), 20.0);
}

// ━━ 4. Functions ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EvaluatorTest, EvaluatesSin) {
    EXPECT_NEAR(eval("sin(0)"), 0.0, 1e-9);
}

TEST_F(EvaluatorTest, EvaluatesCos) {
    EXPECT_NEAR(eval("cos(0)"), 1.0, 1e-9);
}

TEST_F(EvaluatorTest, EvaluatesSqrt) {
    EXPECT_DOUBLE_EQ(eval("sqrt(9)"), 3.0);
}

// TODO fix 
// TEST_F(EvaluatorTest, EvaluatesAtan2) {
//     EXPECT_NEAR(eval("atan2(1,1)"), M_PI / 4.0, 1e-9);
// }

// ━━ 5. Constants ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EvaluatorTest, EvaluatesPiConstant) {
    EXPECT_NEAR(eval("pi"), M_PI, 1e-9);
}

TEST_F(EvaluatorTest, EvaluatesEConstant) {
    EXPECT_NEAR(eval("e"), M_E, 1e-9);
}

// ━━ 6. Error Cases ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(EvaluatorTest, ThrowsOnDivisionByZero) {
    Lexer lexer(reg);
    auto tokens = lexer.tokenize("1/0");
    Parser parser(tokens, reg);
    auto ast = parser.parse();
    Evaluator evaluator(reg);

    EXPECT_THROW(evaluator.evaluate(*ast), EvalError);
}
