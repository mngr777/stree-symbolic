#include "genetic.hpp"
#include <cassert>
#include "tree.hpp"

namespace {

stree::Subtree random_subtree(stree::Tree& tree, Random& rd, float p_term);

} // namespace


TreeList crossover_one_point(
    stree::Tree tree1,
    stree::Tree tree2,
    Random& rd,
    float p_term)
{
    stree::Subtree subtree1 = random_subtree(tree1, rd, p_term);
    stree::Subtree subtree2 = random_subtree(tree2, rd, p_term);
    subtree1.swap(subtree2);
    return std::vector<stree::Tree>({std::move(tree1), std::move(tree2)});
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
