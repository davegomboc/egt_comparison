#include "representation/cube_type.hpp"


#include <ostream>
#include <sstream>


namespace representation {


void cube_type::print(
    std::ostream & out,
    bit_region_schema_type const & schema,
    std::string_view const output_map
) const {
    assert(2 == output_map.length());
    std::size_t const past_last_bbit{
     schema.first_bit(schema.past_last_var(var_kind_enum::binary))};
    std::size_t const past_last_mbit{
     schema.first_bit(schema.past_last_var(var_kind_enum::multiple_valued))};
    std::size_t const past_last_obit{
     schema.first_bit(schema.past_last_var(var_kind_enum::output))};

    for (std::size_t bit_n{}; bit_n != past_last_bbit; bit_n += 2) {
        char const ch{(*this)[bit_n] ? ((*this)[1 + bit_n] ? '-' : '0')
         : ((*this)[1 + bit_n] ? '1' : '_')};
        out << ch;
    }
    for (std::size_t bit_n{past_last_bbit}; bit_n != past_last_mbit; ++bit_n) {
        char const ch{(*this)[bit_n] ? '1' : '~'};
        out << ch;
    }
    out << ' ';
    for (std::size_t bit_n{past_last_mbit}; bit_n != past_last_obit; ++bit_n) {
        char const ch{output_map[(*this)[bit_n]]};
        out << ch;
    }
}


std::string cube_type::as_string(
    bit_region_schema_type const & schema,
    std::string_view const output_map
) const {
    std::ostringstream out;
    print(out, schema, output_map);
    return out.str();
}


} // namespace representation
