#include "generate_material_signatures_main.hpp"


#include <algorithm>
#include <iostream>
#include <vector>

#include "representation/enumerated_material_signatures_type.hpp"
#include "representation/material_signature_type.hpp"


using representation::enumerated_material_signatures_type;
using representation::material_signature_type;


int main(int argc, char * argv[]) {
    unsigned int highest_figurine_count{2};
    if (argc > 2) {
        std::cerr << "EE: Too many command-line arguments.\n";
    }
    if (argc == 2) {
        int value{std::atoi(argv[1])};
        if ((value < 2) or (value > 32)) {
            std::cerr << "EE: Invalid command-line argument.\n";
            return EXIT_FAILURE;
        }
        if (value > 8) {
            std::cerr << "EE: Not yet implemented.\n";
            return EXIT_FAILURE;
        }
        highest_figurine_count = static_cast<unsigned int>(value);
    }
    enumerated_material_signatures_type matsigs(highest_figurine_count);
    std::vector<material_signature_type> const & combos{
     matsigs.material_signatures(highest_figurine_count)};
    std::for_each(combos.cbegin(), combos.cend(),
     [&](material_signature_type const & material_signature) {
        std::cout << material_signature << '\n';
    });
    std::cout << std::flush;
}
