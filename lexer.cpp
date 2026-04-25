#include <def_const.hpp>
#include "token.hpp"

// STEPS:
// 1. Receive user input from command line interface (CLI).
// 2. Create list of tokens from the input string using the Lexer.

// 2

// This is how you create a token
Token* token1 = new Token(TokenType::NUMBER, "2", new Value(2), 1, 4);