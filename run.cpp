#include "run.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include "functions.hpp"
#include "tree.hpp"

namespace {

std::string positional_name(unsigned n);

} // anonymous namespace

Run::Run(const Args& args)
    : args_(args)
{
    init_random();
    init_env();
    init_fitness_cases();
}

void Run::run() {
    init_population();
    PopulationFitness fitness = population_fitness(population_);
    output_population_data(fitness);
    while (generation_ < args_.max_generation && fitness.min > args_.goal) {
        next_population_ = Population();

        mutation();
        crossover();
        reproduction();

        std::swap(population_, next_population_);
        evaluate(population_, fitness_cases_);
        ++generation_;
        fitness = population_fitness(population_);
        output_population_data(fitness);
    }
}

void Run::init_random() {
    // TODO: `random' refactoring
    rd_ = std::unique_ptr<Random>(new RandomMt19937(args_.prng_seed));
    rv_ = std::unique_ptr<RandomValue>(
        new RandomValue(*rd_, 0.0, 1.0 /* TODO: use args */));
}

void Run::init_env() {
    env_.add_function("+", 2, &::fun::plus);
    env_.add_function("+3", 3, &::fun::plus);
    env_.add_function("-", 2, &::fun::minus);
    env_.add_function("-3", 3, &::fun::minus);
    env_.add_function("*", 2, &::fun::multiply);
    env_.add_function("*3", 3, &::fun::multiply);
    env_.add_function("%", 2, &::fun::divide);
    env_.add_function("square", 1, &::fun::square);
    env_.add_function("cube", 1, &::fun::cube);
    env_.add_function("expt", 2, &::fun::power);
    for (unsigned n = 0; n < args_.param_num; ++n)
        env_.add_positional(positional_name(n), n);
    env_.add_constant(RandomValueSymbolName);
}

void Run::init_fitness_cases() {
    fitness_cases_ = load_fitness_cases(
        args_.fitness_cases_filename, args_.param_num);
}

void Run::init_population() {
    generation_ = 1;
    population_ = ramped_half_and_half(
        env_,
        args_.population_size,
        args_.initial_depth,
        *rd_, rv_.get(),
        args_.p_term);
    evaluate(population_, fitness_cases_);
}

void Run::output_population_data(const PopulationFitness& fitness) {
    std::cout << "Generation " << generation_ << std::endl
              << "Fitness: " << std::endl
              << fitness << std::endl;
}

void Run::mutation() {
    GroupIdx mutants = random_group(10 /* TODO */);
    for (IndivIdx idx : mutants) {
        mutate_headless(population_[idx].tree(), 3, *rd_, rv_.get());
    }
}

void Run::crossover() {
    while (next_population_.size() < args_.population_size) {
        TreeList offspring = _crossover();
        for (stree::Tree& child : offspring) {
            next_population_.emplace_back(std::move(child));
            if (next_population_.size() == args_.population_size)
                break;
        }
    }
}

void Run::reproduction() {

}

TreeList Run::_crossover() {
    IndivIdx idx1 = tournament(args_.crossover_tournament_size);
    IndivIdx idx2 = tournament(args_.crossover_tournament_size);
    TreeList offspring;
    offspring = crossover_one_point(
        population_[idx1].tree(),
        population_[idx2].tree(),
        *rd_);
    // TODO: crossover stats
    return offspring;
}

IndivIdx Run::tournament(unsigned size) {
    return ::tournament(population_, random_group(size));
}

GroupIdx Run::random_group(unsigned size) {
    return ::random_group(population_, size, *rd_);
}

namespace {

// "a"-"z", "a1"-"z1", ...
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

} // anonymous namespace
