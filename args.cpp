#include "args.hpp"
#include <fstream>
#include <istream>
#include <random>
#include <regex>
#include <stdexcept>
#include <string>
#include <getopt.h>

// param-name = value
static char ConfigLineRegex[] = "^\\s*([a-z-]+)\\s*(=\\s*(.*))?\\s*$";
static unsigned ConfigLineRegexNameIdx = 1;
static unsigned ConfigLineRegexValueIdx = 3;

static char ConfigFileArg[]           = "config-file";
static char PopulationSizeArg[]       = "population-size";
static char InitialDepthArg[]         = "initial-depth";
static char PrngSeedArg[]             = "prng-seed";
static char PTermArg[]                = "p-term";
static char MaxGenerationArg[]        = "max-generation";
static char GoalArg[]                 = "goal";
static char ParamNumArg[]             = "param-num";
static char FitnessCasesFilenameArg[] = "fitness-cases-file";

namespace {

Args make_args_default();

void read_unsigned(
    unsigned& arg, const std::string& value,
    unsigned min = 1, unsigned max = -1);

void read_seed(Random::result_type& arg, const std::string value);
void read_probablity(float& arg, const std::string& value);
void read_goal(float& arg, const std::string& value);

void read_config_line(Args& args, const std::string& line);

void load_config(Args& args, std::istream& is);
void load_config(Args& args, const std::string& filename);

} // namespace


std::ostream& operator<<(std::ostream& os, const Args& args) {
    return os
        << "Config file: " << args.config_file << std::endl
        << "Population size: " << args.population_size << std::endl
        << "Initial tree depth: " << args.initial_depth << std::endl
        << "PRNG seed: " << args.prng_seed << std::endl
        << "P(terminal), generation: " << args.p_term << std::endl
        << "Max generation number: " << args.max_generation << std::endl
        << "Fitness goal: " << args.goal << std::endl
        << "Parameter number: "<< args.param_num << std::endl
        << "Fitness cases file: " << args.fitness_cases_filename << std::endl;
}

Args parse_args(int argc, char** argv) {
    Args args = make_args_default();

    // TODO: replace literals with Param* constants
    static struct option options[] = {
        {ConfigFileArg, required_argument, 0, 'c'},
        {PopulationSizeArg, required_argument, 0, 'm'},
        {InitialDepthArg, required_argument, 0, 'd'},
        {PrngSeedArg, required_argument, 0, 's'},
        {PTermArg, required_argument, 0, 't'},
        {MaxGenerationArg, required_argument, 0, 'n'},
        {GoalArg, required_argument, 0, 'g'},
        {ParamNumArg, required_argument, 0, 'p'},
        {FitnessCasesFilenameArg, required_argument, 0, 'f'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    char c = -1;
    try {
        while ((c = getopt_long(argc, argv, "", options, &option_index)) != -1) {
            switch (c) {
                case 'c':
                    args.config_file = optarg;
                    load_config(args, args.config_file);
                    break;
                case 'm':
                    read_unsigned(args.population_size, optarg);
                    break;
                case 'd':
                    read_unsigned(args.initial_depth, optarg);
                    break;
                case 's':
                    read_seed(args.prng_seed, optarg);
                    break;
                case 't':
                    read_probablity(args.p_term, optarg);
                    break;
                case 'n':
                    read_unsigned(args.max_generation, optarg);
                    break;
                case 'g':
                    read_goal(args.goal, optarg);
                    break;
                case 'p':
                    read_unsigned(args.param_num, optarg);
                    break;
                case 'f':
                    args.fitness_cases_filename = optarg;
                    break;
                case '?':
                    break;
            }
        }
    } catch (std::invalid_argument& e) {
        // re-throw with argument name
        // ?? is option_index always valid?
        throw std::invalid_argument(
            std::string("`") + options[option_index].name + "': "
            + e.what());
    }

    // Random seed if not provided
    if (args.prng_seed == -1) {
        args.prng_seed = std::random_device{}();
    }

    return args;
}

void validate_args(const Args& args) {
    std::string message;
    // param-num
    if (args.param_num == -1)
        message += "--param-num argument is required\n";
    // fitness-cases-file
    if (args.fitness_cases_filename.empty())
        message += "--fitness-cases-file argument is required\n";
    if (!message.empty())
        throw std::invalid_argument(message);
}


namespace {

Args make_args_default() {
    Args args;
    args.population_size = 100;
    args.initial_depth   = 5;
    args.prng_seed       = -1;
    args.p_term          = 0.2;
    args.max_generation  = 500;
    args.goal            = 0.1;
    args.param_num       = -1;
    return args;
}

void read_unsigned(
    unsigned& arg, const std::string& value,
    unsigned min, unsigned max)
{
    arg = std::stoul(value);
    if (min > 0 && arg < min) {
        throw std::invalid_argument(
            std::string("Min value is ") + std::to_string(min) + ", "
            + value + " provided");
    } else if (max < -1 && arg > max) {
        throw std::invalid_argument(
            std::string("Max value is ") + std::to_string(max) + ", "
            + value + " provided");
    }
}

void read_seed(Random::result_type& arg, const std::string value) {
    arg = std::stoul(value);
}

void read_probablity(float& arg, const std::string& value) {
    arg = std::stof(value);
    if (0.0 >= arg || arg >= 1.0)
        throw std::invalid_argument("probability must be (0.0, 1.0)");
}

void read_goal(float& arg, const std::string& value) {
    arg = std::stof(value);
    if (arg < 0.0)
        throw std::invalid_argument("goal must be >= 0.0");
}

void read_config_line(Args& args, const std::string& line) {
    static std::regex re(ConfigLineRegex);
    std::smatch matches;
    if (!std::regex_match(line, matches, re))
        throw std::invalid_argument("Invalid config line format");
    const std::string& name = matches[ConfigLineRegexNameIdx];
    const std::string& value = matches[ConfigLineRegexValueIdx];

    bool name_found = true;
    try {
        if (name == PopulationSizeArg) {
            read_unsigned(args.population_size, value);
        } else if (name == InitialDepthArg) {
            read_unsigned(args.initial_depth, value);
        } else if (name == PrngSeedArg) {
            read_seed(args.prng_seed, value);
        } else if (name == PTermArg) {
            read_probablity(args.p_term, value);
        } else if (name == MaxGenerationArg) {
            read_unsigned(args.max_generation, value);
        } else if (name == GoalArg) {
            read_goal(args.goal, value);
        } else if (name == ParamNumArg) {
            read_unsigned(args.param_num, value);
        } else if (name == FitnessCasesFilenameArg) {
            args.fitness_cases_filename = value;
        } else {
            name_found = false;
        }
    } catch (std::invalid_argument& e) {
        // re-throw with argument name
        throw std::invalid_argument(std::string("`") + name + "': "+ e.what());
    }

    if (!name_found) {
        throw std::invalid_argument(
            std::string("Invalid argument name: `" + name + "'"));
    }
}

void load_config(Args& args, std::istream& is) {
    std::string line;
    unsigned linum = 0;
    while (std::getline(is, line)) {
        ++linum;
        if (line.empty() || line[0] == '#')
            continue;
        try {
            read_config_line(args, line);
        } catch (std::exception& e) {
            throw std::invalid_argument(
                std::string(e.what()) + "\nline number: " + std::to_string(linum));
        }
    }
}

void load_config(Args& args, const std::string& filename) {
    std::ifstream is(filename);
    load_config(args, is);
}

} // namespace
