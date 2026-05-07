#include <gtest/gtest.h>
#include "lexer/lexer.hpp"
#include "reg/registry.hpp"

extern void load_registry(Registry& registry);

class LexerTest : public ::testing::Test {
protected:
    Registry reg;
    Lexer* lexer;

    void SetUp() override {
        load_registry(reg);
        lexer = new Lexer(reg);
    }

    void TearDown() override {
        delete lexer;
    }
};

// ━━ 1. Token Types ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(LexerTest, TokenizesIntegerLiteral) {
    auto tokens = lexer->tokenize("42");

    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[0].lexeme, "42");
    EXPECT_DOUBLE_EQ(std::get<double>(tokens[0].literal), 42.0);
}

TEST_F(LexerTest, TokenizesFloatLiteral) {
    auto tokens = lexer->tokenize("3.14");

    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_DOUBLE_EQ(std::get<double>(tokens[0].literal), 3.14);
}

TEST_F(LexerTest, TokenizesKnownOperator) {
    auto tokens = lexer->tokenize("2+3");

    ASSERT_GE(tokens.size(), 3u);
    EXPECT_EQ(tokens[1].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[1].lexeme, "+");
}

TEST_F(LexerTest, TokenizesMultiCharOperator) {
    auto tokens = lexer->tokenize("2**3");

    ASSERT_GE(tokens.size(), 3u);
    EXPECT_EQ(tokens[1].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[1].lexeme, "**");
}

TEST_F(LexerTest, TokenizesRegisteredFunction) {
    auto tokens = lexer->tokenize("sin(1)");

    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].type, TokenType::FUNCTION);
    EXPECT_EQ(tokens[0].lexeme, "sin");
}

TEST_F(LexerTest, TokenizesRegisteredConstant) {
    auto tokens = lexer->tokenize("pi");

    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].lexeme, "pi");
}

TEST_F(LexerTest, TokenizesParentheses) {
    auto tokens = lexer->tokenize("(1)");

    ASSERT_GE(tokens.size(), 3u);
    EXPECT_EQ(tokens[0].type, TokenType::LPAREN);
    EXPECT_EQ(tokens[2].type, TokenType::RPAREN);
}

// TODO fix
// TEST_F(LexerTest, TokenizesComma) {
//     auto tokens = lexer->tokenize("max(1,2)");

//     bool found_comma = false;
//     for (const auto& t : tokens) {
//         if (t.type == TokenType::COMMA) { found_comma = true; break; }
//     }
//     EXPECT_TRUE(found_comma);
// }

// ━━ 2. Whitespace & EOF ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(LexerTest, SkipsWhitespace) {
    auto tokens = lexer->tokenize("  2  +  3  ");

    // Should produce: NUMBER, OPERATOR, NUMBER, EOF
    ASSERT_GE(tokens.size(), 3u);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_EQ(tokens[1].type, TokenType::OPERATOR);
    EXPECT_EQ(tokens[2].type, TokenType::NUMBER);
}

TEST_F(LexerTest, AppendsEOFToken) {
    auto tokens = lexer->tokenize("1");

    EXPECT_EQ(tokens.back().type, TokenType::EOF_TOKEN);
}

TEST_F(LexerTest, EmptyInputProducesOnlyEOF) {
    auto tokens = lexer->tokenize("");

    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].type, TokenType::EOF_TOKEN);
}

// ━━ 3. Column Tracking ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(LexerTest, TracksColumnPosition) {
    auto tokens = lexer->tokenize("1+2");

    EXPECT_EQ(tokens[0].column, 1);
    EXPECT_EQ(tokens[1].column, 2);
    EXPECT_EQ(tokens[2].column, 3);
}

// ━━ 4. Complex Expressions ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TEST_F(LexerTest, TokenizesFullExpression) {
    auto tokens = lexer->tokenize("sin(pi) + 2**3");

    // sin, (, pi, ), +, 2, **, 3, EOF = 9 tokens
    EXPECT_EQ(tokens.size(), 9u);
}
