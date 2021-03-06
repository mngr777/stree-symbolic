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
        if (args.show_help)
            usage();
        validate_args(args);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::exit(2);
    }
    std::cout << args << std::endl;

    // Environment
    stree::Environment env;
    // TODO: custom function list from args
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
    for (unsigned n = 0; n < args.param_num; ++n)
        env.add_positional(positional_name(n), n);
    env.add_constant(RandomValueSymbolName);

    // Load fitness cases
    FitnessCaseList fitness_cases;
    try {
        fitness_cases = load_fitness_cases(args.fitness_cases_filename, args.param_num);
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

    unsigned generation = 1;
    Fitness min_fitness = evaluate(population, fitness_cases);
    for (; generation <= args.max_generation && min_fitness > args.goal; ++generation) {
        std::cout << "Generation " << generation << std::endl;

        // TEST mutation
        assert(population.size() > 10);
        GroupIdx mutants = random_group(population, 10, rd);
        for (IndivIdx idx : mutants) {
            mutate_headless(population[idx].tree(), 3, rd, &rv);
        }

        Population next_population;
        unsigned crossover_num = 0, crossover_success_num = 0;
        while (next_population.size() < population.size()) {
            // Select 1st individual
            IndivIdx idx1 = tournament(
                population,
                random_group(population, args.crossover_tournament_size, rd));
            // Select 2nd individual
            IndivIdx idx2 = tournament(
                population,
                random_group(population, args.crossover_tournament_size, rd));
            // Crossover
            bool crossover_success = false;
            auto offspring = crossover_one_point(
                population[idx1].tree(),
                population[idx2].tree(),
                rd, 0.2,
                &crossover_success);
            ++crossover_num;
            if (crossover_success)
                ++crossover_success_num;
            // Add to new population
            for (stree::Tree& tree : offspring) {
                next_population.emplace_back(std::move(tree));
            }
        }
        std::cout << "Crossover number: " << crossover_num
                  << ", success " << crossover_success_num
                  << std::endl;
        std::swap(population, next_population);
        min_fitness = evaluate(population, fitness_cases);
    }

    std::cout << min_fitness << std::endl;
    {
        auto it = std::find_if(
            population.begin(), population.end(),
            [min_fitness](const Individual& individual) {
                return individual.fitness() == min_fitness;
            });
        assert(it != population.end());
        std::cout << it->tree() << std::endl;
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
