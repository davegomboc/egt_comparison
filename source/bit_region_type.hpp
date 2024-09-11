#if !defined (EGT_COMPARISON_BIT_REGION_TYPE_HPP)
#define EGT_COMPARISON_BIT_REGION_TYPE_HPP


#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <type_traits>
#include <vector>

#include "binary_stream_io.hpp"
#include "bit_array_display.hpp"
#include "bit_indexing_and_sizing.hpp"
#include "primitive_bit_operations.hpp"


template <typename T>
using is_non_const = std::enable_if_t<not std::is_const_v<T>>;


template <typename uint_type>
class bit_region_type {
    static_assert(std::is_integral_v<uint_type>,
     "Template argument for bit_region_type must be integral.");
    static_assert(std::is_unsigned_v<uint_type>,
     "Template argument for bit_region_type must be unsigned.");

public:
    bit_region_type(std::size_t bit_count, uint_type * data_ptr)
     : the_bit_count(bit_count), the_data_ptr(data_ptr) {
        assert((0 == bit_count) or (nullptr != the_data_ptr));
    }
    template <
        typename data_uint_type,
        typename = std::enable_if_t<
            std::is_same_v<
                std::remove_const<uint_type>,
                std::remove_const<data_uint_type>
            >
        >
    >
    bit_region_type(std::size_t bit_count, data_uint_type * data_ptr)
      : the_bit_count(bit_count),
        the_data_ptr(const_cast<uint_type *>(data_ptr))
    {
        assert((0 == bit_count) or (nullptr != the_data_ptr));
    }
    bit_region_type(bit_region_type const &) = delete;
    bit_region_type(bit_region_type &&) = default;
    bit_region_type & operator=(bit_region_type const &) = delete;
    bit_region_type & operator=(bit_region_type &&) = default;

    // This is not operator=(bit_region_type const &) because it would be
    // surprising if the copy assignment operator relied upon the_bit_count
    // being compatible.
    template <
        typename region_uint_type,
        typename = std::enable_if_t<
            std::is_same_v<
                std::remove_const_t<uint_type>,
                std::remove_const_t<region_uint_type>
            >
        >
    >
    void copy_from(bit_region_type<region_uint_type> const & other) {
        assert(the_bit_count == other.bit_count());
        std::copy_n(other.data_ptr(), other.uint_count(), the_data_ptr);
    }

    bool empty() const {
        return 0 == bit_count();
    }

