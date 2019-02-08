#ifndef TREE_HPP_
#define TREE_HPP_

#include <vector>
#include <stree/stree.hpp>
#include "random.hpp"

static const char RandomValueSymbolName[] = "<random>";

const stree::Symbol* random_term(const stree::Environment& env, Random& rd);
const stree::Symbol* random_nonterm(const stree::Environment& env, Random& rd);

stree::Tree grow(
    stree::Environment& env,
    unsigned depth,
    Random& rd,
    RandomValue* rv = nullptr,
    float p_term = 0.2);

stree::Tree full(
    stree::Environment& env,
    unsigned depth,
    Random& rd,
    RandomValue* rv = nullptr);

#endif
