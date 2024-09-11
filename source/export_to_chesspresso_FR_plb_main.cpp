#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "cube_list.hpp"
#include "filesystem_stuff.hpp"
#include "load_and_save.hpp"


using cube_list_type = cube_list_tt<32, 8>;


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name << " material_balance "
     "proc_for_ON_cover proc_for_OFF_cover output_path_segment_name\n    or "
     << executable_name << " output_path F_cube_list_path R_cube_list_path\n";
    std::exit(EXIT_FAILURE);
}

void export_fr_bin(
    std::filesystem::path const & output_path,
    std::filesystem::path const & F_cube_list_path,
    std::filesystem::path const & R_cube_list_path
) {
    std::optional<cube_list_type> const F_cube_list_opt{
     cube_list_type::create_opt(F_cube_list_path)};
    if (std::nullopt == F_cube_list_opt) {
        std::cerr << "EE: Unable to retrieve F_cube_list from path "
         << F_cube_list_path << ".\n";
        std::exit(EXIT_FAILURE);
    }
    std::optional<cube_list_type> R_cube_list_opt{
     cube_list_type::create_opt(R_cube_list_path)};
    if (std::nullopt == R_cube_list_opt) {
        std::cerr << "EE: Unable to retrieve R_cube_list from path "
         << R_cube_list_path << ".\n";
        std::exit(EXIT_FAILURE);
    }
    save_chesspresso_FR_plb(output_path, *F_cube_list_opt, *R_cube_list_opt);
    (void)update_permissions(output_path, std::filesystem::perms::owner_read,
     std::filesystem::perm_options::replace);
}

void export_fr_bin(
    std::string_view const material_balance,
    std::string_view const proc_for_F,
    std::string_view const proc_for_R,
    [[maybe_unused]] std::optional<std::string_view> const output_path_segment_name_opt
) {
    std::optional<std::filesystem::path> output_path_opt(chesspresso_path_opt(
     material_balance));
    if (std::nullopt == output_path_opt) {
        std::cerr << "EE: Unable to identify an appropriate output path.\n";
        std::exit(EXIT_FAILURE);
    }
    std::cerr << "II: output_path = " << *output_path_opt << '\n';

    std::optional<std::filesystem::path> const F_cube_list_path_opt(
     cube_list_path_opt(material_balance, proc_for_F, false));
    if (std::nullopt == F_cube_list_path_opt) {
        std::cerr << "EE: Unable to determine load path for the F cube list.\n";
        std::exit(EXIT_FAILURE);
    }
    std::cerr << "II: F_cube_list_path = " << *F_cube_list_path_opt << '\n';

    std::optional<std::filesystem::path> const R_cube_list_path_opt(
     cube_list_path_opt(material_balance, proc_for_R, false));
    if (std::nullopt == R_cube_list_path_opt) {
        std::cerr << "EE: Unable to determine load path for the R cube list.\n";
        std::exit(EXIT_FAILURE);
    }
    std::cerr << "II: R_cube_list_path = " << *R_cube_list_path_opt << '\n';

    export_fr_bin(*output_path_opt, *F_cube_list_path_opt,
     *R_cube_list_path_opt);
    (void)update_permissions(*output_path_opt,
     std::filesystem::perms::owner_read,
     std::filesystem::perm_options::replace);
}

int main(int argc, char ** argv) {
    // TODO: Fix this.  User must use "" when there is no
    // output_path_segment_name to avoid getting the wrong overload called.
    // However, an output_path_segment_name isn't even supported anymore.
    if (4 == argc) {
        export_fr_bin(argv[1], argv[2], argv[3]);
    } else if (5 == argc) {
        export_fr_bin(argv[1], argv[2], argv[3], argv[4]);
    } else {
        print_usage(argv[0]);
    }
}
