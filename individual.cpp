#include "individual.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include "tree.hpp"

Population ramped_half_and_half(
    stree::Environment& env,
    unsigned n,
    unsigned depth,
    Random& rd,
    RandomValue* rv,
    float p_term)
{
    assert(n > 0 && "Population size should be > 0");

    Population population;
    unsigned i = 0;
    for (; i < n / 2; ++i)
        population.emplace_back(grow(env, depth, rd, rv, p_term));
    for (; i < n; ++i)
        population.emplace_back(full(env, depth, rd, rv));
    return population;
}

Fitness evaluate(Individual& individual, const FitnessCaseList& fitness_cases) {
    Fitness fitness = evaluate(individual.tree(), fitness_cases);
    individual.set_fitness(fitness);
    return fitness;
}

Fitness evaluate(Population& population, const FitnessCaseList& fitness_cases) {
    Fitness min_fitness = std::numeric_limits<Fitness>::max();
    for (auto& individual : population) {
        if (!individual.has_fitness())
            min_fitness = std::min(
                min_fitness,
                evaluate(individual, fitness_cases));
    }
    return min_fitness;
}

std::ostream& operator<<(std::ostream& os, PopulationFitness fitness) {
    return os << "Min: " << fitness.min << std::endl
              << "Max: " << fitness.max << std::endl
              << "Avg: " << fitness.avg << std::endl;
}

PopulationFitness population_fitness(const Population& population) {
    PopulationFitness result;
    Fitness sum = 0.0;
    auto num = population.size();
    for (auto& individual : population) {
        Fitness fitness = individual.fitness();
        result.min = (result.min < 0)
            ? fitness
            : std::min(result.min, fitness);
        result.max = (result.max < 0)
            ? fitness
            : std::max(result.max, fitness);
        if (std::isfinite(fitness)) {
            sum += fitness;
        } else {
            --num;
        }
    }
    result.avg = sum / num;
    return result;
}

GroupIdx random_group(const Population& population, unsigned size, Random& rd) {
    assert(size < population.size());
    std::uniform_int_distribution<IndivIdx> dist(0, population.size() - 1);
    GroupIdx group;
    while (group.size() < size) {
        IndivIdx idx = dist(rd);
        if (group.count(idx) == 0) {
            group.insert(idx);
        }
    }
    return group;
}

IndivIdx tournament(const Population& population, const GroupIdx& group) {
    auto it =  std::min_element(
        group.begin(), group.end(),
        [&population](IndivIdx idx1, IndivIdx idx2) {
            return population[idx1].fitness() < population[idx2].fitness();
        });
    return *it;
}
