#ifndef ARGS_HPP_
#define ARGS_HPP_

#include <ostream>
#include "fitness.hpp"
#include "random.hpp"

struct Args {
    unsigned population_size;
    unsigned initial_depth;
    Random::result_type prng_seed;
    float p_term;
    unsigned max_generation;
    Fitness goal;
    std::string fitness_cases_filename;
};

std::ostream& operator<<(std::ostream& os, const Args& args);

Args parse_args(int argc, char** argv);

#endif
