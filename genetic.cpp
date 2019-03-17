#include "genetic.hpp"
#include <cassert>
#include "tree.hpp"

namespace {

stree::Subtree random_subtree(stree::Tree& tree, Random& rd, float p_term);

} // namespace


TreeList crossover_random(
    stree::Tree tree1,
    stree::Tree tree2,
    Random& rd,
    float p_term)
{
    stree::Subtree subtree1 = random_subtree(tree1, rd, p_term);
    stree::Subtree subtree2 = random_subtree(tree2, rd, p_term);
    subtree1.swap(subtree2);
    return TreeList({std::move(tree1), std::move(tree2)});
}

TreeList crossover_one_point(
    stree::Tree tree1,
    stree::Tree tree2,
    Random& rd,
    float p_term,
    bool *success)
{
    // NOTE: ignoring p_term for now
    if (success)
        (*success) = false;
    stree::Environment* env = tree1.env();
    assert(tree2.env() == env);
    stree::CommonRegion common_region = stree::common_region(
        tree1.env(),
        tree1.root(),
        tree2.root(),
        stree::NodeCompare(env));
    if (common_region.size() > 0) {
        // Swap subtrees in common region
        std::uniform_int_distribution<stree::NodeNum> dist(0, common_region.size() - 1);
        stree::NodeNum idx1 = dist(rd);
        stree::NodeNum idx2 = dist(rd);
        stree::Subtree subtree1(env, nullptr, common_region[idx1].node1);
        stree::Subtree subtree2(env, nullptr, common_region[idx2].node2);
        subtree1.swap(subtree2);
        // Success if nodes are not both roots
        if (success)
            (*success) = (common_region[idx1].n != 0 && common_region[idx2].n != 0);
    }
    return TreeList({std::move(tree1), std::move(tree2)});
}

void mutate_headless(
    stree::Tree& tree,
    unsigned depth,
    Random& rd,
    RandomValue* rv,
    float p_term,
    float p_term_grow)
{
    stree::Tree chicken = grow(*tree.env(), depth, rd, rv, p_term_grow);
    stree::Subtree subtree = random_subtree(tree, rd, p_term);
    subtree.replace(chicken);
}


namespace {

stree::Subtree random_subtree(stree::Tree& tree, Random& rd, float p_term) {
    assert(tree.is_valid());
    bool use_term = (tree.describe().nonterm_num == 0)
        || std::bernoulli_distribution{p_term}(rd);
    stree::NodeNum n_max = use_term
        ? tree.describe().term_num - 1
        : tree.describe().nonterm_num - 1;
    stree::NodeFilter filter;
    filter.is_terminal = use_term ? stree::IsTerminalYes : stree::IsTerminalNo;
    return tree.sub(
        std::uniform_int_distribution<stree::NodeNum>{0, n_max}(rd),
        filter);
}

} // namespace
