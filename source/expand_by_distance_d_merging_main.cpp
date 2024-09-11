#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "cube_list.hpp"
#include "distance_d_merging.hpp"
#include "indexed_cube_list.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name << " <path to cubes to expand "
     "(.clst)> <path to baseline (.icl)> <expanded cubes save path (.clst)> "
     "<maximum merge distance>\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
    using cube_list_type = cube_list_tt<32, 8>;

    if (5 != argc) {
        print_usage(argv[0]);
    }
    std::filesystem::path const to_expand_input_path(argv[1]);
    std::filesystem::path const baseline_input_path(argv[2]);
    std::filesystem::path const save_path(argv[3]);
    int const maximum_merge_distance_as_int{std::atoi(argv[4])};
    if (maximum_merge_distance_as_int < 1) {
        std::cerr << "EE: Only distance >= 1 is supported.\n";
        return EXIT_FAILURE;
    }
    unsigned int const maximum_merge_distance{
     static_cast<unsigned int>(maximum_merge_distance_as_int)};

    std::error_code const no_error;
    std::error_code error_code;
    if (std::filesystem::exists(save_path, error_code)) {
        std::cerr << "EE: " << save_path << " already exists.\n";
        return EXIT_FAILURE;
    }

    using cube_list_type = cube_list_tt<32, 8>;
    using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;

    std::optional<cube_list_type> cubes_to_expand_opt{
     cube_list_type::create_opt(to_expand_input_path)};
    if (not cubes_to_expand_opt) {
        std::cerr << "EE: " << to_expand_input_path
         << " was not succesfully read in.\n";
        return EXIT_FAILURE;
    }
    std::cerr << "II: There are " << cubes_to_expand_opt->size()
     << " cubes to expand.\n";

    std::optional<indexed_cube_list_type> baseline_cubes_opt{
     indexed_cube_list_type::create_opt(baseline_input_path)};
    if (not baseline_cubes_opt) {
        std::cerr << "EE: " << baseline_input_path
         << " was not succesfully read in.\n";
        return EXIT_FAILURE;
    }
    std::cerr << "II: There are " << baseline_cubes_opt->ncubes()
     << " baseline cubes.\n";

    for (unsigned int distance_d{1}; distance_d <= maximum_merge_distance;
     ++distance_d) {
        std::cerr << "II: About to do distance-" << distance_d << " merging.\n";
        std::size_t const eliminated_cube_count{distance_d_merge(
         *cubes_to_expand_opt, distance_d, *baseline_cubes_opt, 0)};
        std::cerr << "II: " << eliminated_cube_count
         << " cubes were eliminated.\n";
        std::cerr << "II: There are now " << cubes_to_expand_opt->size()
         << " cubes to expand.\n";
    }

    std::cerr << "II: Saving expanded cubes to " << save_path << ".\n";
    bool const error_occurred{cubes_to_expand_opt->serialize(save_path)};
    if (error_occurred) {
        std::cerr << "EE: Unable to save to " << save_path << ".\n";
        std::filesystem::path emergency_save_path(
         std::filesystem::temp_directory_path(error_code));
        emergency_save_path /= "emergency_D";
        emergency_save_path += std::to_string(maximum_merge_distance);
        emergency_save_path += ".clst";
        std::cerr << "II: Will attempt to save to " << emergency_save_path
         << ".  (No guarantees!)\n";
        (void)cubes_to_expand_opt->serialize(emergency_save_path);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
