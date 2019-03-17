#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <functional>
#include <ostream>
#include <set>
#include <utility>
#include <stree/stree.hpp>
#include "fitness.hpp"
#include "random.hpp"

class Individual {
public:
    Individual(stree::Tree&& tree)
        : tree_(std::move(tree)),
         fitness_(0.0),
         has_fitness_(false) {}

    stree::Tree& tree() {
        return tree_;
    }

    const stree::Tree& tree() const {
        return tree_;
    }

    void set_fitness(float fitness) {
        fitness_ = fitness;
        has_fitness_ = true;
    }

    Fitness fitness() const {
        return fitness_;
    }

    bool has_fitness() const {
        return has_fitness_;
    }

protected:
    stree::Tree tree_;
    Fitness fitness_;
    bool has_fitness_;
};


using Population = std::vector<Individual>;
using IndivIdx = Population::size_type;
using GroupIdx = std::set<IndivIdx>;

Population ramped_half_and_half(
    stree::Environment& env,
    unsigned n,
    unsigned depth,
    Random& rd,
    RandomValue* rv = nullptr,
    float p_term = 0.2);

Fitness evaluate(Individual& individual, const FitnessCaseList& fitness_cases);
Fitness evaluate(Population& population, const FitnessCaseList& fitness_cases);


struct PopulationFitness {
    PopulationFitness()
        : min(-1.0), max(-1.0), avg(-1.0) {}
    Fitness min;
    Fitness max;
    Fitness avg;
    // Fitness median;
    // Fitness var;
};

std::ostream& operator<<(std::ostream& os, PopulationFitness fitness);

PopulationFitness population_fitness(const Population& population);

GroupIdx random_group(const Population& population, unsigned size, Random& rd);

IndivIdx tournament(const Population& population, const GroupIdx& group);

#endif