    std::vector<std::size_t> indices_of_set_bits() const {
        std::vector<std::size_t> result;
        for (std::size_t uint_n{}, bits_before{}; uint_n != uint_count();
         ++uint_n, bits_before += bits_per<uint_type>) {
            std::remove_const_t<uint_type> uint_bits{the_data_ptr[uint_n]};
            while (0 != uint_bits) {
                std::size_t const bit_n_of_uint{
                 find_first_set_bit<std::size_t>(uint_bits) - 1};
                uint_bits &= invert(one_hot_of<uint_type>(bit_n_of_uint));
                result.emplace_back(bits_before + bit_n_of_uint);
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
    bit_region_type & operator&=(
        bit_region_type<region_uint_type> const & other
    ) {
        assert(bit_count() == other.bit_count());
        for (std::size_t uint_n{}; uint_n != uint_count(); ++uint_n) {
            the_data_ptr[uint_n] &= other.uint_ref(uint_n);
        }
        return *this;
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
    bit_region_type & operator|=(
        bit_region_type<region_uint_type> const & other
    ) {
        assert(bit_count() == other.bit_count());
        for (std::size_t uint_n{}; uint_n != uint_count(); ++uint_n) {
            the_data_ptr[uint_n] |= other.uint_ref(uint_n);
        }
        return *this;
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
    bit_region_type & operator^=(
        bit_region_type<region_uint_type> const & other
    ) {
        assert(bit_count() == other.bit_count());
        for (std::size_t uint_n{}; uint_n != uint_count(); ++uint_n) {
            the_data_ptr[uint_n] ^= other.uint_ref(uint_n);
        }
        return *this;
    }

    // Set subtraction (asymmetric difference).
    template <
        typename region_uint_type,
        typename = std::enable_if_t<
            std::is_same_v<
                std::remove_const_t<uint_type>,
                std::remove_const_t<region_uint_type>
            >
        >
    >
    bit_region_type & operator-=(
        bit_region_type<region_uint_type> const & other
    ) {
        assert(bit_count() == other.bit_count());
        for (std::size_t uint_n{}; uint_n != uint_count(); ++uint_n) {
            the_data_ptr[uint_n] &= invert(other.uint_ref(uint_n));
        }
        return *this;
    }

    bool operator[](std::size_t const bit_n) const {
        // Sadly, the_bit_count isn't always set, so this isn't safe to assert.
        // assert(bit_n < bit_count());
        return the_data_ptr[which<uint_type>(bit_n)]
         & one_hot_of<uint_type>(bit_n);
    }

    template <typename T = uint_type, typename = is_non_const<T>>
    bit_region_type & flip(std::size_t const bit_n) {
        assert(bit_n < bit_count());
        std::size_t const which_uint{which<uint_type>(bit_n)};
        uint_type const which_bit{one_hot_of<uint_type>(bit_n)};
        the_data_ptr[which_uint] ^= which_bit;
        return *this;
    }

    template <typename T = uint_type, typename = is_non_const<T>>
    bit_region_type & reset(std::size_t const bit_n) {
        return set(bit_n, false);
    }

    template <typename T = uint_type, typename = is_non_const<T>>
    bit_region_type & set(std::size_t const bit_n, bool const value = true) {
        // Sadly, the_bit_count isn't always set, so this isn't safe to assert.
        //assert(bit_n < bit_count());
        std::size_t const which_uint{which<uint_type>(bit_n)};
        uint_type const which_bit{one_hot_of<uint_type>(bit_n)};
        if (value) {
            the_data_ptr[which_uint] |= which_bit;
        } else {
            the_data_ptr[which_uint] &= invert(which_bit);
        }
        return *this;
    }

    // Returns false on success, or true on an incomplete read.
    template <typename T = uint_type, typename = is_non_const<T>>
    bool read(std::istream & in) {
        return read_bytes(in, sizeof(uint_type) * uint_count(), the_data_ptr);
    }

    // Returns false on success, or true on an incomplete write.
    bool write(std::ostream & out) const {
        return write_bytes(out, sizeof(uint_type) * uint_count(), the_data_ptr);
    }

    std::size_t bit_count() const {
        return the_bit_count;
    }

    std::size_t uint_count() const {
        return required<uint_type>(bit_count());
    }

    uint_type const * data_ptr() const {
        return the_data_ptr;
    }

    uint_type const & uint_ref(std::size_t n) const {
        assert(nullptr != the_data_ptr);
        assert(n < uint_count());
        return the_data_ptr[n];
    }

    uint_type & uint_ref(std::size_t n) {
        assert(nullptr != the_data_ptr);
        assert(n < uint_count());
        return the_data_ptr[n];
    }

    void debug_print(std::ostream & out) const {
        if (nullptr == the_data_ptr) {
            out << "nullptr";
            return;
        }
        debug_print_bits(out, the_data_ptr, 0, the_bit_count,
         bit_grouping_enum::highlight);
        debug_print_bits(out, the_data_ptr, the_bit_count,
         bits_per<uint_type> * required<uint_type>(the_bit_count),
         bit_grouping_enum::padding);
    }

    void probe_debug_print(std::ostream & out) const {
        if (nullptr == the_data_ptr) {
            out << "nullptr";
            return;
        }
        debug_print_bit_pairs(out, the_data_ptr, 0, the_bit_count - 3,
         bit_grouping_enum::highlight);
        out << ' ';
        debug_print_bits(out, the_data_ptr, the_bit_count - 3, the_bit_count,
         bit_grouping_enum::normal);
        debug_print_bits(out, the_data_ptr, the_bit_count,
         bits_per<uint_type> * required<uint_type>(the_bit_count),
         bit_grouping_enum::padding);
    }

private:
    std::size_t the_bit_count;
    uint_type * the_data_ptr;
};

template <typename uint_type> bit_region_type(std::size_t, uint_type *)
 -> bit_region_type<uint_type>;


template <typename uint_type>
std::ostream & operator<<(
    std::ostream & out,
    bit_region_type<uint_type> const & bit_region
) {
    bit_region.debug_print(out);
    return out;
}

template <typename uint_type>
bool operator==(
    bit_region_type<uint_type> const & lhs,
    bit_region_type<uint_type> const & rhs
) {
    std::size_t const bit_count{lhs.bit_count()};
    if (bit_count != rhs.bit_count()) {
        return false;
    }
    if (0 == bit_count) {
        return true;
    }
    std::size_t const last_uint{required<uint_type>(bit_count) - 1};
    if (0 != std::memcmp(lhs.data_ptr(), rhs.data_ptr(),
     last_uint * sizeof(uint_type))) {
        return false;
    }
    uint_type const relevant_bit_mask{
     used_bit_mask_for_final_place<uint_type>(bit_count)};
    // We test so that we avoid accessing any memory past the last bit:
    // dereferencing that memory may be undefined behaviour.
    if (no_bits_set<uint_type> == relevant_bit_mask) return false;
    return (lhs.data_ptr()[last_uint] & relevant_bit_mask)
     == (rhs.data_ptr()[last_uint] & relevant_bit_mask);
}

template <typename uint_type>
inline bool operator!=(
    bit_region_type<uint_type> const & lhs,
    bit_region_type<uint_type> const & rhs
) {
    return not operator==(lhs, rhs);
}

template <typename uint_type>
bool operator<(
    bit_region_type<uint_type> const & lhs,
    bit_region_type<uint_type> const & rhs
) {
    std::size_t const bit_count{lhs.bit_count()};
    if (bit_count != rhs.bit_count()) {
        return bit_count < rhs.bit_count();
    }
    if (0 == bit_count) {
        return false;
    }
    std::size_t const last_uint{required<uint_type>(bit_count) - 1};
    int const memcmp_result{std::memcmp(lhs.data_ptr(), rhs.data_ptr(),
     last_uint * sizeof(uint_type))};
    if (0 != memcmp_result) {
        return memcmp_result < 0;
    }
    uint_type const relevant_bit_mask{
     used_bit_mask_for_final_place<uint_type>(bit_count)};
    // We test so that we avoid accessing any memory past the last bit:
    // dereferencing that memory may be undefined behaviour.
    if (no_bits_set<uint_type> == relevant_bit_mask) return false;
    return (lhs.data_ptr()[last_uint] & relevant_bit_mask)
     < (rhs.data_ptr()[last_uint] & relevant_bit_mask);
}

template <typename uint_type>
inline bool operator>(
    bit_region_type<uint_type> const & lhs,
    bit_region_type<uint_type> const & rhs
) {
    return operator<(rhs, lhs);
}

template <typename uint_type>
inline bool operator>=(
    bit_region_type<uint_type> const & lhs,
    bit_region_type<uint_type> const & rhs
) {
    return not operator<(lhs, rhs);
}

template <typename uint_type>
inline bool operator<=(
    bit_region_type<uint_type> const & lhs,
    bit_region_type<uint_type> const & rhs
) {
    return not operator<(rhs, lhs);
}


#endif // EGT_COMPARISON_BIT_REGION_TYPE
