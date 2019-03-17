#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <stree/stree.hpp>
#include "args.hpp"
#include "individual.hpp"
#include "fitness.hpp"
#include "functions.hpp"
#include "genetic.hpp"
#include "plot.hpp"
#include "run.hpp"
#include "tree.hpp"

namespace {

void usage();

Args get_args(int argc, char** argv);

} // namespace


int main(int argc, char** argv) {
    // Arguments
    Args args = get_args(argc, argv);
    std::cout << args << std::endl;

    Run run(args);
    run.run();

    return 0;
}

namespace {

void usage() {
    std::cout << "Usage: TODO" << std::endl;
    std::exit(1);
}

Args get_args(int argc, char** argv) {
    Args args;
    try {
        args = parse_args(argc, argv);
        if (args.show_help)
            usage();
        validate_args(args);
        return args;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(2);
    }
}

} // namespace
