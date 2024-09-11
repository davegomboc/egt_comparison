#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <utility>

#include "generic_tree.hpp"


struct pair_t {
    pair_t(short const xx, short const yy) {
        x = xx;
        y = yy;
    }
    short x, y;
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv) {
    using generic_tree_type
     = generic_tree_tt<std::pair<char const, int const>, pair_t, 24, 3>;
    generic_tree_type tree;
    tree.set_root_node_index(tree.add_internal(
     pair_t{3, 4},
     {tree.add_internal(
       pair_t{1, 1},
       {tree.add_leaf(std::make_pair('1', 1)),
        tree.add_leaf(std::make_pair('$', 14)),
        tree.add_leaf(std::make_pair('2', 2))}),
      tree.add_internal(
       pair_t{26, 26},
       {tree.add_leaf(std::make_pair('0', 0)),
        tree.add_leaf(std::make_pair('0', 0)),
        tree.add_leaf(std::make_pair('0', 0))}),
      tree.add_internal(
       pair_t{3, 3},
       {tree.add_leaf(std::make_pair('3', 3)),
        tree.add_leaf(std::make_pair('$', 14)),
        tree.add_internal(
         pair_t{'D', 4},
         {tree.add_internal(
           pair_t{'E', 5},
           {tree.add_leaf(std::make_pair('4', 4)),
            tree.add_leaf(std::make_pair('$', 14)),
            tree.add_leaf(std::make_pair('5', 5))}),
          tree.add_internal(
           pair_t{'Z', 26},
           {tree.add_leaf(std::make_pair('0', 0)),
            tree.add_leaf(std::make_pair('0', 0)),
            tree.add_leaf(std::make_pair('0', 0))}),
          tree.add_internal(
           pair_t{'B', 2},
           {tree.add_leaf(std::make_pair('1', 1)),
            tree.add_leaf(std::make_pair('$', 14)),
            tree.add_leaf(std::make_pair('2', 2))})})})}));

    std::cout << tree.to_string(
     [](pair_t const & p) {
         return std::to_string(p.x) + "x" + std::to_string(p.y);
     },
     [](std::pair<char const, int const> const & pair) {
         return std::to_string(pair.first) + ',' + std::to_string(pair.second);
     }) << std::endl;
    std::cout << "nleaves() = " << tree.nleaves() << ".\n"
     "nnodes() = " << tree.nnodes() << ".\n"
     "bits_per_node() = " << tree.bits_per_node() << ".\n\n";

    std::ofstream ofs("junk.file", std::ios::out | std::ios::binary);
    bool const write_failed{tree.serialize(ofs)};
    if (write_failed) {
        return EXIT_FAILURE;
    }
    ofs.close();

    std::ifstream ifs("junk.file", std::ios::in | std::ios::binary);
    std::optional<generic_tree_type> tree2_opt{
     generic_tree_type::create_opt(ifs)};
    assert(tree2_opt);
    std::cout << tree2_opt->to_string(
     [](pair_t const & p) {
         return std::to_string(p.x) + "x" + std::to_string(p.y);
     },
     [](std::pair<const char, const int> const & pair) {
         return std::to_string(pair.first) + ',' + std::to_string(pair.second);
     }) << std::endl;
    std::cout << "nleaves() = " << tree2_opt->nleaves() << ".\n"
     "nnodes() = " << tree2_opt->nnodes() << ".\n"
     "bits_per_node() = " << tree2_opt->bits_per_node() << ' ' << std::endl;
}
