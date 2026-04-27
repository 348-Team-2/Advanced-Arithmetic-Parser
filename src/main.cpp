#include "cli/def_cli.hpp"
#include "err/debug.hpp"
#include "lexer/lexer.hpp"
#include "reg/registry.hpp"
#include "reg/math.hpp"

// here is the equation I have been testing on: ((abs(sqrt(-16)) * pi ** 2) > ln(e)) && (10 << 2 == 40) || (i == !0) -> (max(42 // 10, 5!) >= phi)
int main() {
    Registry math_registry;
    load_registry(math_registry);

    Lexer lexer(math_registry);

    CLI::print("Ready. Type an expression:");
    const std::string user_input = CLI::read_input(); // remove consts if not single line

    const std::vector<Token> tokens = lexer.tokenize(user_input);

    print_token_stream(tokens);

    return 0;
}
