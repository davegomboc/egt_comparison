#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "cube_list.hpp"
#include "distance_d_merging.hpp"
#include "indexed_cube_list.hpp"
#include "irreduce_algorithm.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name
     << " <material balance> <proc> <maximum merge distance>\n";
    std::exit(EXIT_FAILURE);
}


std::optional<std::filesystem::path> serialization_path_opt(
    std::string const & material_balance,
    std::string const & original_proc,
    unsigned int const distance_d
) {
    std::string proc;
    if (original_proc.empty()) {
        if (0 < distance_d) {
            proc = "D";
            if (distance_d < 10) {
                proc += '0';
            }
            proc += std::to_string(distance_d);
            proc += 'I';
        }
    } else {
        proc = original_proc;
        if (0 < distance_d) {
            proc += "_D";
            if (distance_d < 10) {
                proc += '0';
            }
            proc += std::to_string(distance_d);
            proc += 'I';
        }
    }
    return cube_list_path_opt(material_balance, proc, false);
}


void emit_cube_count_sentence(
    std::ostream & out,
    std::size_t const cube_count,
    std::string_view const trailing_text
) {
    bool const plural(1 != cube_count);
    out << "II: There " << (plural ? "are " : "is ") << cube_count
     << (plural ? " cubes " : " cube ") << trailing_text << '\n';
}


int main(int argc, char ** argv) {
    using cube_list_type = cube_list_tt<32, 8>;

    if (4 != argc) {
        print_usage(argv[0]);
    }
    int const maximum_merge_distance_as_int{std::atoi(argv[3])};
    if (maximum_merge_distance_as_int < 2) {
        std::cerr << "EE: Only distance >= 2 is supported.\n";
        return EXIT_FAILURE;
    }
    unsigned int const maximum_merge_distance{
     static_cast<unsigned int>(maximum_merge_distance_as_int)};

    std::string material_balance(argv[1]);
    std::string proc(argv[2]);
    std::optional<std::filesystem::path> const original_simplify_input_path_opt{
     cube_list_path_opt(material_balance, proc, false)};
    std::optional<std::filesystem::path> const unprocessed_baseline_input_path_opt{
     cube_list_path_opt(material_balance, "", true)};
    std::optional<std::filesystem::path> const D01I_baseline_input_path_opt{
     cube_list_path_opt(material_balance, "D01I", true)};
    if ((std::nullopt == original_simplify_input_path_opt)
     or (std::nullopt == unprocessed_baseline_input_path_opt)
     or (std::nullopt == D01I_baseline_input_path_opt)) {
        std::cerr << "EE: Could not determine input paths.\n";
        return EXIT_FAILURE;
    }

    using cube_list_type = cube_list_tt<32, 8>;
    using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;

    std::cerr << "II: *unprocessed_baseline_input_path_opt = "
     << *unprocessed_baseline_input_path_opt << '\n';
    std::optional<indexed_cube_list_type> unprocessed_baseline_opt{
     indexed_cube_list_type::create_opt(*unprocessed_baseline_input_path_opt)};
    if (std::nullopt == unprocessed_baseline_opt) {
        std::cerr << "EE: Could not load unprocessed baseline.\n";
        return EXIT_FAILURE;
    }
    emit_cube_count_sentence(std::cerr, unprocessed_baseline_opt->ncubes(),
     "in the unprocessed baseline.");

    std::cerr << "II: *D01I_baseline_input_path_opt = "
     << *D01I_baseline_input_path_opt << '\n';
    std::optional<indexed_cube_list_type> D01I_baseline_opt{
     indexed_cube_list_type::create_opt(*D01I_baseline_input_path_opt)};
    if (std::nullopt == D01I_baseline_opt) {
        std::cerr << "EE: Could not load D01I baseline.\n";
        return EXIT_FAILURE;
    }
    emit_cube_count_sentence(std::cerr, D01I_baseline_opt->ncubes(),
     "in the D01I baseline.");

    std::optional<std::filesystem::path> simplify_input_path_opt;
    unsigned int already_done_merge_distance{maximum_merge_distance};
    while (1 < already_done_merge_distance) {
        simplify_input_path_opt = serialization_path_opt(material_balance, proc,
         already_done_merge_distance);
        if (std::nullopt == simplify_input_path_opt) {
            std::cerr << "EE: Unable to generate load path.\n";
            return EXIT_FAILURE;
        }
        std::error_code error_code;
        if (std::filesystem::exists(*simplify_input_path_opt, error_code)) {
            std::cerr << "II: Found " << *simplify_input_path_opt
             << " to resume from.\n";
            break;
        }
        --already_done_merge_distance;
    }
    if (1 == already_done_merge_distance) {
        simplify_input_path_opt = original_simplify_input_path_opt;
    }

    std::cerr << "II: *simplify_input_path_opt = " << *simplify_input_path_opt
     << '\n';
    std::optional<cube_list_type> simplify_opt{
     cube_list_type::create_opt(*simplify_input_path_opt)};
    if (std::nullopt == simplify_opt) {
        std::cerr << "EE: Could not load cubes to simplify.\n";
        return EXIT_FAILURE;
    }
    simplify_opt->ensure_sorted();
    emit_cube_count_sentence(std::cerr, simplify_opt->ncubes(),
     "in the function cover to be simplified.");

    for (unsigned int distance_d{1 + already_done_merge_distance};
     distance_d <= maximum_merge_distance; ++distance_d) {
        std::cerr << "II: About to do one sweep of distance-" << distance_d
         << " merging.\n";
        std::size_t const eliminated_cube_count{do_single_merge_sweep(
         *simplify_opt, distance_d, *unprocessed_baseline_opt, true, false)};
        std::cerr << "II: " << eliminated_cube_count
         << " cubes were eliminated.\n";
        std::cerr << "II: About to make irredundant.\n";
        (void)irreduce<false, false, true, true>(*simplify_opt,
         *unprocessed_baseline_opt, true, false, false);
        emit_cube_count_sentence(std::cerr, simplify_opt->ncubes(),
         "in the function cover to be simplified.");

        std::optional<std::filesystem::path> save_path_opt{
         serialization_path_opt(material_balance, proc, distance_d)};
        if (std::nullopt == save_path_opt) {
            // This condition should rarely occur, given that the same code is
            // used to determine the paths to load from earlier.  The filesystem
            // would need to get screwed up while the program was running.
            std::cerr << "EE: Unable to generate save path.\n";
            return EXIT_FAILURE;
        }
        simplify_opt->ensure_sorted();
        std::cerr << "II: Saving to " << *save_path_opt << ".\n";
        bool const write_failed{simplify_opt->serialize(*save_path_opt)};
        if (write_failed) {
            std::cerr << "EE: Unable to save to " << *save_path_opt << ".\n";
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
