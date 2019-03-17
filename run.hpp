#ifndef RUN_HPP_
#define RUN_HPP_

#include <memory>
#include <stree/stree.hpp>
#include "args.hpp"
#include "fitness.hpp"
#include "genetic.hpp"
#include "individual.hpp"
#include "random.hpp"

class Run {
public:
    Run(const Args& args);

    void run();

private:
    void init_random();
    void init_env();
    void init_fitness_cases();

    void init_population();
    void output_population_data(const PopulationFitness& fitness);

    void mutation();
    void crossover();
    void reproduction();

    TreeList _crossover();
    IndivIdx tournament(unsigned size);
    GroupIdx random_group(unsigned size);

    Args args_;
    std::unique_ptr<Random> rd_;
    std::unique_ptr<RandomValue> rv_;
    stree::Environment env_;
    FitnessCaseList fitness_cases_;
    unsigned generation_;
    Population population_;
    Population next_population_;
};

#endif
