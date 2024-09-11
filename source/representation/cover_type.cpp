#include "representation/cover_type.hpp"


#include <bitset>
#include <istream>

#include "bit_region_type.hpp"
#include "bit_region_uint_operators.hpp"


static std::array<
    std::array<std::bitset<3>, probe_report_enum_limit>,
    representation::cover_enum_limit
> constexpr outputs_as_bits{
    {   // unk    los    bls    drw    cwn    win    inv    ill    red
        { 0b000, 0b001, 0b011, 0b010, 0b110, 0b100, 0b000, 0b000, 0b000 }, // F
        { 0b000, 0b110, 0b100, 0b010, 0b001, 0b011, 0b000, 0b000, 0b000 }, // R
        { 0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b111, 0b111, 0b111 }, // D
    }
};


std::bitset<3> as_bitset(
    representation::cover_enum const cover,
    probe_report_type const probe_report
) {
    assert(cover_enum_is_valid(cover));
    return outputs_as_bits[static_cast<unsigned char>(cover)]
     [probe_report.as_index()];
}


namespace representation {


void cover_type::read(std::istream & in) {
    assert(in.good());
    for (std::size_t i{}; i != the_cubes.size(); ++i) {
        the_cubes[i].read(in);
    }
    assert(in.good());
}


std::unique_ptr<cover_type> cover_type::create_ptr(
    std::istream & in,
    std::size_t const cube_count,
    std::size_t const per_cube_bit_count
) {
    std::unique_ptr<cover_type> cover_ptr{
     std::make_unique<cover_type>(cube_count, per_cube_bit_count)};
    cover_ptr->read(in);
    if (not cover_ptr->is_sorted()) cover_ptr->sort();
    return in.good() ? std::move(cover_ptr) : nullptr;
}


std::vector<cube_type>::const_iterator cover_type::find_containing_cube(
    cube_type const & contained_cube
) const {
    //std::cerr << "DD: Entered find_containing_cube().\n";
    //std::cerr << "TT: contained_cube = ";
    //contained_cube.const_region().debug_print(std::cerr);
    //std::cerr.put('\n');
    std::vector<cube_type>::const_iterator result{cbegin()};
    while (result != cend()) {
        //std::cerr << "TT: candidate cube = ";
        //result->const_region().debug_print(std::cerr);
        //std::cerr.put('\n');
        if (runs_when_set_lhs_bits_are_subset_of_set_rhs_bits<
         cube_type::uint_type const>(0, contained_cube.bit_count() - 3,
         contained_cube.const_region(), result->const_region())) {
            //std::cerr << "TT:         result = ";
            //result->debug_print(std::cerr);
            //std::cerr << "\nDD: Exiting find_containing_cube().\n";
            return result;
        }
        ++result;
    }
    //std::cerr << "TT: contained_cube = ";
    //contained_cube.const_region().debug_print(std::cerr);
    //std::cerr << " was not actually contained by any candidate cube.\n";
    //std::cerr << "DD: Exiting find_containing_cube().\n";
    return result;
}


cube_type prepare_cube_for_cover(
    logic_digits_type const & logic_digits,
    cover_enum const cover,
    probe_report_type const wpr_probe_report
) {
    std::size_t const bit_count{3 + 2 * logic_digits.size()};
    cube_type result(bit_count);
    for (std::size_t i{}; i != logic_digits.size(); ++i) {
        switch (logic_digits[i].as_logic_digit_enum()) {
        case logic_digit_enum::singular:
            result.reset(2 * i);
            result.reset(2 * i + 1);
            break;
        case logic_digit_enum::reset:
            result.set(2 * i);
            result.reset(2 * i + 1);
            break;
        case logic_digit_enum::set:
            result.reset(2 * i);
            result.set(2 * i + 1);
            break;
        case logic_digit_enum::do_not_care:
            result.set(2 * i);
            result.set(2 * i + 1);
            break;
        default:
            assert(false);
            __builtin_unreachable();
        }
    }
    std::bitset<3> output_bits{as_bitset(cover, wpr_probe_report)};
    for (std::size_t output_bit_n{}; output_bit_n != 3; ++output_bit_n) {
        result.set(bit_count - 3 + output_bit_n, output_bits[output_bit_n]);
    }
    return result;
}


} // namespace representation;
