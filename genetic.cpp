#include "genetic.hpp"
#include <cassert>

// TODO: replace random_device

stree::Subtree random_subtree(Random& rd, stree::Tree& tree, float p_term) {
    assert(tree.is_valid());
    bool use_term = (tree.describe().nonterm_num == 0)
        || std::uniform_real_distribution<float>{0, 1.0}(rd);
    stree::NodeNum n_max = use_term
        ? tree.describe().term_num - 1
        : tree.describe().nonterm_num - 1;
    return tree.sub(
        std::uniform_int_distribution<stree::NodeNum>{0, n_max}(rd),
        use_term ? stree::IsTerminalYes : stree::IsTerminalNo);
}

std::vector<stree::Tree> crossover_one_point(
    Random& rd,
    stree::Tree tree1,
    stree::Tree tree2,
    float p_term)
{
    stree::Subtree subtree1 = random_subtree(rd, tree1, p_term);
    stree::Subtree subtree2 = random_subtree(rd, tree2, p_term);
    subtree1.swap(subtree2);
    return std::vector<stree::Tree>({std::move(tree1), std::move(tree2)});
}
