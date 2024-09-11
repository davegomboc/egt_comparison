#if !defined(EGT_COMPARISON_SET_LHS_BITS_ARE_SUBSET_OF_SET_RHS_BITS_TYPE_HPP)
#define EGT_COMPARISON_SET_LHS_BITS_ARE_SUBSET_OF_SET_RHS_BITS_TYPE_HPP


#include "bit_region_uint_operator_type.hpp"


template <typename uint_type>
class set_lhs_bits_are_subset_of_set_rhs_bits_type
  : public bit_region_uint_operator_type<
    uint_type, set_lhs_bits_are_subset_of_set_rhs_bits_type<uint_type>>
{
public:
    using result_type = bool;

    auto combine_partial_results(
        result_type & result,
        result_type new_result
    ) const -> result_type {
        return result &= new_result;
    }

    auto partial_result_suffices(result_type const result) const -> bool {
        return not result;
    }

    auto apply_on_full_uint(
        std::add_const_t<uint_type> const lhs,
        std::add_const_t<uint_type> const rhs
    ) const -> result_type {
        return 0 == static_cast<uint_type>(lhs & invert(rhs));
    }

    auto apply_on_partial_uint(
        std::add_const_t<uint_type> const include_mask,
        std::add_const_t<uint_type> const lhs,
        std::add_const_t<uint_type> const rhs
    ) const -> result_type {
        return 0 == static_cast<uint_type>(include_mask & lhs & invert(rhs));
    }
};


#endif // EGT_COMPARISON_SET_LHS_BITS_ARE_SUBSET_OF_SET_RHS_BITS_TYPE_HPP
