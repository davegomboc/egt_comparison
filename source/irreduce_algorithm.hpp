#if !defined(EGT_COMPARISON_IRREDUCE_ALGORITHM_HPP)
#define EGT_COMPARISON_IRREDUCE_ALGORITHM_HPP


#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <set>
#include <type_traits>
#include <vector>

#include "atomic_stuff.hpp"
#include "cube_list.hpp"
#include "indexed_cube_list.hpp"
#include "integer_priority_queue.hpp"
#include "parallel_for_each.hpp"
#include "synchronized_shuffle.hpp"
#include "verify_equivalence.hpp"


// Returns true iff an error is detected, which can only happen if check_outputs
// is true.
//
// Caching speeds things up by about a factor of two, but uses much more memory.
//
// N.B.  This implementation requires that the reference_set must contain only
// minterms (unit cubes).
template <
    bool const emit_tracing = false,
    bool const debug_irreduce = false,
    bool const use_caching = false,
    bool const use_parallelism_if_possible = true,
    std::size_t const required_input_bit_pair_count,
    std::size_t const required_output_bit_count,
    std::size_t const required_cube_list_index_bit_count,
    std::size_t const required_tree_node_index_bit_count
> bool irreduce(
    cube_list_tt<
        required_input_bit_pair_count,
        required_output_bit_count
    > & expanded_cubes,
    indexed_cube_list_tt<
        cube_list_tt<required_input_bit_pair_count, required_output_bit_count>,
        required_cube_list_index_bit_count,
        required_tree_node_index_bit_count
    > const & reference_set,
    bool const do_make_irredundant,
    bool const do_reduce,
    bool const check_outputs,
    bool const do_prereduce_shuffle = true,
    std::optional<unsigned long> const seed_opt = std::nullopt
) {
    using indexed_cube_list_type = indexed_cube_list_tt<
        cube_list_tt<
            required_input_bit_pair_count,
            required_output_bit_count
        > &,
        required_cube_list_index_bit_count,
        required_tree_node_index_bit_count
    >;

    // Even though this is only used when making irredundant, not when
    // reducing, if we do want to run make irredundant then we must
    // store the current cubes.
    std::optional<indexed_cube_list_type> icl_opt;
    if (do_make_irredundant) {
        icl_opt.emplace(expanded_cubes);
    }

    auto const for_each_expanded_cube_index = [&expanded_cubes](auto unary_fn) {
        expanded_cubes.choose_if_parallelism_is_desirable_for_each(
         use_parallelism_if_possible,
         [&](auto & expanded_cube) {
            std::size_t const cube_index{
             static_cast<std::size_t>(&expanded_cube - expanded_cubes.data())};
            unary_fn(cube_index);
         });
    };

    std::vector<std::set<std::size_t>> indices_of_covered_baseline_cubes_cache;
    if constexpr (use_caching) {
        indices_of_covered_baseline_cubes_cache.resize(expanded_cubes.size());
        for_each_expanded_cube_index([&](std::size_t const ec_index) {
            indices_of_covered_baseline_cubes_cache[ec_index]
             = reference_set.lookupallindex(expanded_cubes[ec_index].inputs());
        });
    }

    decltype(auto) baseline_cube_indices_covered_by
     = [&](std::size_t const ec_index) {
        if (use_caching) {
            return indices_of_covered_baseline_cubes_cache[ec_index];
        }
        return reference_set.lookupallindex(expanded_cubes[ec_index].inputs());
    };

    std::vector<std::size_t> baseline_cubes_covered_by_counts;
    auto const compute_baseline_cubes_covered_by_counts = [&]() {
        if constexpr (use_parallelism_if_possible) {
            std::vector<std::atomic_size_t> updatable_in_parallel(
             reference_set.size());
            initialize_if_necessary<std::size_t>(updatable_in_parallel, 0);
            for_each_expanded_cube_index([&](std::size_t const ec_index) {
                for (auto const bc_index:
                 baseline_cube_indices_covered_by(ec_index)) {
                    ++updatable_in_parallel[bc_index];
                }
            });
            baseline_cubes_covered_by_counts.assign(updatable_in_parallel.cbegin(),
             updatable_in_parallel.cend());
        } else {
            baseline_cubes_covered_by_counts.assign(reference_set.size(), 0);
            for_each_expanded_cube_index([&](std::size_t const ec_index) {
                for (auto const bc_index:
                 baseline_cube_indices_covered_by(ec_index)) {
                    ++baseline_cubes_covered_by_counts[bc_index];
                }
            });
        }
    };

    compute_baseline_cubes_covered_by_counts();

    // IRREDUNDANT ALGORITHM:
    if (do_make_irredundant) {

        // N.B. std::vector<bool> can't be atomically updated in parallel.
        using marking_element_type
         = std::conditional_t<use_parallelism_if_possible, char, bool>;
        std::vector<marking_element_type> to_use(expanded_cubes.size(), false);
        std::vector<marking_element_type> covered(reference_set.size(), false);
        std::vector<long> ncovers(expanded_cubes.size(), 0);
        std::mutex io_mutex;

        integer_priority_queue_type<long> cube_queue;

        auto mark_to_use = [&](
            std::size_t const ec_index,
            std::set<std::size_t> const & baseline_cube_indices_covered_by_ec_index,
            bool const update_priority
        ) {
            if constexpr (emit_tracing) {
                std::lock_guard<std::mutex> const lock(io_mutex);
                std::cerr << "DD: Using cube " << std::setw(11) << ec_index
                 << ", which covers ";
                if (update_priority) {
                    std::cerr << std::setw(11) << ncovers[ec_index]
                     << " cube" << (1 == ncovers[ec_index] ? ".\n" : "s.\n");
                } else {
                    std::cerr << "an undetermined number of cubes.\n";
                }
            }
            to_use[ec_index] = true;
            std::set<std::size_t> changed;
            for (auto const & bc_index: baseline_cube_indices_covered_by_ec_index) {
                if (not covered[bc_index]) {
                    covered[bc_index] = true;
                    if (update_priority) {
                        for (auto const & ec2: icl_opt->lookupallindex(
                         reference_set[bc_index].inputs())) {
                            if (not to_use[ec2]) {
                                if constexpr (debug_irreduce) {
                                    bool found{};
                                    for (auto const & bc2: baseline_cube_indices_covered_by(ec2)) {
                                        if (bc2 == bc_index) {
                                            found = true;
                                            break;
                                        }
                                    }
                                    if (not found) {
                                        std::cerr << "EE: Maps don't agree.\n"
                                         "II: ec_index = " << ec_index << ": " << expanded_cubes[ec_index]
                                         << "\nII:  said to cover " << bc_index << ": " << reference_set[bc_index]
                                         << "\nII:    which is said to be contained in " << ec2 << ": " << expanded_cubes[ec2]
                                         << "\nII:    which is said to be contained in " << ec2 << ": " << (*icl_opt)[ec2]
                                         << "\nII:      which has the following minterms:\n";
                                        for (auto const & bc3: baseline_cube_indices_covered_by(ec2)) {
                                            std::cerr << "II:        " << bc3 << ": " << reference_set[bc3] << '\n';
                                        }
                                        std::exit(EXIT_FAILURE);
                                    }
                                }
                                changed.insert(ec2);
                                --ncovers[ec2];
                            }
                        }
                    }
                }
            }
            if (update_priority) {
                for (auto & ec3: changed) {
                    if (0 == ncovers[ec3]) {
                        cube_queue.remove(ec3);
                    } else {
                        cube_queue.newpriority(ec3, ncovers[ec3]);
                    }
                }
            }
        };

        if constexpr (emit_tracing) {
            std::cerr << "DD: Essential cubes follow.\n";
        }
        // Mark all essential cubes.
        for_each_expanded_cube_index([&](std::size_t const ec_index) {
            bool is_essential{};
            auto const & baseline_cube_indices_covered_by_ec_index
             = baseline_cube_indices_covered_by(ec_index);
            for (auto const bc_index: baseline_cube_indices_covered_by_ec_index) {
                if (1 == baseline_cubes_covered_by_counts[bc_index]) {
                    is_essential = true;
                    break;
                }
            }
            if (is_essential) {
                mark_to_use(ec_index, baseline_cube_indices_covered_by_ec_index, false);
            } else {
                long nc{};
                for (auto const bc_index: baseline_cube_indices_covered_by_ec_index) {
                    if (not covered[bc_index]) {
                        ++nc;
                    }
                }
                ncovers[ec_index] = nc;
            }
        });

        // We cannot use parallel_for_each here, because the integer
        // priority queue implementation is not parallel-update-safe.
        for (std::size_t ec_index{}; ec_index != expanded_cubes.size();
         ++ec_index) {
            if (not to_use[ec_index] and 0 < ncovers[ec_index]) {
                cube_queue.insert(ec_index, ncovers[ec_index]);
            }
        }

        if constexpr (emit_tracing) {
            std::cerr << "DD: Any greedily chosen cubes follow.\n";
        }
        // Repeatedly greedily mark an expanded cube that covers the
        // most uncovered baseline cubes, until all baseline cubes
        // have been covered.
        while (not cube_queue.empty()) {
            auto const ec_index{cube_queue.top()};
            cube_queue.pop();
            mark_to_use(ec_index, baseline_cube_indices_covered_by(ec_index), true);
        }

        // Shift all marked expanded_cubes to its beginning.
        std::size_t ec_write_index{};
        for (std::size_t ec_read_index{};
         ec_read_index != expanded_cubes.size(); ++ec_read_index) {
            if (to_use[ec_read_index]) {
                if (ec_write_index != ec_read_index) {
                    expanded_cubes[ec_write_index]
                     = std::move(expanded_cubes[ec_read_index]);
                    if constexpr (use_caching) {
                        if (do_reduce) {
                            indices_of_covered_baseline_cubes_cache[
                             ec_write_index] = std::move(
                             indices_of_covered_baseline_cubes_cache[
                             ec_read_index]);
                        }
                    }
                }
                ++ec_write_index;
            }
        }
        // Shrink expanded_cubes (and the cache, if used) to the proper size.
        expanded_cubes.resize(ec_write_index);
        if constexpr (use_caching) {
            if (do_reduce) {
                indices_of_covered_baseline_cubes_cache.resize(ec_write_index);
            }
        }
    }

    // REDUCE ALGORITHM:
    if (do_reduce) {
        // Shuffling expanded_cubes will mess up *icl_opt if it exists, but
        // fortunately, we actually no longer need it.
        icl_opt = std::nullopt;

        if (do_prereduce_shuffle) {
            std::mt19937_64 generator;
            if (std::nullopt != seed_opt) {
                generator.seed(*seed_opt);
            }
            if constexpr (use_caching) {
                synchronized_shuffle(generator, static_cast<std::ptrdiff_t>(
                 expanded_cubes.size()), expanded_cubes.begin(),
                 indices_of_covered_baseline_cubes_cache.begin());
            } else {
                synchronized_shuffle(generator, static_cast<std::ptrdiff_t>(
                 expanded_cubes.size()), expanded_cubes.begin());
            }
        }

        if (do_make_irredundant) {
            // Needs to be rebuilt.
            compute_baseline_cubes_covered_by_counts();
        }

        std::vector<bool> covered_by_a_reduced_cube(reference_set.size(),
         false);
        std::size_t ec_write_index{};
        for (std::size_t ec_read_index{};
         ec_read_index != expanded_cubes.size(); ++ec_read_index) {
            if (0 == ec_read_index % 10000) {
                std::cerr << "II: " << ec_read_index << "/"
                 << expanded_cubes.size() << '\n';
            }
            std::vector<std::size_t> must_cover_now;
            for (auto const bc_index:
             baseline_cube_indices_covered_by(ec_read_index)) {
                assert(0 < baseline_cubes_covered_by_counts[bc_index]);
                --baseline_cubes_covered_by_counts[bc_index];
                if (0 == baseline_cubes_covered_by_counts[bc_index]
                 and not covered_by_a_reduced_cube[bc_index]) {
                    // Help me, Obi-Wan Kenobi.  You're my only hope.
                    must_cover_now.push_back(bc_index);
                }
            }
            if (not must_cover_now.empty()) {
                // Build the reduced cube that covers what must be covered now.
                auto reduced_cube{expanded_cubes[ec_read_index]};
                reduced_cube.inputs()
                 = reference_set[must_cover_now[0]].inputs();
                for (std::size_t k{1}; k < must_cover_now.size(); ++k) {
                    reduced_cube.inputs().expand_in_place(
                     reference_set[must_cover_now[k]].inputs());
                }
                if constexpr (debug_irreduce) {
                    bool fatal_error{};
                    if (reduced_cube.outputs()
                     != expanded_cubes[ec_read_index].outputs()) {
                        std::cerr << "EE: Cubes' outputs don't match.\n";
                        fatal_error = true;
                    }
                    if (not reduced_cube.inputs().contained_by(
                     expanded_cubes[ec_read_index].inputs())) {
                        std::cerr << "EE: Cube input was not reduced.\nII:\t"
                         "orig = " << expanded_cubes[ec_read_index].inputs()
                         << '\n';
                        for (auto const k: must_cover_now) {
                            std::cerr << "II:\tmin  = "
                             << reference_set[k].inputs() << '\n';
                        }
                        std::cerr << "II:\tnew  = " << reduced_cube.inputs()
                         << '\n';
                         fatal_error = true;
                    }
                    if (fatal_error) {
                        std::exit(EXIT_FAILURE);
                    }
                }
                // Mark as covered all of the baseline cubes that the
                // reduced cube actually does cover, which may include
                // one or more baseline cubes that were not required
                // to be covered immediately.
                for (auto const bc_index:
                 reference_set.lookupallindex(reduced_cube.inputs())) {
                    covered_by_a_reduced_cube[bc_index] = true;
                }
                expanded_cubes[ec_write_index] = reduced_cube;
                ++ec_write_index;
            }
        }
        if constexpr (debug_irreduce) {
            bool fatal_error{};
            for (auto const bc_index: covered_by_a_reduced_cube) {
                if (not bc_index) {
                    std::cerr << "EE: Baseline cube " << bc_index
                     << " was not marked as covered by a reduced cube.\n";
                    fatal_error = true;
                }
            }
            if (fatal_error) {
                std::exit(EXIT_FAILURE);
            }
        }
        expanded_cubes.resize(ec_write_index);
    }

    if (check_outputs) {
        if constexpr (emit_tracing) {
            std::cerr
             << "II: Checking that outputs were resolved as expected.\n";
        }
        bool const problem_found{verify_equivalence(expanded_cubes,
         reference_set)};
        if (problem_found) {
            return true;
        }
    }

    return false;
}


#endif // EGT_COMPARISON_IRREDUCE_ALGORITHM_HPP
