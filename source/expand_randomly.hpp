#ifndef EGT_COMPARISON_EXPAND_RANDOMLY_HPP
#define EGT_COMPARISON_EXPAND_RANDOMLY_HPP


#include <cstddef>
#include <random>
#include <type_traits>

#include "cube_list.hpp"
#include "indexed_cube_list.hpp"


template <
    std::size_t const required_input_bit_pair_count,
    std::size_t const required_output_bit_count,
    std::size_t const required_cube_list_index_bit_count,
    std::size_t const required_tree_node_index_bit_count
> void expand_randomly(
    cube_list_tt<
        required_input_bit_pair_count,
        required_output_bit_count
    > & cubes_to_expand,
    indexed_cube_list_tt<
        cube_list_tt<required_input_bit_pair_count, required_output_bit_count>,
        required_cube_list_index_bit_count,
        required_tree_node_index_bit_count
    > const & reference_set,
    std::optional<unsigned long> const seed_opt = std::nullopt,
    bool const parallelize_if_possible = true
) {
    using cube_list_type = std::remove_reference_t<decltype(cubes_to_expand)>;
    using cube_input_index_type = typename cube_list_type::inputs_index_type;

    std::mt19937_64 generator;
    if (std::nullopt != seed_opt) {
        generator.seed(*seed_opt);
    }
    cubes_to_expand.choose_if_parallelism_is_desirable_for_each(
     parallelize_if_possible, [&](auto & unexpanded_cube) {
        std::vector<cube_input_index_type> raise_attempt_order{
         unexpanded_cube.inputs().generate_raise_attempt_ordering(generator)};
        reference_set.attempt_raising(unexpanded_cube, raise_attempt_order);
    });
}


#endif // EGT_COMPARISON_EXPAND_RANDOMLY_HPP
