#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string_view>

#include "decision_dag.hpp"
#include "make_decision_dag.hpp"


int main(int argc, char ** argv) {
    if ((argc < 2) or (3 < argc)) {
        std::cerr << "EE: Incorrect usage.\n";
        return EXIT_FAILURE;
    }
    std::string_view const material_balance(argv[1]);
    std::optional<std::filesystem::path> archive_path_opt;
    if (3 == argc) {
        archive_path_opt = argv[2];
    }
    ddag_type const dt(get_decision_dag<32, 8, 27>(material_balance,
     archive_path_opt));
    std::cerr << "II: node count = " << dt.nnodes() << "\nII: byte count = "
     << dt.nbytes() << '\n';
}
