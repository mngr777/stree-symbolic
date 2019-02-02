#include "fitness.hpp"
#include <cmath>
#include <string>

// NOTE: assuming value is floating point number

namespace {

FitnessCase read_line(const std::string line, unsigned param_num) {
    FitnessCase fitness_case;
    std::string::size_type pos = 0;
    for (unsigned i = 0; i < param_num + 1; ++param_num) {
        // read value
        std::string::size_type pos_inc;
        stree::Value value = std::stod(line.substr(pos), &pos_inc);
        pos += pos_inc;
        // add to fitness case
        if (i < param_num) {
            fitness_case.params.push_back(value);
        } else {
            fitness_case.answer = value;
        }
    }
    return fitness_case;
}

} // namespace

FitnessCaseList load_fitness_cases(std::istream& is,  unsigned param_num) {
    FitnessCaseList case_list;
    std::string line;
    while (std::getline(is, line)) {
        case_list.push_back(read_line(line, param_num));
    }
    return case_list;
}

Fitness evaluate(const stree::Tree& tree, const FitnessCaseList& fitness_cases) {
    float fitness = 0.0;
    for (const FitnessCase& fitness_case : fitness_cases)
        fitness += evaluate(tree, fitness_case);
    return fitness;
}

Fitness evaluate(const stree::Tree& tree, const FitnessCase& fitness_case) {
    stree::Value answer = stree::eval(tree, fitness_case.params);
    return std::fabs(answer - fitness_case.answer);
}
