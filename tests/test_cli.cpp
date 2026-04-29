#include <gtest/gtest.h>
#include "cli/cli.hpp"
#include <iostream>
#include <sstream>

TEST(SanityCheck, DoesGTestWork) {
    EXPECT_EQ(1, 1);
}

class CLITest : public ::testing:Test {
protected:
    CLI cli;
    std::streambuf* orig_cin_buf;
    std::streambuf* orig_cout_buf;
    std::stringstream test_cin;
    std::stringstream test_cout;

    void SetUp() override {
        // Redirect cin and cout to our stringstreams
        orig_cin_buf = std::cin.rdbuf(test_cin.rdbuf());
        orig_cout_buf = std::cout.rdbuf(test_cout.rdbuf());
    }

    void TearDown() override {
        // Restore original buffers
        std::cin.rdbuf(orig_cin_buf);
        std::cout.rdbuf(orig_cout_buf);
    }
};

TEST_F(CLITest, HandlesSimpleInput) {
    CLI cli;

    test_cin << "bouncy";
    std::string result = cli.read_input(); 

    EXPECT_EQ(test_cout.str(), "bouncy\n");
}

TEST(CLITest, HandlesInputWithSpaces) {
    test_cin << "super bouncy";
    std::string result = cli.read_input(); 

    EXPECT_EQ(test_cout.str(), "superbouncy\n");
}

TEST(CLITest, HandlesPrintingCharacters) {
    cli.print(1234567890qwertyuiopasdfghjklzxcvbnm)
    EXPECT_EQ(test_cout.str(), "1234567890qwertyuiopasdfghjklzxcvbnm");
}

TEST(CLITest, HandlesPrintingSymbols) {
    cli.print(!@#$%^&*()_+-=[]/,.<>;':\"\\|}{})
    EXPECT_EQ(test_cout.str(), "!@#$%^&*()_+-=[]/,.<>;':\"\\|}{}");
}
