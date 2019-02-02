#ifndef TREE_HPP_
#define TREE_HPP_

#include <random>
#include <vector>
#include <stree/stree.hpp>
#include "individual.hpp"

const stree::Symbol* random_term(
    const stree::Environment& env,
    std::random_device& rd);

const stree::Symbol* random_nonterm(
    const stree::Environment& env,
    std::random_device& rd);

stree::Tree grow(
    stree::Environment& env,
    std::random_device& rd,
    unsigned depth,
    float p_term);

stree::Tree full(
    stree::Environment& env,
    std::random_device& rd,
    unsigned depth);

#endif
