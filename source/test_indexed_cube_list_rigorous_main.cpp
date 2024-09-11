#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <mutex>

#include "cube_list.hpp"
#include "indexed_cube_list.hpp"
#include "load_and_save.hpp"
#include "parallel_for_each.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name
     << " <path_to_expanded_cube_list> <path_to_baseline_cube_list> [simple]\n"
     "Note that the baseline cubes currently must contain only unit cubes.\n";
    std::exit(EXIT_FAILURE);
}


int main(int argc, char ** argv) {
    if ((3 != argc) and (4 != argc)) {
        print_usage(argv[0]);
    }
    bool const simple = (argc > 3);
    std::filesystem::path const path_to_icl1(argv[1]);
    if (not std::filesystem::exists(path_to_icl1)) {
        std::cerr << "EE: The path " << path_to_icl1 << " does not exist.\n";
        return EXIT_FAILURE;
    }
    std::filesystem::path const path_to_icl2(argv[2]);
    if (not std::filesystem::exists(path_to_icl2)) {
        std::cerr << "EE: The path " << path_to_icl2 << " does not exist.\n";
        return EXIT_FAILURE;
    }

    std::optional<indexed_cube_list_type> const icl1_opt{
     indexed_cube_list_type::create_opt(path_to_icl1)};
    if (not icl1_opt) {
        std::cerr << "EE: Unable to load " << path_to_icl1 << ".\n";
        return EXIT_FAILURE;
    }
    indexed_cube_list_type const & icl1{*icl1_opt};

    std::optional<indexed_cube_list_type> const icl2_opt{
     indexed_cube_list_type::create_opt(path_to_icl2)};
    if (not icl2_opt) {
        std::cerr << "EE: Unable to load " << path_to_icl2 << ".\n";
        return EXIT_FAILURE;
    }
    indexed_cube_list_type const & icl2{*icl2_opt};

    std::mutex io_mutex;
    bool found_problem{};
    std::size_t so_far{};
    std::size_t const totn{icl2.cubes().size()};
    std::size_t nskip{totn / 100000};
    if (nskip < 1) {
        nskip = 1;
    }
    std::size_t const noutbits{icl1.cubes().noutbits()};
    desire_parallel_for_each(icl2.begin(), icl2.end(),
     [&](auto & inout) {
        bool probhere = false;
        // first lookup min term to see if output matches
        auto const res1{icl1.lookup(inout.inputs())};
        if (res1 != inout.outputs()) {
            io_mutex.lock();
            std::cerr << "EE: Error on input " << inout.inputs()
             << "\nII:\tshould be " << to_binary_string(inout.outputs(), noutbits)
             << "\nII:\tgot       " << to_binary_string(res1, noutbits) << '\n';
            found_problem = true;
            io_mutex.unlock();
            probhere = true;
        }
        if (simple) return;
        // should just be the index of the par_for_each loop
        auto const ind2{icl2.lookupindex(inout.inputs())};
        // next lookup min term to see if index is correct
        auto const ind1{icl1.lookupindex(inout.inputs())};
        if (icl1[ind1].outputs() != inout.outputs()) {
            io_mutex.lock();
            std::cerr << "EE: Error (index) on input " << inout.inputs()
             << "\nII:\tindex of " << ind1 << " produces cube of "
             << icl1[ind1].inputs() << " => "
             << to_binary_string(icl1[ind1].outputs(), noutbits)
             << "\nII:\tshould be "
             << to_binary_string(inout.outputs(), noutbits) << '\n';
            found_problem = true;
            io_mutex.unlock();
            probhere = true;
        }
        if (not icl1[ind1].inputs().overlaps(inout.inputs())) {
            io_mutex.lock();
            std::cerr << "EE: Error (index) on input " << inout.inputs()
             << "\nII:\tindex of " << ind1 << " is \"in cube\" of "
             << icl1[ind1].inputs() << "\tbut does not overlap.\n";
            found_problem = true;
            io_mutex.unlock();
            probhere = true;
        }
        // next lookup all indexes for min term
        auto const set1{icl1.lookupallindex(inout.inputs())};
        for (auto const i: set1) {
            if (icl1[i].outputs() != inout.outputs()) {
                io_mutex.lock();
                std::cerr << "EE: Error (allindex) on input " << inout.inputs()
                 << "\nII:\tindex of " << i << " produces cube of "
                 << icl1[i].inputs() << " => "
                 << to_binary_string(icl1[i].outputs(), noutbits)
                 << "\nII:\tshould be "
                 << to_binary_string(inout.outputs(), noutbits) << '\n';
                found_problem = true;
                io_mutex.unlock();
                probhere = true;
            }
            if (not icl1[i].inputs().overlaps(inout.inputs())) {
                io_mutex.lock();
                std::cerr << "EE: Error (allindex) on input " << inout.inputs()
                 << "\nII:\tindex of " << i << " is \"in cube\" of "
                 << icl1[i].inputs() << " but does not overlap.\n";
                found_problem = true;
                io_mutex.unlock();
                probhere = true;
            }
            // Next, check we get this cube back when querying from the other
            // direction.
            auto const set2{icl2.lookupallindex(icl1[i].inputs())};
            if (set2.find(ind2) == set2.end()) {
                io_mutex.lock();
                std::cerr << "EE: Error (allindex) on input " << inout.inputs()
                 << "\nII:\tindex of " << i << " is \"in cube\" of "
                 << icl1[i].inputs() << " but fails reverse lookup ("
                 << ind2 << "):\n";
                for (auto const i2: set2) {
                    std::cerr << "II:\t\t" << i2 << " "
                     << icl2[i2].inputs() << "\n";
                }
                found_problem = true;
                io_mutex.unlock();
                probhere = true;
            }
        }
        if (not probhere) {
             io_mutex.lock();
             so_far++;
             if (so_far % nskip == 0) {
                 std::cerr << so_far << '/' << totn << std::endl;
             }
             io_mutex.unlock();
        }
     });
    if (found_problem) {
        return EXIT_FAILURE;
    }
    std::cerr << "II: PASS.\n";
    return EXIT_SUCCESS;
}
