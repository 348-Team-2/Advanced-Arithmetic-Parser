#include <gtest/gtest.h>
#include "parse/parser.hpp"
#include "lexer/lexer.hpp"
#include "reg/registry.hpp"

extern void load_registry(Registry& registry);

class ParserTest : public ::testing::Test {
protected:
    Registry reg;

    void SetUp() override {
        load_registry(reg);
    }

    std::unique_ptr<ASTNode> parse(const std::string& input) {
        Lexer lexer(reg);
        auto tokens = lexer.tokenize(input);
        Parser parser(tokens, reg);
        return parser.parse();
    }
};

// ━━ 1. Literal Nodes ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(ParserTest, ParsesNumericLiteral) {
    auto ast = parse("42");

    ASSERT_NE(ast, nullptr);
    auto* lit = dynamic_cast<LiteralNode*>(ast.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_DOUBLE_EQ(std::get<double>(lit->value), 42.0);
}

TEST_F(ParserTest, ParsesConstantIdentifier) {
    auto ast = parse("pi");

    ASSERT_NE(ast, nullptr);
    // pi resolves to a LiteralNode with the constant's value
    auto* lit = dynamic_cast<LiteralNode*>(ast.get());
    ASSERT_NE(lit, nullptr);
}

// ━━ 2. Binary Operations ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(ParserTest, ParsesAddition) {
    auto ast = parse("1+2");

    ASSERT_NE(ast, nullptr);
    auto* bin = dynamic_cast<BinaryOpNode*>(ast.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, "+");
}

TEST_F(ParserTest, ParsesSubtraction) {
    auto ast = parse("5-3");

    auto* bin = dynamic_cast<BinaryOpNode*>(ast.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, "-");
}

TEST_F(ParserTest, ParsesExponentiation) {
    auto ast = parse("2**8");

    auto* bin = dynamic_cast<BinaryOpNode*>(ast.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, "**");
}

TEST_F(ParserTest, RespectsOperatorPrecedence) {
    // 2 + 3 * 4 should parse as 2 + (3 * 4), so root is "+"
    auto ast = parse("2+3*4");

    auto* bin = dynamic_cast<BinaryOpNode*>(ast.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, "+");

    // Right child of "+" should be "*"
    auto* rhs = dynamic_cast<BinaryOpNode*>(bin->right.get());
    ASSERT_NE(rhs, nullptr);
    EXPECT_EQ(rhs->op, "*");
}

TEST_F(ParserTest, RespectsParenthesizedGrouping) {
    // (2 + 3) * 4 — root must be "*"
    auto ast = parse("(2+3)*4");

    auto* bin = dynamic_cast<BinaryOpNode*>(ast.get());
    ASSERT_NE(bin, nullptr);
    EXPECT_EQ(bin->op, "*");
}

// ━━ 3. Unary Operations ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(ParserTest, ParsesPrefixNegation) {
    auto ast = parse("-5");

    auto* un = dynamic_cast<UnaryOpNode*>(ast.get());
    ASSERT_NE(un, nullptr);
    EXPECT_EQ(un->op, "u-");
}

TEST_F(ParserTest, ParsesPostfixFactorial) {
    auto ast = parse("5!");

    auto* un = dynamic_cast<UnaryOpNode*>(ast.get());
    ASSERT_NE(un, nullptr);
    EXPECT_EQ(un->op, "p!");
}

// ━━ 4. Function Calls ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(ParserTest, ParsesSingleArgFunction) {
    auto ast = parse("sin(1)");

    auto* fn = dynamic_cast<FunctionCallNode*>(ast.get());
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->name, "sin");
    EXPECT_EQ(fn->args.size(), 1u);
}

TEST_F(ParserTest, ParsesMultiArgFunction) {
    auto ast = parse("atan2(1,2)");

    auto* fn = dynamic_cast<FunctionCallNode*>(ast.get());
    ASSERT_NE(fn, nullptr);
    EXPECT_EQ(fn->name, "atan2");
    EXPECT_EQ(fn->args.size(), 2u);
}

// ━━ 5. Error Cases ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(ParserTest, ThrowsOnUnmatchedParen) {
    EXPECT_THROW(parse("(1+2"), ParseError);
}

TEST_F(ParserTest, ThrowsOnTrailingOperator) {
    EXPECT_THROW(parse("1+"), ParseError);
}
