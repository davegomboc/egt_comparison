#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <string_view>

#include "load_and_save.hpp"
#include "parallel_for_each.hpp"


using ddag_type = ddag<32, 8, 27>;


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name << " <balance>" << std::endl;
    std::exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
    if (2 != argc) {
        print_usage(argv[0]);
    }
    std::string_view const material_balance(argv[1]);
    std::optional<ddag_type> const ddag_opt{
     load_ddag_opt<32, 8, 27>(material_balance)};
    if (not ddag_opt) {
        std::cerr << "EE: Unable to load ddag (" << material_balance << ")\n";
        return EXIT_FAILURE;
    }
    std::cerr << "II: *ddag_opt size (bytes) = " << std::setw(11)
     << ddag_opt->nbytes() << "\nII: *ddag_opt size (nodes) = "
     << std::setw(11) << ddag_opt->nnodes() << '\n';

    auto const cubes(
     load_chesspresso_F_plb_from_archive<32, 8>(material_balance));
    std::cerr << "II: orig data size (cubes) = " << cubes.ncubes()
     << "\nII: orig data size (bytes) =" << cubes.nbytes()
     << "\nII: data size (min) = "
     << cubes.size() * ((cubes.ninbits() + cubes.noutbits() + 7) / 8) << '\n';

    std::mutex io_mutex;
    bool found_problem{};
    std::size_t so_far{};
    assert(std::begin(cubes) <= std::end(cubes));
    std::size_t const totn{static_cast<std::size_t>(
     std::distance(cubes.begin(), cubes.end()))};
    std::size_t nskip{totn / 100000};
    if (nskip < 1) {
        nskip = 1;
    }
    desire_parallel_for_each(cubes.begin(), cubes.end(), [&](auto & inout) {
        auto const lookup_result{ddag_opt->lookup(inout.inputs())};
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
