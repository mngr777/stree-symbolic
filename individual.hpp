#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <random>
#include <utility>
#include <stree/stree.hpp>
#include "fitness.hpp"

class Individual {
public:
    Individual(stree::Tree&& tree)
        :tree_(std::move(tree)),
         fitness_(0.0),
         has_fitness_(false) {}

    Individual(const Individual&) = delete;
    Individual(Individual&&) = default;

    Individual& operator=(const Individual&) = delete;
    Individual& operator=(Individual&&) = default;

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
using Group = std::vector<Individual>;

Population ramped_half_and_half(
    stree::Environment& env,
    std::random_device& rd,
    unsigned n,
    unsigned depth,
    float p_term = 0.2);

void evaluate(Individual& individual, const FitnessCaseList& fitness_cases);

// ?? is this function required
void evaluate(Population& population, const FitnessCaseList& fitness_cases);

#endif
