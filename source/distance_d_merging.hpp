#ifndef EGT_COMPARISON_DISTANCE_D_MERGING_HPP
#define EGT_COMPARISON_DISTANCE_D_MERGING_HPP


#include <cassert>
#include <cstddef>
#include <iomanip>
#include <iostream>

#include "cube_list.hpp"
#include "uint_type_selection.hpp"


// Always provide a reference set when the distance being merged exceeds one.
struct no_reference_set {};


/* Performs a single pass of merging of cubes that disagree only in the d
 * variables indicated by mask.
 *
 * First, we sort the cubes so that candidates for merging will be grouped
 * together.  Then, we scan through the cubes, merging each group of them into a
 * single larger cube when possible.
 *
 * Once some merging happens, we shift all later cubes up as they are scanned
 * through, overwriting cubes that have been merged together.  Marking the cubes
 * as all-zeroes (removed), then either shifting them up and the end of the pass
 * or waiting until a following sort groups them together were also tried: both
 * were found to be slower.
 *
 * Returns how many cubes were eliminated during the pass.
 */
template <
    std::size_t const MAXINBITS,
    std::size_t const MAXOUTBITS,
    typename T
> std::size_t do_single_merge_pass(
    cube_list_tt<MAXINBITS, MAXOUTBITS> & mergeable_cubes,
    typename cube_list_tt<MAXINBITS, MAXOUTBITS>::inputs_value_type const mask,
    T const & reference_set
) {
    using cube_list_type = cube_list_tt<MAXINBITS, MAXOUTBITS>;
    using cube_binary_inputs_type
     = typename cube_list_type::cube_type::inputs_type;
    using inputs_value_type = typename cube_list_type::inputs_value_type;
    using cube_iter_type
     = typename std::vector<typename cube_list_type::cube_type>::iterator;
    mergeable_cubes.sort(mask);
    inputs_value_type const set_mask = (~mask) & mergeable_cubes.lookup_mask();
    cube_iter_type mergeable_write_iter(mergeable_cubes.begin());
    cube_iter_type first_in_mergeable_zone_iter(mergeable_cubes.begin());
    cube_iter_type past_last_in_mergeable_zone_iter;
    while (mergeable_cubes.end() != first_in_mergeable_zone_iter) {
        bool outputs_in_mergeable_zone_are_consistent{true};
        for (past_last_in_mergeable_zone_iter
         = std::next(first_in_mergeable_zone_iter);
         (mergeable_cubes.end() != past_last_in_mergeable_zone_iter)
         and ((mask & static_cast<inputs_value_type>(
         past_last_in_mergeable_zone_iter->inputs()))
         == (mask & static_cast<inputs_value_type>(
         first_in_mergeable_zone_iter->inputs())));
         ++past_last_in_mergeable_zone_iter) {
            if (outputs_in_mergeable_zone_are_consistent
             and (first_in_mergeable_zone_iter->outputs()
             != past_last_in_mergeable_zone_iter->outputs())) {
                outputs_in_mergeable_zone_are_consistent = false;
            }
        }
        bool attempt_merging{outputs_in_mergeable_zone_are_consistent
         and (1 < std::distance(first_in_mergeable_zone_iter,
         past_last_in_mergeable_zone_iter))};
        if (attempt_merging) {
            cube_binary_inputs_type tempc(
             first_in_mergeable_zone_iter->inputs());
            tempc = static_cast<inputs_value_type>(tempc) | set_mask;
            if constexpr (not std::is_same_v<T, no_reference_set>) {
                bool const reference_set_has_inconsistent_outputs(
                 1 != reference_set.lookupall(tempc, 2).size());
                if (reference_set_has_inconsistent_outputs) {
                    // For now, we'll simply retain all of the existing
                    // mergeable cubes in the mergeable zone.  TODO: Consider
                    // minimizing the cube count within the mergeable zone
                    // subject to the constraints imposed by the cubes within
                    // the blocking (reference set) zone.
                    attempt_merging = false;
                }
            }
        }
        if (attempt_merging) {
            // Replace all of the cubes within the mergeable zone with a single
            // cube that encompasses the entire zone.
            first_in_mergeable_zone_iter->inputs()
             = static_cast<inputs_value_type>(
             first_in_mergeable_zone_iter->inputs()) | set_mask;
            if (first_in_mergeable_zone_iter != mergeable_write_iter) {
                *mergeable_write_iter = *first_in_mergeable_zone_iter;
            }
            ++mergeable_write_iter;
            first_in_mergeable_zone_iter = past_last_in_mergeable_zone_iter;
        } else {
            if (mergeable_write_iter == first_in_mergeable_zone_iter) {
                // The existing cubes do not need to be repositioned.
                first_in_mergeable_zone_iter = past_last_in_mergeable_zone_iter;
                mergeable_write_iter = past_last_in_mergeable_zone_iter;
            } else {
                // We need to write the existing cubes into their new locations.
                for (; first_in_mergeable_zone_iter
                 != past_last_in_mergeable_zone_iter; ) {
                    *mergeable_write_iter++ = *first_in_mergeable_zone_iter++;
                }
            }
        }
    }

    std::ptrdiff_t const eliminated_cube_count{
     std::distance(mergeable_write_iter, mergeable_cubes.end())};
    assert(0 <= eliminated_cube_count);
    std::size_t const result{static_cast<std::size_t>(eliminated_cube_count)};
    if (result) {
        // Erase any unused mergeable cube spots.
        mergeable_cubes.resize(mergeable_cubes.size() - result);
    }
    return result;
}


