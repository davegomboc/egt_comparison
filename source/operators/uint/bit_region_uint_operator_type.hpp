#if !defined(EGT_COMPARISON_BIT_REGION_UINT_OPERATOR_TYPE)
#define EGT_COMPARISON_BIT_REGION_UINT_OPERATOR_TYPE


#include <type_traits>

#include "bit_region_type.hpp"


template <typename uint_type, typename operator_type>
class bit_region_uint_operator_type {
    static_assert(std::is_integral_v<uint_type>,
     "Template argument must not be integral.");
    static_assert(std::is_unsigned_v<uint_type>,
     "Template argument must not be unsigned.");
public:
    auto casted_this() const -> operator_type const * {
        return static_cast<operator_type const *>(this);
    }
    auto casted_this() -> operator_type * {
        return static_cast<operator_type *>(this);
    }

    auto apply_on_bit_run(
        std::size_t const first_bit,
        std::size_t const past_last_bit,
        bit_region_type<uint_type> const & region
    ) const {
        assert(first_bit <= past_last_bit);
        assert(past_last_bit <= region.bit_count());
        std::size_t const first_uint{which<uint_type>(first_bit)};
        std::size_t const past_last_uint{which<uint_type>(past_last_bit)};
        if (first_uint == past_last_uint) {
            uint_type const include_mask_for_single_uint{
             single_uint_mask<uint_type>(first_bit, past_last_bit)};
            // TODO: region.uint_ref() unsafe due to nullptr as the_data_ptr
            return casted_this()->apply_on_partial_uint(
             include_mask_for_single_uint, region.uint_ref(first_uint));
        }
        uint_type const include_mask_for_first_uint{
         first_uint_mask<uint_type>(first_bit)};
        typename operator_type::result_type result{
         casted_this()->apply_on_partial_uint(include_mask_for_first_uint,
         region.uint_ref(first_uint))};
        if (casted_this()->partial_result_suffices(result)) {
            return result;
        }
        uint_type const include_mask_for_past_last_uint{
         past_last_uint_mask<uint_type>(past_last_bit)};
        // We test so that we avoid accessing any memory past the last bit:
        // dereferencing that memory may be undefined behaviour.
        if (0 != include_mask_for_past_last_uint) {
            casted_this()->combine_partial_results(result,
             casted_this()->apply_on_partial_uint(
             include_mask_for_past_last_uint, region.uint_ref(past_last_uint)));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        for (std::size_t current_uint{past_last_uint - 1};
         current_uint != first_uint; --current_uint) {
            casted_this()->combine_partial_results(result,
             casted_this()->apply_on_full_uint(region.uint_ref(current_uint)));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        return result;
    }

    auto apply_on_bit_run(
        std::size_t const first_bit,
        std::size_t const past_last_bit,
        bit_region_type<uint_type> & region,
        uint_type const value
    ) const {
        assert(first_bit <= past_last_bit);
        assert(past_last_bit <= region.bit_count());
        std::size_t const first_uint{which<uint_type>(first_bit)};
        std::size_t const past_last_uint{which<uint_type>(past_last_bit)};
        if (first_uint == past_last_uint) {
            uint_type const include_mask_for_single_uint{
             single_uint_mask<uint_type>(first_bit, past_last_bit)};
            // TODO: region.uint_ref() unsafe due to nullptr as the_data_ptr
            return casted_this()->apply_on_partial_uint(
             include_mask_for_single_uint, region.uint_ref(first_uint), value);
        }
        uint_type const include_mask_for_first_uint{
         first_uint_mask<uint_type>(first_bit)};
        typename operator_type::result_type result{
         casted_this()->apply_on_partial_uint(include_mask_for_first_uint,
         region.uint_ref(first_uint), value)};
        if (casted_this()->partial_result_suffices(result)) {
            return result;
        }
        uint_type const include_mask_for_past_last_uint{
         past_last_uint_mask<uint_type>(past_last_bit)};
        // We test so that we avoid accessing any memory past the last bit:
        // dereferencing that memory may be undefined behaviour.
        if (0 != include_mask_for_past_last_uint) {
            casted_this()->combine_partial_results(result,
             casted_this()->apply_on_partial_uint(
             include_mask_for_past_last_uint,
             region.uint_ref(past_last_uint), value));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        for (std::size_t current_uint{past_last_uint - 1};
         current_uint != first_uint; --current_uint) {
            casted_this()->combine_partial_results(result, casted_this()
             ->apply_on_full_uint(region.uint_ref(current_uint), value));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        return result;
    }

    template <
        typename region_uint_type,
        typename = std::enable_if_t<
            std::is_same_v<
                std::remove_const_t<uint_type>,
                std::remove_const_t<region_uint_type>
            >
        >
    >
    auto apply_on_bit_run(
        std::size_t const first_bit,
        std::size_t const past_last_bit,
        bit_region_type<region_uint_type> const & region,
        uint_type const value
    ) const {
        assert(first_bit <= past_last_bit);
        assert(past_last_bit <= region.bit_count());
        std::size_t const first_uint{which<uint_type>(first_bit)};
        std::size_t const past_last_uint{which<uint_type>(past_last_bit)};
        if (first_uint == past_last_uint) {
            uint_type const include_mask_for_single_uint{
             single_uint_mask<uint_type>(first_bit, past_last_bit)};
            // TODO: region.uint_ref() unsafe due to nullptr as the_data_ptr
            return casted_this()->apply_on_partial_uint(
             include_mask_for_single_uint, region.uint_ref(first_uint), value);
        }
        uint_type const include_mask_for_first_uint{
         first_uint_mask<uint_type>(first_bit)};
        typename operator_type::result_type result{
         casted_this()->apply_on_partial_uint(include_mask_for_first_uint,
         region.uint_ref(first_uint), value)};
        if (casted_this()->partial_result_suffices(result)) {
            return result;
        }
        uint_type const include_mask_for_past_last_uint{
         past_last_uint_mask<uint_type>(past_last_bit)};
        // We test so that we avoid accessing any memory past the last bit:
        // dereferencing that memory may be undefined behaviour.
        if (0 != include_mask_for_past_last_uint) {
            casted_this()->combine_partial_results(result,
             casted_this()->apply_on_partial_uint(
             include_mask_for_past_last_uint,
             region.uint_ref(past_last_uint), value));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        for (std::size_t current_uint{past_last_uint - 1};
         current_uint != first_uint; --current_uint) {
            casted_this()->combine_partial_results(result, casted_this()
             ->apply_on_full_uint(region.uint_ref(current_uint), value));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        return result;
    }

    template <
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
    auto apply_on_bit_run(
        std::size_t const first_bit,
        std::size_t const past_last_bit,
        bit_region_type<lhs_uint_type> const & lhs_region,
        bit_region_type<rhs_uint_type> const & rhs_region
    ) const {
        assert(first_bit <= past_last_bit);
        assert(past_last_bit <= lhs_region.bit_count());
        assert(lhs_region.bit_count() == rhs_region.bit_count());
        std::size_t const first_uint{which<uint_type>(first_bit)};
        std::size_t const past_last_uint{which<uint_type>(past_last_bit)};
        if (first_uint == past_last_uint) {
            uint_type const include_mask_for_single_uint{
             single_uint_mask<uint_type>(first_bit, past_last_bit)};
            // TODO: region.uint_ref() unsafe due to nullptr as the_data_ptr
            return casted_this()->apply_on_partial_uint(
             include_mask_for_single_uint, lhs_region.uint_ref(first_uint),
             rhs_region.uint_ref(first_uint));
        }
        uint_type const include_mask_for_first_uint{
         first_uint_mask<uint_type>(first_bit)};
        typename operator_type::result_type result{
         casted_this()->apply_on_partial_uint(include_mask_for_first_uint,
         lhs_region.uint_ref(first_uint), rhs_region.uint_ref(first_uint))};
        if (casted_this()->partial_result_suffices(result)) {
            return result;
        }
        uint_type const include_mask_for_past_last_uint{
         past_last_uint_mask<uint_type>(past_last_bit)};
        // We test so that we avoid accessing any memory past the last bit:
        // dereferencing that memory may be undefined behaviour.
        if (0 != include_mask_for_past_last_uint) {
            casted_this()->combine_partial_results(result,
             casted_this()->apply_on_partial_uint(
             include_mask_for_past_last_uint,
             lhs_region.uint_ref(past_last_uint),
             rhs_region.uint_ref(past_last_uint)));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        for (std::size_t current_uint{past_last_uint - 1};
         current_uint != first_uint; --current_uint) {
            casted_this()->combine_partial_results(result, casted_this()
             ->apply_on_full_uint(lhs_region.uint_ref(current_uint),
             rhs_region.uint_ref(current_uint)));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        return result;
    }

    template <
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
    auto apply_on_bit_run(
        std::size_t const first_bit,
        std::size_t const past_last_bit,
        bit_region_type<lhs_uint_type> & lhs_region,
        bit_region_type<rhs_uint_type> const & rhs_region
    ) const {
        assert(first_bit <= past_last_bit);
        assert(past_last_bit <= lhs_region.bit_count());
        assert(lhs_region.bit_count() == rhs_region.bit_count());
        std::size_t const first_uint{which<uint_type>(first_bit)};
        std::size_t const past_last_uint{which<uint_type>(past_last_bit)};
        if (first_uint == past_last_uint) {
            uint_type const include_mask_for_single_uint{
             single_uint_mask<uint_type>(first_bit, past_last_bit)};
            // TODO: region.uint_ref() unsafe due to nullptr as the_data_ptr
            return casted_this()->apply_on_partial_uint(
             include_mask_for_single_uint, lhs_region.uint_ref(first_uint),
             rhs_region.uint_ref(first_uint));
        }
        uint_type const include_mask_for_first_uint{
         first_uint_mask<uint_type>(first_bit)};
        typename operator_type::result_type result{
         casted_this()->apply_on_partial_uint(include_mask_for_first_uint,
         lhs_region.uint_ref(first_uint), rhs_region.uint_ref(first_uint))};
        if (casted_this()->partial_result_suffices(result)) {
            return result;
        }
        uint_type const include_mask_for_past_last_uint{
         past_last_uint_mask<uint_type>(past_last_bit)};
        // We test so that we avoid accessing any memory past the last bit:
        // dereferencing that memory may be undefined behaviour.
        if (0 != include_mask_for_past_last_uint) {
            casted_this()->combine_partial_results(result,
             casted_this()->apply_on_partial_uint(
             include_mask_for_past_last_uint,
             lhs_region.uint_ref(past_last_uint),
             rhs_region.uint_ref(past_last_uint)));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        for (std::size_t current_uint{past_last_uint - 1};
         current_uint != first_uint; --current_uint) {
            casted_this()->combine_partial_results(result, casted_this()
             ->apply_on_full_uint(lhs_region.uint_ref(current_uint),
             rhs_region.uint_ref(current_uint)));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        return result;
    }

    template <
        typename lhs_uint_type,
        typename rhs_uint_type,
        typename dst_uint_type,
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
        >,
        typename = std::enable_if_t<
            std::is_same_v<
                std::remove_const_t<uint_type>,
                dst_uint_type
            >
        >
    >
    auto apply_on_bit_run(
        std::size_t const first_bit,
        std::size_t const past_last_bit,
        bit_region_type<dst_uint_type> & destination_region,
        bit_region_type<lhs_uint_type> const & lhs_region,
        bit_region_type<rhs_uint_type> const & rhs_region
    ) const {
        assert(first_bit <= past_last_bit);
        assert(past_last_bit <= destination_region.bit_count());
        assert(lhs_region.bit_count() == destination_region.bit_count());
        assert(rhs_region.bit_count() == destination_region.bit_count());
        std::size_t const first_uint{which<uint_type>(first_bit)};
        std::size_t const past_last_uint{which<uint_type>(past_last_bit)};
        if (first_uint == past_last_uint) {
            uint_type const include_mask_for_single_uint{
             single_uint_mask<uint_type>(first_bit, past_last_bit)};
            // TODO: region.uint_ref() unsafe due to nullptr as the_data_ptr
            return casted_this()->apply_on_partial_uint(
             include_mask_for_single_uint,
             destination_region.uint_ref(first_uint),
             lhs_region.uint_ref(first_uint), rhs_region.uint_ref(first_uint));
        }
        uint_type const include_mask_for_first_uint{
         first_uint_mask<uint_type>(first_bit)};
        typename operator_type::result_type result{
         casted_this()->apply_on_partial_uint(include_mask_for_first_uint,
         destination_region.uint_ref(first_uint),
         lhs_region.uint_ref(first_uint), rhs_region.uint_ref(first_uint))};
        if (casted_this()->partial_result_suffices(result)) {
            return result;
        }
        uint_type const include_mask_for_past_last_uint{
         past_last_uint_mask<uint_type>(past_last_bit)};
        // We test so that we avoid accessing any memory past the last bit:
        // dereferencing that memory may be undefined behaviour.
        if (0 != include_mask_for_past_last_uint) {
            casted_this()->combine_partial_results(result,
             casted_this()->apply_on_partial_uint(
             include_mask_for_past_last_uint,
             destination_region.uint_ref(past_last_uint),
             lhs_region.uint_ref(past_last_uint),
             rhs_region.uint_ref(past_last_uint)));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        for (std::size_t current_uint{past_last_uint - 1};
         current_uint != first_uint; --current_uint) {
            casted_this()->combine_partial_results(result, casted_this()->
             apply_on_full_uint(destination_region.uint_ref(current_uint),
             lhs_region.uint_ref(current_uint),
             rhs_region.uint_ref(current_uint)));
            if (casted_this()->partial_result_suffices(result)) {
                return result;
            }
        }
        return result;
    }

private:
    // Enforce that operator_type is the "derived" type.
    bit_region_uint_operator_type() = default;
    friend operator_type;
};


#endif // EGT_COMPARISON_BIT_REGION_UINT_OPERATOR_TYPE
