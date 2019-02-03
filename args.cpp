#include "args.hpp"
#include <string>
#include <getopt.h>

namespace {

Args make_args_default() {
    Args args;
    // TODO: config file
    args.population_size = 100;
    args.initial_depth   = 5;
    args.p_term          = 0.2;
    args.max_generation  = 500;
    args.goal            = 0.1;
    return args;
}

} // namespace


std::ostream& operator<<(std::ostream& os, const Args& args) {
    return os
        << "Population size: " << args.population_size << std::endl
        << "Initial tree depth: " << args.initial_depth << std::endl
        << "P(terminal), generation: " << args.p_term << std::endl
        << "Max generation number: " << args.max_generation << std::endl
        << "Fitness goal: " << args.goal << std::endl
        << "Fitness cases file: " << args.fitness_cases_filename << std::endl;
}

Args parse_args(int argc, char** argv) {
    Args args = make_args_default();

    static struct option options[] = {
        {"population-size", required_argument, 0, 'p'},
        {"initial-depth", required_argument, 0, 'd'},
        {"p-term", required_argument, 0, 't'},
        {"max-generation", required_argument, 0, 'n'},
        {"goal", required_argument, 0, 'g'},
        {"fitness-cases-file", required_argument, 0, 'f'},
        {0, 0, 0, 0}
    };
    int option_index = 0;
    char c = -1;
    while ((c = getopt_long(argc, argv, "", options, &option_index)) != -1) {
        switch (c) {
            case 'p':
                args.population_size = std::stoul(optarg);
                break;
            case 'd':
                args.initial_depth = std::stoul(optarg);
                break;
            case 't':
                args.p_term = std::stof(optarg);
                if (0.0 >= args.p_term || args.p_term >= 1.0)
                    throw std::invalid_argument("p_term must be (0.0, 1.0)");
                break;
            case 'n':
                args.max_generation = std::stoul(optarg);
                break;
            case 'g':
                args.goal = std::stod(optarg);
                if (args.goal < 0.0)
                    throw std::invalid_argument("goal must be >= 0.0");
            case 'f':
                args.fitness_cases_filename = optarg;
                break;
            case '?':
                break;
        }
    }

    return args;
}
