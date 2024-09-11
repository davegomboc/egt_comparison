#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <optional>
#include <mutex>

#include "indexed_cube_list.hpp"
#include "load_and_save.hpp"
#include "parallel_for_each.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;

[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name << " <balance> [proc]\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
    if ((2 != argc) and (3 != argc)) {
        print_usage(argv[0]);
    }
    std::string const material_balance(argv[1]);
    std::string const second_arg{argc > 2 ? argv[2] : ""};
    std::optional<indexed_cube_list_type> const indexed_cube_list_opt{
     load_indexed_cube_list_opt(material_balance, second_arg)};
    if (not indexed_cube_list_opt) {
        std::cerr << "EE: Unable to load indexed cube list ("
         << material_balance << ", " << second_arg << ")\n";
        return EXIT_FAILURE;
    }
    std::cerr << "II: *indexed_cube_list_opt size (bytes): "
     << indexed_cube_list_opt->nbytes()
     << "\nII: *indexed_cube_list_opt size (cubes): "
     << indexed_cube_list_opt->ncubes() << '\n';

    auto const cubes(
     load_chesspresso_F_plb_from_archive<32, 8>(material_balance));
    std::cerr << "II: orig data size (cubes) = " << cubes.ncubes()
     << "\nII: orig data size (bytes) =" << cubes.nbytes()
     << "\nII: data size (min) = "
     << cubes.size() * ((cubes.ninbits() + cubes.noutbits() + 7) / 8) << '\n';

    std::mutex io_mutex;
    bool found_problem{};
    std::size_t so_far{};
    std::size_t const totn{cubes.size()};
    std::size_t nskip{totn / 100000};
    if (nskip < 1) {
        nskip = 1;
    }
    desire_parallel_for_each(cubes.begin(), cubes.end(), [&](auto & inout) {
        auto const lookup_result{indexed_cube_list_opt->lookup(inout.inputs())};
        std::lock_guard<std::mutex> lock(io_mutex);
        if (lookup_result != inout.outputs()) {
            std::cerr << "EE: Error on input " << inout.inputs()
             << "\nII:\tshould be "
             << to_binary_string(inout.outputs(), cubes.noutbits())
             << "\nII:\tgot       "
             << to_binary_string(lookup_result, cubes.noutbits()) << '\n';
            found_problem = true;
        } else {
            ++so_far;
            if (0 == so_far % nskip) {
                std::cerr << "II: " << so_far << '/' << totn << '\n';
            }
        }
     });
    if (found_problem) {
        return EXIT_FAILURE;
    }
    std::cerr << "II: PASS.\n";
    return EXIT_SUCCESS;
}
