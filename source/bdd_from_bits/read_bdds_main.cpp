#include "bdd_from_bits/read_bdds_main.hpp"


#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "bdd_from_bits/fixed_decision_diagram_type.hpp"


using std::printf;


int main(int argc, char ** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " infile infile ...\n";
        return EXIT_FAILURE;
    }

    fixed_decision_diagram_type bdd;
    printf("%-40s  %12s  %12s\n", "File", "File nodes", "Forest nodes");
    printf("%-40s  %12s  %12s\n", "========================================",
     "============", "============");
    unsigned int file_total{};
    unsigned int prev_nodes{};
    for (int i = 1; i < argc; i++) {
        bdd.add_nodes_from_file(argv[i]);
        printf("%-40s  %12u  %12u\n",
            argv[i],
            bdd.file_node_count(),
            bdd.nonterminal_node_count() - prev_nodes
        );
        file_total += bdd.file_node_count();
        prev_nodes = bdd.nonterminal_node_count();
    }
    printf("%-40s  %12s  %12s\n", "========================================",
     "============", "============");
    printf("%-40s  %12u  %12u\n",
     "Total",
     file_total,
     bdd.nonterminal_node_count());

    return EXIT_SUCCESS;
}
