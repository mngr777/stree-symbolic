#ifndef GENETIC_HPP_
#define GENETIC_HPP_

#include <stree/stree.hpp>
#include "random.hpp"

stree::Subtree random_subtree(Random& rd, stree::Tree& tree, float p_term);

std::vector<stree::Tree> crossover_one_point(
    Random& rd,
    stree::Tree tree1,
    stree::Tree tree2,
    float p_term = 0.2);


#endif
