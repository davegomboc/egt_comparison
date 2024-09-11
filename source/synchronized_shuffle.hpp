#ifndef EGT_COMPARISON_SYNCHRONIZED_SHUFFLE_HPP
#define EGT_COMPARISON_SYNCHRONIZED_SHUFFLE_HPP


#include <cstddef>
#include <iterator>
#include <random>


template <typename iter_type>
void swap_contents_at_indices(
    iter_type first,
    std::ptrdiff_t const left_index,
    std::ptrdiff_t const right_index
) {
    iter_type const lhs_iter{std::next(first, left_index)};
    iter_type const rhs_iter{std::next(first, right_index)};
    iter_swap(lhs_iter, rhs_iter);
}


template <typename urbg_type, typename ...iter_types>
void synchronized_shuffle(
    urbg_type && generator,
    std::ptrdiff_t const shuffle_distance,
    iter_types ...firsts
) {
    using distr_type = std::uniform_int_distribution<std::ptrdiff_t>;
    distr_type uniform_d;
    for (std::ptrdiff_t i{shuffle_distance - 1}; i != 0; --i) {
        std::ptrdiff_t const j{uniform_d(generator,
         distr_type::param_type(0, i))};
        ( swap_contents_at_indices(firsts, i, j), ... );
    }
}


#endif // EGT_COMPARISON_SYNCHRONIZED_SHUFFLE_HPP
