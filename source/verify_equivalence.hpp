#ifndef EGT_COMPARISON_VERIFY_EQUIVALENCE_HPP
#define EGT_COMPARISON_VERIFY_EQUIVALENCE_HPP


#include <cstddef>
#include <iomanip>
#include <iostream>
#include <mutex>

#include "cube_list.hpp"
#include "indexed_cube_list.hpp"
#include "parallel_for_each.hpp"


// Verify that probing simplified returns the same answer as probing baseline
// (reference set).
//
// Returns true iff an error occurred.
template <
    typename simplified_cubes_type,
    typename baseline_cubes_type,
    bool const desire_parallelism = true
> bool verify_equivalence(
    simplified_cubes_type const & simplified_cubes,
    baseline_cubes_type const & baseline_cubes
) {
    auto for_each_baseline_cube = [&baseline_cubes](auto unary_fn) {
        optionally_parallel_for_each(
         desire_parallelism,
         baseline_cubes.begin(),
         baseline_cubes.end(),
         [&](auto & baseline_cube) {
            std::size_t const baseline_cube_index{static_cast<std::size_t>(
             &baseline_cube - baseline_cubes.data())};
            unary_fn(baseline_cube_index);
         });
    };

    using cube_list_type = cube_list_tt<32, 8>;
    using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;

    std::mutex io_mutex;
    bool problem_found{};
    indexed_cube_list_type indexed_simplified_cubes(simplified_cubes);
    for_each_baseline_cube([&](std::size_t const baseline_cube_index) {
        auto const & baseline_cube{baseline_cubes[baseline_cube_index]};
        auto const corresponding_simplified_outputs{
         indexed_simplified_cubes.lookupall(baseline_cube.inputs(), 2)};
        if (1 == corresponding_simplified_outputs.size()) {
            cube_list_type::outputs_value_type const resolved_outputs{
             *corresponding_simplified_outputs.begin()};
            if (baseline_cube.outputs() == resolved_outputs) {
                // Probing returns the correct outputs for every unit cube
                // within these binary inputs that we actually care about, so we
                // intentionally do nothing.
            } else {
                problem_found = true;
                std::lock_guard<std::mutex> const lock(io_mutex);
                std::cerr << "EE: Erroneous simplification detected.\n"
                 "II: Baseline cube = " << baseline_cube << "\n"
                 "II: Resolved outputs = " << resolved_outputs << "\n";
            }
        } else {
            problem_found = true;
            std::lock_guard<std::mutex> const lock(io_mutex);
            std::cerr << "EE: Erroneous simplification detected.\n"
             "II: Baseline cube = " << baseline_cube << "\n";
            if (corresponding_simplified_outputs.empty()) {
                std::cerr << "II: No corresponding simplified outputs exist.\n";
            } else {
                std::cerr << "II: At least two distinct corresponding "
                 "simplified outputs exist:";
                for (auto const & output: corresponding_simplified_outputs) {
                    std::cerr << ' ' << static_cast<unsigned long long>(output);
                }
                std::cerr << ".\n";
                auto const corresponding_simplified_output_indices{
                 indexed_simplified_cubes.lookupallindex(baseline_cube.inputs())};
                std::cerr << "II: All such indices and their cubes follow.\n";
                for (std::size_t const index: corresponding_simplified_output_indices) {
                    std::cerr << "II: index = " << std::setw(11) << index
                     << "  cube = " << indexed_simplified_cubes[index] << '\n';
                }
            }
        }
    });

    return problem_found;
}


#endif // EGT_COMPARISON_VERIFY_EQUIVALENCE_HPP
