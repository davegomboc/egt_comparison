#include "dump_cube_list_main.hpp"


#include <cstdlib>
#include <iostream>

#include "cube_list.hpp"


using cube_list_type = cube_list_tt<32, 8>;


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name << " <path>\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char * argv[]) {
    if (2 != argc) {
        print_usage(argv[0]);
    }
    std::optional<cube_list_type> const D00_egt_opt{
     cube_list_type::create_opt(argv[1])};
    if (std::nullopt == D00_egt_opt) {
        std::cerr << "EE: Unable to read D00_egt in from " << argv[1] << ".\n";
        std::exit(EXIT_FAILURE);
    }

    std::cout << *D00_egt_opt;
}
