#if !defined(EGT_COMPARISON_BIT_REGION_UINT_OPERATORS_HPP)
#define EGT_COMPARISON_BIT_REGION_UINT_OPERATORS_HPP


#include <iostream>
#include <type_traits>

#include "bit_region_type.hpp"
#include "operators/uint/set_lhs_bits_are_subset_of_set_rhs_bits_type.hpp"
#include "primitive_bit_operations.hpp"


template <
    typename uint_type,
    typename lhs_uint_type,
    typename rhs_uint_type,
    typename = std::enable_if_t<
        std::is_same_v<
            std::remove_const_t<uint_type>,
            std::remove_const_t<lhs_uint_type>
        >
    >,
    typename = std::enable_if_t<
        std::is_same_v<
            std::remove_const_t<uint_type>,
            std::remove_const_t<rhs_uint_type>
        >
    >
>
bool runs_when_set_lhs_bits_are_subset_of_set_rhs_bits(
    std::size_t const first_bit,
    std::size_t const past_last_bit,
    bit_region_type<lhs_uint_type> const & lhs_region,
    bit_region_type<rhs_uint_type> const & rhs_region
) {
    static_assert(std::is_integral_v<uint_type>,
     "Template argument for bit_region_type must be integral.");
    static_assert(not std::is_signed_v<uint_type>,
     "Template argument for bit_region_type must be unsigned.");
    static set_lhs_bits_are_subset_of_set_rhs_bits_type<uint_type const>
     predicate;
    //std::cerr << "II: [" << first_bit << ", " << past_last_bit << ") of:\n";
    //std::cerr << "II: lhs_region = ";
    //lhs_region.debug_print(std::cerr);
    //std::cerr << '\n';
    //std::cerr << "II: rhs_region = ";
    //rhs_region.probe_debug_print(std::cerr);
    //std::cerr << '\n';
    bool const result{predicate.apply_on_bit_run(first_bit, past_last_bit,
     lhs_region, rhs_region)};
    //std::cerr << "II: result = " << result << '\n';
    return result;
}

template <
    typename uint_type,
    typename lhs_uint_type,
    typename rhs_uint_type,
    typename = std::enable_if_t<
        std::is_same_v<
            std::remove_const_t<uint_type>,
            std::remove_const_t<lhs_uint_type>
        >
    >,
    typename = std::enable_if_t<
        std::is_same_v<
            std::remove_const_t<uint_type>,
            std::remove_const_t<rhs_uint_type>
        >
    >
>
bool set_lhs_bits_are_subset_of_set_rhs_bits(
    bit_region_type<lhs_uint_type> const & lhs_region,
    bit_region_type<rhs_uint_type> const & rhs_region
) {
    std::size_t const bit_count{lhs_region.bit_count()};
    assert(bit_count == rhs_region.bit_count());
    return runs_when_set_lhs_bits_are_subset_of_set_rhs_bits<uint_type>(0,
     bit_count, lhs_region, rhs_region);
}


#endif // EGT_COMPARISON_BIT_REGION_UINT_OPERATORS_HPP
