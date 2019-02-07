#ifndef FITNESS_HPP_
#define FITNESS_HPP_

#include <istream>
#include <vector>
#include <stree/stree.hpp>

struct FitnessCase {
    stree::Params params;
    stree::Value answer;
};

using FitnessCaseList = std::vector<FitnessCase>;
using Fitness = float;

FitnessCaseList load_fitness_cases(const std::string& filename, unsigned param_num);
FitnessCaseList load_fitness_cases(std::istream& is, unsigned param_num);

// TODO: add fitness combine function as a parameter
Fitness evaluate(const stree::Tree& tree, const FitnessCaseList& fitness_cases);
Fitness evaluate(const stree::Tree& tree, const FitnessCase& fitness_case);


#endif
