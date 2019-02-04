#include "tree.hpp"
#include <algorithm>
#include <cassert>

// TODO: replace random_device

const stree::Symbol* random_term(const stree::Environment& env, Random& rd) {
    assert(env.terminal_num() > 0);
    std::uniform_int_distribution<unsigned> dist(0, env.terminal_num() - 1);
    return env.terminal(dist(rd));
}

const stree::Symbol* random_nonterm(const stree::Environment& env, Random& rd) {
    assert(env.nonterminal_num() > 0);
    std::uniform_int_distribution<unsigned> dist(0, env.nonterminal_num() - 1);
    return env.nonterminal(dist(rd));
}

#define CHECK_CAN_GROW(env, depth)                                      \
    assert(depth > 0 && "Tree depth should be > 0");                    \
    assert(env.terminal_num() > 0 && "Cannot grow a tree without terminals"); \

stree::Tree grow(stree::Environment& env, Random& rd, unsigned depth, float p_term) {
    CHECK_CAN_GROW(env, depth);

    // Get random symbol
    std::uniform_real_distribution<float> dist(0, 1.0);
    bool use_term = (depth == 1 || dist(rd) < p_term || env.nonterminal_num() == 0);
    const stree::Symbol* symbol = use_term
        ? random_term(env, rd)
        : random_nonterm(env, rd);

    // Make tree
    stree::Tree tree(&env, symbol);

    // Grow subtrees
    for (stree::Arity n = 0; n < tree.arity(); ++n) {
        stree::Tree arg_tree = grow(env, rd, depth - 1, p_term);
        tree.argument(n).replace(arg_tree);
    }

    return tree;
}

stree::Tree full(stree::Environment& env, Random& rd, unsigned depth) {
    CHECK_CAN_GROW(env, depth);

    // Get random symbol
    const stree::Symbol* symbol = (depth == 1)
        ? random_term(env, rd)
        : random_nonterm(env, rd);

    // Make tree
    stree::Tree tree(&env, symbol);

    // Full subtrees
    for (stree::Arity n = 0; n < tree.arity(); ++n) {
        stree::Tree arg_tree = full(env, rd, depth - 1);
        tree.argument(n).replace(arg_tree);
    }

    return tree;
}

#undef CHECK_CAN_GROW
