#include "one_shot_minimize_main.hpp"


#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <system_error>

#include "cube_list.hpp"
#include "filesystem_stuff.hpp"
#include "indexed_cube_list.hpp"
#include "minimization_cycle.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name
     << " <material balance> <proc>\n";
    std::exit(EXIT_FAILURE);
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
    if (3 != argc) {
        print_usage(argv[0]);
    }
    std::string const material_balance(argv[1]);
    std::string const proc(argv[2]);

    std::string const updated_proc(proc + "+X+I");
    std::optional<std::filesystem::path> const save_path_opt{
     cube_list_path_opt(material_balance, updated_proc, false)};
    std::optional<std::filesystem::path> const D00I_baseline_input_path_opt{
     cube_list_path_opt(material_balance, "", true)};
    std::optional<std::filesystem::path> const D01I_baseline_input_path_opt{
     cube_list_path_opt(material_balance, "D01I", true)};
    std::optional<std::filesystem::path> const cubes_to_minimize_path_opt{
     cube_list_path_opt(material_balance, proc, false)};
    if ((std::nullopt == save_path_opt)
     or (std::nullopt == D00I_baseline_input_path_opt)
     or (std::nullopt == D01I_baseline_input_path_opt)
     or (std::nullopt == cubes_to_minimize_path_opt)) {
        std::cerr << "EE: Could not determine all of the input/output paths.\n";
        return EXIT_FAILURE;
    }

    std::error_code error_code;
    if (std::filesystem::exists(*save_path_opt, error_code)) {
        std::cerr << "EE: The save path " << *save_path_opt << " already exists.\n";
        return EXIT_FAILURE;
    }

    using cube_list_type = cube_list_tt<32, 8>;
    using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;

    std::optional<indexed_cube_list_type> D00I_baseline_opt{
     indexed_cube_list_type::create_opt(*D00I_baseline_input_path_opt)};
    std::cerr << "II: *D00I_baseline_input_path_opt = "
     << *D00I_baseline_input_path_opt << '\n';
    if (std::nullopt == D00I_baseline_opt) {
        std::cerr << "EE: Could not load D00I baseline.\n";
        return EXIT_FAILURE;
    }
    emit_cube_count_sentence(std::cerr, D00I_baseline_opt->ncubes(),
     "in the D00I baseline.");

    std::optional<indexed_cube_list_type> D01I_baseline_opt{
     indexed_cube_list_type::create_opt(*D01I_baseline_input_path_opt)};
    std::cerr << "II: *D01I_baseline_input_path_opt = "
     << *D01I_baseline_input_path_opt << '\n';
    if (std::nullopt == D01I_baseline_opt) {
        std::cerr << "EE: Could not load D01I baseline.\n";
        return EXIT_FAILURE;
    }
    emit_cube_count_sentence(std::cerr, D01I_baseline_opt->ncubes(),
     "in the D01I baseline.");

    std::optional<cube_list_type> cubes_to_minimize_opt{
     cube_list_type::create_opt(*cubes_to_minimize_path_opt)};
    std::cerr << "II: *cubes_to_minimize_path_opt = "
     << *cubes_to_minimize_path_opt << '\n';
    if (std::nullopt == cubes_to_minimize_opt) {
        std::cerr << "EE: Could not load cubes to minimize.\n";
        return EXIT_FAILURE;
    }
    emit_cube_count_sentence(std::cerr, cubes_to_minimize_opt->ncubes(),
     "in the ON-cover to be simplified.");

    bool const do_expand_randomly{true};
    bool const do_make_irredundant{true};
    bool const do_not_reduce{};
    bool const do_check_outputs{true};
    do_minimization_cycle<false, false, true>(*cubes_to_minimize_opt,
     std::addressof(*D00I_baseline_opt), std::addressof(*D01I_baseline_opt),
     do_expand_randomly, do_make_irredundant, do_not_reduce, do_check_outputs);
    cubes_to_minimize_opt->ensure_sorted();

    std::cerr << "II: Saving result of one-shot minimization to "
     << *save_path_opt << ".\n";
    bool const write_failed{
     cubes_to_minimize_opt->serialize(*save_path_opt)};
    if (write_failed) {
        std::cerr << "EE: Unable to save to " << *save_path_opt << ".\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
