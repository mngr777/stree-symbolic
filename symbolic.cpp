#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <stree/stree.hpp>
#include "args.hpp"
#include "individual.hpp"
#include "fitness.hpp"
#include "functions.hpp"
#include "tree.hpp"

namespace {

void usage();

std::string positional_name(unsigned n);

} // namespace


int main(int argc, char** argv) {
    // Arguments
    Args args;
    try {
        args = parse_args(argc, argv);
        validate_args(args);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(2);
    }
    std::cout << args << std::endl;

    // Environment
    stree::Environment env;
    // TODO: custom function list from args
    env.add_function("+", 2, &::plus);
    env.add_function("-", 2, &::minus);
    env.add_function("*", 2, &::multiply);
    env.add_function("%", 2, &::divide);
    for (unsigned n = 0; n < args.param_num; ++n)
        env.add_positional(positional_name(n), n);
    env.add_constant(RandomValueSymbolName);

    // Load fitness cases
    try {
        load_fitness_cases(args.fitness_cases_filename, args.param_num);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(3);
    }

    // PRNG
    // TODO: selectable PRNG
    RandomMt19937 rd(args.prng_seed);

    // Random value generator
    RandomValue rv(rd, 0.0, 1.0);

    // Generate initial population
    Population population = ramped_half_and_half(
        env,
        args.population_size,
        args.initial_depth,
        rd,
        &rv,
        args.p_term);

    for (Individual& individual: population) {
        std::cout << individual.tree() << std::endl;
    }

    return 0;
}


namespace {

void usage() {
    std::cout << "Usage: TODO" << std::endl;
    std::exit(1);
}

// "a"-"z", "a1"-"z1", ...
// TODO: custom names from args
std::string positional_name(unsigned n) {
    static unsigned letter_num = ('z' - 'a' + 1);
    char ch = 'a' + (n % letter_num);
    std::string name({ch});
    unsigned mult = n / letter_num;
    if (mult > 0) {
        name += std::to_string(mult);
    }
    return name;
}

} // namespace
