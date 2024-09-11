#if !defined(EGT_COMPARISON_MINIMIZATION_CYCLE_HPP)
#define EGT_COMPARISON_MINIMIZATION_CYCLE_HPP


#include <iomanip>
#include <iostream>

#include "cube_list.hpp"
#include "expand_randomly.hpp"
#include "indexed_cube_list.hpp"
#include "irreduce_algorithm.hpp"


template <
    bool const invoke_internal_debug_checks = false,
    bool const use_caching = false,
    bool const request_parallelism = true,
    std::size_t const required_input_bit_pair_count,
    std::size_t const required_output_bit_count,
    std::size_t const required_cube_list_index_bit_count,
    std::size_t const required_tree_node_index_bit_count
> void do_minimization_cycle(
    cube_list_tt<
        required_input_bit_pair_count,
        required_output_bit_count
    > & cubes_to_minimize,
    indexed_cube_list_tt<
        cube_list_tt<required_input_bit_pair_count, required_output_bit_count>,
        required_cube_list_index_bit_count,
        required_tree_node_index_bit_count
    > const * const D00I_reference_set_ptr,
    [[maybe_unused]] indexed_cube_list_tt<
        cube_list_tt<required_input_bit_pair_count, required_output_bit_count>,
        required_cube_list_index_bit_count,
        required_tree_node_index_bit_count
    > const * const D01I_reference_set_ptr,
    bool const do_expand_randomly,
    bool const do_make_irredundant,
    bool const do_reduce,
    bool const check_outputs,
    std::optional<unsigned long> const seed_opt = std::nullopt
) {
    assert(D01I_reference_set_ptr or not do_expand_randomly);
    assert(D00I_reference_set_ptr or not do_make_irredundant);
    assert(D00I_reference_set_ptr or not do_reduce);

    std::cerr << "II:      original cubes_to_minimize.size() = "
     << std::setw(11) << cubes_to_minimize.size() << '\n';
    if (do_expand_randomly) {
        expand_randomly(cubes_to_minimize, *D00I_reference_set_ptr, seed_opt,
         request_parallelism);
        std::cerr << "II:   post-expand cubes_to_minimize.size() = "
         << std::setw(11) << cubes_to_minimize.size() << '\n';
    }
    if (do_make_irredundant or do_reduce) {
        bool const do_not_emit_tracing{};
        bool const problem_found{irreduce<do_not_emit_tracing, invoke_internal_debug_checks,
         use_caching, request_parallelism>(cubes_to_minimize,
         *D00I_reference_set_ptr, do_make_irredundant, do_reduce, check_outputs)};
        if (problem_found) {
            std::cerr << "EE: A problem was encountered during minimization.\n";
            std::exit(EXIT_FAILURE);
        }
        std::cerr << "II: post-irreduce cubes_to_minimize.size() = "
         << std::setw(11) << cubes_to_minimize.size() << '\n';
    }
}


#endif // EGT_COMPARISON_MINIMIZATION_CYCLE_HPP
