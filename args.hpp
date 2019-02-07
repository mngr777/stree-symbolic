#ifndef ARGS_HPP_
#define ARGS_HPP_

#include <ostream>
#include <string>
#include "fitness.hpp"
#include "random.hpp"

struct Args {
    std::string config_file;
    unsigned population_size;
    unsigned initial_depth;
    Random::result_type prng_seed;
    float p_term;
    unsigned max_generation;
    Fitness goal;
    unsigned param_num;
    std::string fitness_cases_filename;
};

std::ostream& operator<<(std::ostream& os, const Args& args);

Args parse_args(int argc, char** argv);
bool validate_args(const Args& args);

#endif
