#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "cube_list.hpp"
#include "filesystem_stuff.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name << " <path (.clst)>\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
    using cube_list_type = cube_list_tt<32, 8>;

    if (2 != argc) {
        print_usage(argv[0]);
    }
    std::filesystem::path const input_path(argv[1]);

    std::optional<cube_list_type> cube_list_opt{
     cube_list_type::create_opt(input_path)};
    if (not cube_list_opt) {
        std::cerr << "EE: " << input_path << " was not succesfully read in.\n";
        return EXIT_FAILURE;
    }
    cube_list_opt->ensure_sorted();

    std::filesystem::path temp_output_path(input_path);
    temp_output_path.replace_extension(".sorted_clst");

    std::cerr << "II: Saving to " << temp_output_path << ".\n";
    bool const write_failed{cube_list_opt->serialize(temp_output_path)};
    if (write_failed) {
        return EXIT_FAILURE;
    }

    std::filesystem::path new_input_path(input_path);
    new_input_path.replace_extension();
    new_input_path += "_before_sort.clst";
    std::error_code const no_error;
    std::error_code error_code;

    if (rename_path(input_path, new_input_path)) {
        return EXIT_FAILURE;
    }

    if (rename_path(temp_output_path, input_path)) {
        return EXIT_FAILURE;
    }

    (void)update_permissions(input_path, std::filesystem::perms::owner_read,
     std::filesystem::perm_options::replace);

    return EXIT_SUCCESS;
}
