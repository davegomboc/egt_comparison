#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <system_error>

#include "cube_list.hpp"
#include "filesystem_stuff.hpp"
#include "load_and_save.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name
     << " <balance> [path_to_archive] [path_to_extraction_script]\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
    if ((argc < 2) or (4 < argc)) {
        print_usage(argv[0]);
    }
    std::string const material_balance(argv[1]);
    std::optional<std::filesystem::path> archive_path_opt;
    std::optional<std::filesystem::path> extraction_script_path_opt;
    if (2 < argc) {
        archive_path_opt = argv[2];
        std::error_code error_code;
        if (not std::filesystem::exists(*archive_path_opt)) {
            std::cerr << "EE: " << *archive_path_opt << " does not exist.\n";
            return EXIT_FAILURE;
        }
    }
    if (3 < argc) {
        extraction_script_path_opt = argv[3];
        std::error_code error_code;
        if (not std::filesystem::exists(*extraction_script_path_opt)) {
            std::cerr << "EE: " << *extraction_script_path_opt
             << " does not exist.\n";
            return EXIT_FAILURE;
        }
    }
    auto const non_indexed_save_path_opt{
     cube_list_path_opt(material_balance, 0, "", false)};
    if (not non_indexed_save_path_opt) {
        std::cerr << "EE: Unable to determine where the cube list ought to be "
         "saved.\n";
        return EXIT_FAILURE;
    }
    if (create_directories_except_filename(*non_indexed_save_path_opt)) {
        std::cerr << "II: *non_indexed_save_path_opt = "
         << *non_indexed_save_path_opt << ".\n";
        return EXIT_FAILURE;
    }

    // The specified template parameters are suitable for processing endgames
    // with five or less figurines.
    using cube_list_type = cube_list_tt<32, 8>;

    cube_list_type unmerged_cube_list(
     load_chesspresso_F_plb_from_archive<32, 8>(material_balance, 3,
     archive_path_opt, extraction_script_path_opt));
    unmerged_cube_list.ensure_sorted();
    std::cerr << "II: *non_indexed_save_path_opt = "
     << *non_indexed_save_path_opt << ".\n";
    bool const write_failed{
     unmerged_cube_list.serialize(*non_indexed_save_path_opt)};
    if (write_failed) {
        return EXIT_FAILURE;
    }
    std::error_code error_code;
    std::filesystem::permissions(*non_indexed_save_path_opt,
     std::filesystem::perms::owner_read, std::filesystem::perm_options::replace,
     error_code);
    if (error_code != std::error_code()) {
        std::cerr << "WW: Attempt to set permissions of saved file resulted in "
         << error_code << ".\n";
    }
    return EXIT_SUCCESS;
}
