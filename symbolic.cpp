#include <iostream>
#include <random>
#include <string>
#include <stree/stree.hpp>
#include "individual.hpp"
#include "functions.hpp"
#include "tree.hpp"

int main() {
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

    // Parameters
    unsigned population_size = 100;
    unsigned initial_depth   = 5;
    float p_term             = 0.2;
    unsigned max_generation  = 100;
    stree::Params params{2.0, 3.0};

    // Random device
    std::random_device rd;

    // Generate initial population
    Population population =
        ramped_half_and_half(env, rd, population_size, initial_depth, p_term);

    for (Individual& individual: population) {
        cout << individual.tree() << endl;
    }

    return 0;
}
