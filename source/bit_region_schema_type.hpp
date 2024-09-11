#if !defined(EGT_COMPARISON_BIT_REGION_SCHEMA_TYPE_HPP)
#define EGT_COMPARISON_BIT_REGION_SCHEMA_TYPE_HPP


#include <array>
#include <cassert>
#include <cstddef>
#include <memory>
#include <numeric>
#include <type_traits>
#include <vector>


enum class var_kind_enum: unsigned char {
    binary = 0,
    multiple_valued = 1,
    output = 2,
};

inline std::size_t enum_value(var_kind_enum const kind) {
    return static_cast<std::underlying_type_t<var_kind_enum>>(kind);
}


class bit_region_schema_type {
public:
    bit_region_schema_type & operator=(bit_region_schema_type const &)
     = default;
    bit_region_schema_type & operator=(bit_region_schema_type &&) = default;
    bit_region_schema_type(bit_region_schema_type const &) = default;
    bit_region_schema_type(bit_region_schema_type &&) = default;
    ~bit_region_schema_type() = default;

    static std::unique_ptr<bit_region_schema_type> create_ptr(
        std::size_t leading_bvar_count,
        std::vector<std::size_t> const & mvar_bit_widths,
        std::size_t output_bit_count);

    std::size_t first_var(var_kind_enum const kind) const {
       return the_var_kind_borders[enum_value(kind)];
    }
    std::size_t past_last_var(var_kind_enum const kind) const {
        return the_var_kind_borders[1 + enum_value(kind)];
    }
    std::size_t var_count(var_kind_enum const kind) const {
        return past_last_var(kind) - first_var(kind);
    }
    std::size_t var_count() const {
        return the_var_kind_borders.back();
    }
    std::size_t output_var() const {
        assert(past_last_var(var_kind_enum::output) == var_count());
        assert(first_var(var_kind_enum::output) == var_count() - 1);
        return var_count() - 1;
    }

    std::size_t first_bit(std::size_t const var) const {
        assert(var <= var_count());
        return the_var_bit_borders[var];
    }
    std::size_t past_last_bit(std::size_t const var) const {
        assert(var < var_count());
        return the_var_bit_borders[1 + var];
    }
    std::size_t bit_width(std::size_t const var) const {
        return past_last_bit(var) - first_bit(var);
    }
    std::size_t bit_count() const {
        return the_var_bit_borders.back();
    }

private:
    bit_region_schema_type(
        std::size_t const leading_bvar_count,
        std::size_t const input_var_count,
        std::vector<std::size_t> && bit_widths_of_variables
    ) : the_var_bit_borders(1 + bit_widths_of_variables.size())
    {
        std::array<std::size_t, 3> the_var_kind_counts;
        the_var_kind_counts[0] = leading_bvar_count;
        the_var_kind_counts[1] = input_var_count - leading_bvar_count;
        the_var_kind_counts[2] = 1;

        the_var_kind_borders[0] = 0;
        std::inclusive_scan(the_var_kind_counts.cbegin(),
         the_var_kind_counts.cend(), std::next(the_var_kind_borders.begin()));
        assert(the_var_kind_borders.back() == bit_widths_of_variables.size());

        the_var_bit_borders[0] = 0;
        std::inclusive_scan(bit_widths_of_variables.cbegin(),
         bit_widths_of_variables.cend(),
         std::next(the_var_bit_borders.begin()));
    }

    std::array<std::size_t, 4> the_var_kind_borders;
    std::vector<std::size_t> the_var_bit_borders;

    friend bool operator==(
        bit_region_schema_type const & lhs,
        bit_region_schema_type const & rhs);
};


inline bool operator==(
    bit_region_schema_type const & lhs,
    bit_region_schema_type const & rhs
) {
    return (lhs.the_var_kind_borders == rhs.the_var_kind_borders)
     and (lhs.the_var_bit_borders == rhs.the_var_bit_borders);
}


inline bool operator!=(
    bit_region_schema_type const & lhs,
    bit_region_schema_type const & rhs
) {
    return not operator==(lhs, rhs);
}


#endif // EGT_COMPARISON_BIT_REGION_SCHEMA_TYPE_HPP
