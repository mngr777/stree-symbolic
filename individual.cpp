#include "individual.hpp"
#include "tree.hpp"

Population ramped_half_and_half(
    stree::Environment& env,
    Random& rd,
    unsigned n,
    unsigned depth,
    float p_term)
{
    assert(n > 0 && "Population size should be > 0");

    Population population;
    unsigned i = 0;
    for (; i < n / 2; ++i)
        population.emplace_back(grow(env, rd, depth, p_term));
    for (; i < n; ++i)
        population.emplace_back(full(env, rd, depth));
    return population;
}

void evaluate(Individual& individual, const FitnessCaseList& fitness_cases) {
    Fitness fitness = evaluate(individual.tree(), fitness_cases);
    individual.set_fitness(fitness);
}

void evaluate(Population& population, const FitnessCaseList& fitness_cases) {
    for (auto& individual : population)
        if (!individual.has_fitness())
            evaluate(individual, fitness_cases);
}
