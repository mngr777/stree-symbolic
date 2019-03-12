#ifndef GENETIC_HPP_
#define GENETIC_HPP_

#include <vector>
#include <stree/stree.hpp>
#include "random.hpp"

using TreeList = std::vector<stree::Tree>;

TreeList crossover_random(
    stree::Tree tree1,
    stree::Tree tree2,
    Random& rd,
    float p_term = 0.2);

TreeList crossover_one_point(
    stree::Tree tree1,
    stree::Tree tree2,
    Random& rd,
    float p_term = 0.2,
    bool *success = nullptr);

// TODO: same depth by default (do not mess up references)
void mutate_headless(
    stree::Tree& tree,
    unsigned depth,
    Random& rd,
    RandomValue* rv = nullptr,
    float p_term = 0.2,
    float p_term_grow = 0.2);

#endif
