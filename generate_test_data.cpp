#include <cstdlib>
#include <exception>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <stree/stree.hpp>
#include "functions.hpp"

namespace {

void usage();

} // namespace

int main(int argc, char** argv) {
    if (argc < 2)
        usage();

    // Read expression
    // read from STDIN if argument is "-"
    std::string expr;
    if (std::string(argv[1]) == "-") {
        std::istreambuf_iterator<char> it(std::cin), end;
        for (; it != end; ++it)
            expr += *it;
    } else {
        expr = argv[1];
    }

    float from = 0.0;
    float to   = 1.0;
    float step = 0.1;
    try {
        // Read arguments
        if (argc > 2)
            from = std::stof(argv[2]);
        if (argc > 3)
            to = std::stof(argv[3]);
        if (argc > 4)
            step = std::stof(argv[4]);

        // Fix range
        if (from > to)
            std::swap(from, to);

        // Check step
        if (step <= 0.0)
            throw std::invalid_argument("step must be > 0.0");

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(2);
    }

    // Environment
    stree::Environment env;
    env.add_function("+", 2, &::fun::plus);
    env.add_function("+3", 3, &::fun::plus);
    env.add_function("-", 2, &::fun::minus);
    env.add_function("-3", 3, &::fun::minus);
    env.add_function("*", 2, &::fun::multiply);
    env.add_function("*3", 3, &::fun::multiply);
    env.add_function("%", 2, &::fun::divide);
    env.add_function("square", 1, &::fun::square);
    env.add_function("cube", 1, &::fun::cube);
    env.add_function("expt", 2, &::fun::power);
    env.add_positional("a", 0);
    env.add_positional("b", 1);
    env.add_positional("c", 2);
    env.add_positional("d", 3);
    env.add_positional("e", 4);
    env.add_positional("f", 5);

    // Parse expression
    stree::Parser parser(&env);
    std::cerr << expr << std::endl;
    parser.parse(expr);
    if (parser.is_error())
        std::cerr << "Parse error: " << parser.error_message() << std::endl;
    if (!parser.is_done()) {
        std::cerr << "Parsing not finished" << std::endl
                  << "State: " << parser.state_string() << std::endl
                  << "Line: " << parser.line_num()
                  << ", Pos: " << parser.char_num() << std::endl;
        std::exit(3);
    }

    // Make tree
    stree::Tree tree(&env, parser.result());

    // Print comment line
    std::cout << "# " << tree << ", "
              << "(" << from << ", " << to << "), "
              << step << std::endl;

    // Eval for range
    stree::Params params({from});
    for (; params[0] <= to; params[0] += step) {
        stree::Value result = stree::eval(tree, params);
        std::cout << params[0] << ' ' << result << std::endl;
    }

    return 0;
}


namespace {

void usage() {
    std::cerr << "Usage: TODO" << std::endl;
    std::exit(1);
}

} // namespace
