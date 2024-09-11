#include "verify_equivalence_main.hpp"


#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "cube_list.hpp"
#include "data_category_enum.hpp"
#include "indexed_cube_list.hpp"
#include "verify_equivalence.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name
     << " <path to simplified (.clst or .icl)> <path to baseline (.clst)>\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
    using cube_list_type = cube_list_tt<32, 8>;

    if (3 != argc) {
        print_usage(argv[0]);
    }
    std::filesystem::path const simplified_input_path(argv[1]);
    std::filesystem::path const baseline_input_path(argv[2]);

    std::optional<data_category_enum> const simplified_data_category_opt{
     categorize_by_extension_opt(simplified_input_path)};

    using cube_list_type = cube_list_tt<32, 8>;
    using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;

    if ((data_category_enum::indexed_cube_list != simplified_data_category_opt)
     and (data_category_enum::cube_list != simplified_data_category_opt)) {
        print_usage(argv[0]);
    }

    std::optional<indexed_cube_list_type> indexed_simplified_cubes_opt;
    std::optional<cube_list_type> simplified_cubes_opt;
    if (data_category_enum::indexed_cube_list == simplified_data_category_opt) {
        indexed_simplified_cubes_opt
         = indexed_cube_list_type::create_opt(simplified_input_path);
        if (not indexed_simplified_cubes_opt) {
            std::cerr << "EE: " << simplified_input_path
             << " was not succesfully read in.\n";
            return EXIT_FAILURE;
        }
    } else if (data_category_enum::cube_list == simplified_data_category_opt) {
        simplified_cubes_opt
         = cube_list_type::create_opt(simplified_input_path);
        if (not simplified_cubes_opt) {
            std::cerr << "EE: " << simplified_input_path
             << " was not succesfully read in.\n";
            return EXIT_FAILURE;
        }
    } else {
        print_usage(argv[0]);
    }
    assert(indexed_simplified_cubes_opt.has_value()
     xor simplified_cubes_opt.has_value());

    std::optional<cube_list_type> baseline_cubes_opt{
     cube_list_type::create_opt(baseline_input_path)};
    if (not baseline_cubes_opt) {
        std::cerr << "EE: " << baseline_input_path
         << " was not succesfully read in.\n";
        return EXIT_FAILURE;
    }

    std::cerr << "II: Verifying that simplified provides outputs equivalent to "
     "baseline.\n";
    bool const problem_found{simplified_cubes_opt.has_value() ?
     verify_equivalence(*simplified_cubes_opt, *baseline_cubes_opt) :
     verify_equivalence(*indexed_simplified_cubes_opt, *baseline_cubes_opt)};

    if (problem_found) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
