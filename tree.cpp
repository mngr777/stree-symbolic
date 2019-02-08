#include "tree.hpp"
#include <algorithm>
#include <cassert>

namespace {

stree::Tree random_tree(
    stree::Environment& env,
    bool use_term,
    Random& rd,
    RandomValue* rv)
{
    const stree::Symbol* symbol = use_term
        ? random_term(env, rd)
        : random_nonterm(env, rd);
    return (use_term && rv && symbol->name() == RandomValueSymbolName)
        ? stree::Tree(&env, rv->get())
        : stree::Tree(&env, symbol);
}

}// namespace

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

#define CHECK_CAN_GROW(env, depth, rv)                                     \
    assert(depth > 0 && "Tree depth should be > 0");                    \
    assert((env.terminal_num() > 0 || rv) && "Cannot grow a tree without terminals"); \

stree::Tree grow(
    stree::Environment& env,
    unsigned depth,
    Random& rd,
    RandomValue* rv,
    float p_term)
{
    CHECK_CAN_GROW(env, depth, rv);

    // Make random tree
    std::uniform_real_distribution<float> dist(0, 1.0);
    bool use_term = (depth == 1 || dist(rd) < p_term || env.nonterminal_num() == 0);
    stree::Tree tree(random_tree(env, use_term, rd, rv));

    // Grow subtrees
    for (stree::Arity n = 0; n < tree.arity(); ++n) {
        stree::Tree arg_tree = grow(env, depth - 1, rd, rv, p_term);
        tree.argument(n).replace(arg_tree);
    }

    return tree;
}

stree::Tree full(
    stree::Environment& env,
    unsigned depth,
    Random& rd,
    RandomValue* rv)
{
    CHECK_CAN_GROW(env, depth, rv);

    // Make random tree
    bool use_term = (depth == 1);
    stree::Tree tree(random_tree(env, use_term, rd, rv));

    // Full subtrees
    for (stree::Arity n = 0; n < tree.arity(); ++n) {
        stree::Tree arg_tree = full(env, depth - 1, rd, rv);
        tree.argument(n).replace(arg_tree);
    }

    return tree;
}

#undef CHECK_CAN_GROW
