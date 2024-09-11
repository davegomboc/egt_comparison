#include "bit_region_schema_type.hpp"


#include <algorithm>


std::unique_ptr<bit_region_schema_type> bit_region_schema_type::create_ptr(
    std::size_t const leading_bvar_count,
    std::vector<std::size_t> const & mvar_bit_widths,
    std::size_t const output_bit_count
) {
    std::unique_ptr<bit_region_schema_type> result;
    std::size_t const mvar_count{mvar_bit_widths.size()};
    std::size_t const input_var_count{leading_bvar_count + mvar_count};
    if ((0 == input_var_count) or (0 == output_bit_count)
     or mvar_bit_widths.cend() != std::find(mvar_bit_widths.cbegin(),
     mvar_bit_widths.cend(), 0)) {
        return result;
    }
    std::vector<std::size_t> bit_widths_of_variables(leading_bvar_count, 2);
    bit_widths_of_variables.insert(bit_widths_of_variables.end(),
     mvar_bit_widths.begin(), mvar_bit_widths.end());
    bit_widths_of_variables.push_back(output_bit_count);
    result.reset(new bit_region_schema_type(leading_bvar_count, input_var_count,
     std::move(bit_widths_of_variables)));
    return result;
}
