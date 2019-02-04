#ifndef TREE_HPP_
#define TREE_HPP_

#include <vector>
#include <stree/stree.hpp>
#include "random.hpp"

const stree::Symbol* random_term(const stree::Environment& env,Random& rd);
const stree::Symbol* random_nonterm(const stree::Environment& env, Random& rd);

stree::Tree grow(stree::Environment& env, Random& rd, unsigned depth, float p_term = 0.2);
stree::Tree full(stree::Environment& env, Random& rd, unsigned depth);

#endif
