#include "generate_distance_one_cube_lists_main.hpp"


#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <system_error>

#include "cube_list.hpp"
#include "distance_d_merging.hpp"
#include "filesystem_stuff.hpp"
#include "indexed_cube_list.hpp"
#include "irreduce_algorithm.hpp"

#include "representation/material_signature_type.hpp"


using representation::material_signature_type;

using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name << " <material_signature> <proc>\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char * argv[]) {
    if (3 != argc) {
        print_usage(argv[0]);
    }
    std::optional<material_signature_type> material_signature_opt{
     material_signature_type::create_opt(argv[1])};
    if (std::nullopt == material_signature_opt) {
        print_usage(argv[0]);
    }
    std::string const proc{argv[2]};

    std::optional<std::filesystem::path> const original_icl_path_opt{
     cube_list_path_opt(material_signature_opt->as_string(), proc, true)};
    if (std::nullopt == original_icl_path_opt) {
        std::cerr << "EE: Unable to construct original_icl_path_opt.\n";
        return EXIT_FAILURE;
    }

    std::optional<std::filesystem::path> D01I_clst_path_opt{cube_list_path_opt(
     material_signature_opt->as_string(), "D01I" + proc, false)};
    if (std::nullopt == D01I_clst_path_opt) {
        std::cerr << "EE: Unable to construct D01I_clst_path_opt.\n";
        return EXIT_FAILURE;
    }
    std::error_code error_code;
    if (std::filesystem::exists(*D01I_clst_path_opt, error_code)) {
        std::cerr << "EE: Already exists: " << *D01I_clst_path_opt << '\n';
        return EXIT_FAILURE;
    }

    std::optional<std::filesystem::path> D01I_icl_path_opt{cube_list_path_opt(
     material_signature_opt->as_string(), "D01I" + proc, true)};
    if (std::nullopt == D01I_icl_path_opt) {
        std::cerr << "EE: Unable to construct D01I_icl_path_opt.\n";
        return EXIT_FAILURE;
    }
    if (std::filesystem::exists(*D01I_icl_path_opt, error_code)) {
        std::cerr << "EE: Already exists: " << *D01I_icl_path_opt << '\n';
        return EXIT_FAILURE;
    }

    std::cerr << "II: Read .icl from " << *original_icl_path_opt
     << "\nII: Write .clst to " << *D01I_clst_path_opt
     << "\nII: Write .icl  to " << *D01I_icl_path_opt << '\n';

    std::optional<indexed_cube_list_type> original_egt_opt{
     indexed_cube_list_type::create_opt(*original_icl_path_opt)};
    if (std::nullopt == original_egt_opt) {
        std::cerr << "EE: Unable to read original_egt in from "
         << *original_icl_path_opt << ".\n";
        return EXIT_FAILURE;
    }

    no_reference_set const no_ref;
    cube_list_type mergeable_cubes{original_egt_opt->cubes()};
    std::cerr << "II: " << material_signature_opt->as_string()
     << " original cube count = " << mergeable_cubes.size() << '\n';
    std::cerr << "II: Performing a single sweep of distance-one merging.\n";
    do_single_merge_sweep(mergeable_cubes, 1, no_ref, true, false);
    std::cerr << "II: " << material_signature_opt->as_string()
     << " D01  cube count = " << mergeable_cubes.size() << '\n';
    std::cerr << "II: Performing make irredundant.\n";
    bool const problem_found{irreduce<false, false, true, true>(mergeable_cubes,
     *original_egt_opt, true, false, false)};
    std::cerr << "II: " << material_signature_opt->as_string()
     << " D01I cube count = " << mergeable_cubes.size() << '\n';
    if (problem_found) {
        std::cerr << "EE: Simplification problem detected: equivalence "
         "verification failed.\n";
        return EXIT_FAILURE;
    }
    mergeable_cubes.sort(mergeable_cubes.lookup_mask());

    auto const desired_permissions{
     std::filesystem::perms::owner_read | std::filesystem::perms::group_read};

    bool const error_occurred_writing_clst{mergeable_cubes.serialize(
     *D01I_clst_path_opt)};
    if (error_occurred_writing_clst) {
        std::cerr << "EE: Error occurred writing " << *D01I_clst_path_opt
         << '\n';
        return EXIT_FAILURE;
    }
    update_permissions(*D01I_clst_path_opt, desired_permissions,
     std::filesystem::perm_options::replace);

    indexed_cube_list_type const indexed_cubes(mergeable_cubes);
    bool const error_occurred_writing_icl{indexed_cubes.serialize(
     *D01I_icl_path_opt)};
    if (error_occurred_writing_icl) {
        std::cerr << "EE: Error occurred writing " << *D01I_icl_path_opt
         << '\n';
        return EXIT_FAILURE;
    }
    update_permissions(*D01I_icl_path_opt, desired_permissions,
     std::filesystem::perm_options::replace);
}
