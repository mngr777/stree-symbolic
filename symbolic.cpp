#include <iostream>
#include <random>
#include <string>
#include <stree/stree.hpp>
#include "args.hpp"
#include "individual.hpp"
#include "fitness.hpp"
#include "functions.hpp"
#include "tree.hpp"

int main(int argc, char** argv) {
    using std::cerr;
    using std::cout;
    using std::endl;

    // Environment
    stree::Environment env;
    env.add_positional("a", 0);
    env.add_positional("b", 1);
    env.add_function("+", 2, &::plus);
    env.add_function("-", 2, &::minus);
    env.add_function("*", 2, &::multiply);
    env.add_function("%", 2, &::divide);

    // Random device
    std::random_device rd;

    // Arguments
    Args args = parse_args(argc, argv);
    std::cout << args << std::endl;

    // Generate initial population
    Population population =
        ramped_half_and_half(env, rd, args.population_size, args.initial_depth, args.p_term);

    for (Individual& individual: population) {
        cout << individual.tree() << endl;
    }

    return 0;
}
