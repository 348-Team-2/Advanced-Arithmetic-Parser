// here are the equations I have been testing on:
// SIMPLE: (3+2) - (4 * 3 - 2) - 3 * 7 ^ 2
// COMPLEX: ((abs(sqrt(-16)) * pi ** 2) > ln(e)) && (10 << 2 == 40) || (1 == !0) -> (max(42 // 10, 5!) >= phi)

#include <iostream>
#include <string>

#include "cli/cli.hpp"
#include "core/engine.hpp"

int main() {

    CLI::print_header();

    const std::string user_input = CLI::read_input();

    if (user_input == "\n" || user_input.empty()) {
        CLI::print_footer();
        return 0;
    }

    try {
        Engine calc_engine;
        const Value result = calc_engine.execute(user_input);
        CLI::print_value(result);

    } catch (const ParseError& e) {
        CLI::print_error(e);


    } catch (const std::exception& e) {
        CLI::print("Evaluation Error: " + std::string(e.what())); // TODO: REPLACE WITH ERROR?
    }

    CLI::print_footer();

    return 0;
}

//TODO: ADD DOX FOR EVAL
//TODO: ADD DOXYCOMMENTS FOR EVAL/DEF_AST