/* Performs a merging pass for each possible combination of up to d dimensions.
 *
 * The provided d must be a positive integer.
 *
 * Unexpectedly, D00I _must_ be used for the reference set, not even
 * D01I.  (This is different from the other implementation.)
 *
 * Returns how many cubes were eliminated during the sweep.
 */
template <
    std::size_t const MAXINBITS,
    std::size_t const MAXOUTBITS,
    typename T
> std::size_t do_single_merge_sweep(
    cube_list_tt<MAXINBITS, MAXOUTBITS> & mergeable_cubes,
    std::size_t const d,
    T const & reference_set,
    bool const show_progress,
    bool const emit_tracing
) {
    std::size_t result{};
    assert(0 < d);
    auto const past_last_bit_pair_mask{
     past_last_permutation<selected_uint_tt<MAXINBITS>>(d)};
    for (auto bit_pair_mask{
     first_permutation<selected_uint_tt<MAXINBITS>>(d)};
     bit_pair_mask != past_last_bit_pair_mask; bit_pair_mask
     = next_permutation(bit_pair_mask, mergeable_cubes.ninbits())) {
        typename cube_list_tt<MAXINBITS, MAXOUTBITS>::inputs_value_type bit_mask{
         interleave(bit_pair_mask, bit_pair_mask)};
        std::size_t const cubes_eliminated_this_pass{do_single_merge_pass(
         mergeable_cubes, ~bit_mask, reference_set)};
        result += cubes_eliminated_this_pass;
        bool const print_update{emit_tracing
         or (show_progress and 0 != cubes_eliminated_this_pass)};
        if (print_update) {
            std::cerr << "II: "
             << to_binary_string(bit_pair_mask, mergeable_cubes.ninbits());
            if (0 != cubes_eliminated_this_pass) {
                std::cerr.put(' ');
                std::cerr << std::setw(11) << mergeable_cubes.size();
                std::cerr.put('\n');
            } else {
                if (emit_tracing) {
                    std::cerr.put('\r');
                }
            }
        }
    }
    return result;
}

/* Computes the distance-d merge of the provided cube list.
 *
 * The provided d must be a positive integer.
 *
 * Unexpectedly, D00I _must_ be used for the reference set, not even
 * D01I.  (This is different from the other implementation.)
 *
 * Returns how many cubes were eliminated.
 */
template <
    std::size_t const MAXINBITS,
    std::size_t const MAXOUTBITS,
    typename T
> std::size_t distance_d_merge(
    cube_list_tt<MAXINBITS, MAXOUTBITS> & mergeable_cubes,
    std::size_t const d,
    T const & reference_set,
    int const maximum_iteration_count = 0,
    bool const show_progress = true,
    bool const emit_tracing = true
) {
    std::size_t result{};
    assert(0 < d);
    int i{};
    while ((0 == maximum_iteration_count) or (i < maximum_iteration_count)) {
        std::size_t const cubes_eliminated_this_sweep{do_single_merge_sweep(
         mergeable_cubes, d, reference_set, show_progress, emit_tracing)};
        if (0 == cubes_eliminated_this_sweep) {
            // No merges were performed during the just-completed sweep, so
            // no further progress would be made by trying again.
            break;
        }
        result += cubes_eliminated_this_sweep;
        ++i;
    }

    if (show_progress or emit_tracing) {
        // Clean up any leftover output on the current console line.  (See
        // "Erase in Line" at https://en.wikipedia.org/wiki/ANSI_escape_code.)
        std::cerr << "\x1b[2K";
    }

    return result;
}


#endif // EGT_COMPARISON_DISTANCE_D_MERGING_HPP
