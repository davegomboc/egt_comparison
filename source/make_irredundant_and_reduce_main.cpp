#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <random>

#include "cube_list.hpp"
#include "indexed_cube_list.hpp"
#include "irreduce_algorithm.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name
     << " <path to expanded cubes (.clst)> <path to reference set (.icl)> "
     "<output save path (.clst)> [sort = 1] [check = 1] [shuffle = 1] [seed]\n"
     "* reference set must contain only unit cubes (minterms)\n"
     "* sort: 1 = true, 0 = false\n"
     "* check: 1 = true, 0 = false\n"
     "* pre-reduce shuffle: 1 = true, 0 = false\n"
     "* seed for prng if specified, otherwise default prng seed is used\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
    using cube_list_type = cube_list_tt<32, 8>;
    using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;

    if ((4 > argc) or (argc > 8)) {
        print_usage(argv[0]);
    }
    std::filesystem::path const input_path(argv[1]);
    std::filesystem::path const reference_set_path(argv[2]);
    std::filesystem::path const output_path(argv[3]);
    bool const sort_outputs((argc < 5) or std::atoi(argv[4]));
    bool const check_outputs((argc < 6) or std::atoi(argv[5]));
    bool const shuffle_before_reduce((argc < 7) or std::atoi(argv[6]));
    std::optional<unsigned long> seed_opt;
    if (8 == argc) {
        seed_opt = std::strtoul(argv[8], nullptr, 10);
    }

    std::optional<cube_list_type> cube_list_opt{
     cube_list_type::create_opt(input_path)};
    if (not cube_list_opt) {
        std::cerr << "EE: " << input_path << " was not succesfully read in.\n";
        return EXIT_FAILURE;
    }
    std::optional<indexed_cube_list_type> reference_set_opt{
     indexed_cube_list_type::create_opt(reference_set_path)};
    if (not reference_set_opt) {
        std::cerr << "EE: " << reference_set_path
         << " was not succesfully read in.\n";
        return EXIT_FAILURE;
    }

    std::cerr << "II: Starting with " << cube_list_opt->ncubes() << " cubes.\n";
    bool const problem_found{irreduce(*cube_list_opt, *reference_set_opt, true,
     true, check_outputs, shuffle_before_reduce, seed_opt)};
    std::cerr << "II: Finished with " << cube_list_opt->ncubes() << " cubes.\n";
    if (problem_found) {
        std::cerr << "EE: Simplification error detected by checking outputs.\n";
        return EXIT_FAILURE;
    }

    if (sort_outputs) {
        std::cerr << "II: Sorting the cubes.\n";
        cube_list_opt->sort(cube_list_opt->lookup_mask());
    }

    std::cerr << "II: Saving result to " << output_path << ".\n";
    bool const write_failed{cube_list_opt->serialize(output_path)};
    if (write_failed) {
        std::cerr << "EE: " << output_path << " was not successfully saved.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
