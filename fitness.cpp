#include "fitness.hpp"
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>

// NOTE: assuming value is floating point number

namespace {

FitnessCase read_line(const std::string& line, unsigned param_num);

} // namespace


FitnessCaseList load_fitness_cases(const std::string& filename, unsigned param_num) {
    std::ifstream is(filename, std::ifstream::out);
    if (!is.is_open())
        throw std::invalid_argument(
            std::string("Cannot open fitness case file `") + filename + "'");
    return load_fitness_cases(is, param_num);
}

FitnessCaseList load_fitness_cases(std::istream& is,  unsigned param_num) {
    FitnessCaseList case_list;
    // Process each line
    std::string line;
    unsigned linum = 0;
    while (std::getline(is, line)) {
        ++linum;
        if (line.empty() || line[0] == '#')
            continue;
        try {
            case_list.push_back(read_line(line, param_num));
        } catch (std::exception& e) {
            throw std::invalid_argument(
                std::string(e.what())
                + "\nline number: " + std::to_string(linum) + "\n"
                + line);
        }
    }
    // Check if list is empty
    if (case_list.empty())
        throw std::invalid_argument("Fitness case file is empty");

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


namespace {

FitnessCase read_line(const std::string& line, unsigned param_num) {
    FitnessCase fitness_case;
    std::string::size_type pos = 0;
    for (unsigned i = 0; i < param_num + 1; ++i) {
        if (pos + 1 > line.size())
            throw std::invalid_argument("Not enough parameters");

        // read value
        std::string::size_type pos_inc;
        stree::Value value = static_cast<stree::Value>(
            std::stod(line.substr(pos), &pos_inc));
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
