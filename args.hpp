#ifndef ARGS_HPP_
#define ARGS_HPP_

#include <ostream>
#include <string>
#include "fitness.hpp"
#include "random.hpp"

struct Args {
    bool show_help;
    std::string config_file;
    unsigned population_size;
    unsigned initial_depth;
    Random::result_type prng_seed;
    float p_term;
    unsigned max_generation;
    Fitness goal;
    unsigned param_num;
    std::string fitness_cases_filename;
    unsigned tournament_size;
    unsigned crossover_tournament_size;
};

std::ostream& operator<<(std::ostream& os, const Args& args);

Args parse_args(int argc, char** argv);
void validate_args(const Args& args);

#endif
