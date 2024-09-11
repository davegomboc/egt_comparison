#include "bdd_from_bits/read_bdd_main.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

#include "bdd_from_bits/fixed_decision_diagram_type.hpp"
#include "bdd_from_bits/read_bdd.hpp"


int main(int argc, char ** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " infile\n";
        return EXIT_FAILURE;
    }

    fixed_decision_diagram_type bdd;
    unsigned int const root_node{bdd.add_nodes_from_file(argv[1])};
//  std::cerr << "Got root " << root_node << "\n";
    std::cerr << "Read BDD with " << bdd.nonterminal_node_count()
     << " non-terminal nodes\n";
    std::cerr << "Enter queries, one per line, eof to stop\n";

    std::vector<unsigned char> vars(bdd.level_count() + 1, 0);

    while (get_query(std::cin, vars, bdd.level_count())) {
        show_query(std::cout, vars, bdd.level_count(), bdd.evaluate(root_node,
         vars));
    }

    std::cerr << "Done\n";
    return EXIT_SUCCESS;
}
