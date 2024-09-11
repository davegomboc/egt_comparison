#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_COLOUR_ENUM_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_COLOUR_ENUM_HPP


#include <cassert>
#include <cstdint>
#include <string_view>

#include "epd/side_to_move_type.hpp"

#include "representation/logic_digit_type.hpp"


namespace representation {


constexpr std::array<std::string_view, 2> const figurine_colour_descriptions = {
    "white", "black",
};


constexpr std::array<std::string_view, 2> const
figurine_colour_side_to_move_initials = {
    "wtm", "btm",
};


enum class figurine_colour_enum: unsigned char {
    white = 0x00,
    black = 0x08,
    mask = 0x08
};


constexpr bool valid(figurine_colour_enum const colour) {
    unsigned char const index{static_cast<unsigned char>(colour)};
    return 0 == (index & ~(static_cast<unsigned char>(
     figurine_colour_enum::mask)));
}


constexpr bool as_bool(figurine_colour_enum const colour) {
    assert(valid(colour));
    return (figurine_colour_enum::white == colour ? false : true);
}


constexpr char as_char(figurine_colour_enum const colour) {
    assert(valid(colour));
    return as_bool(colour) ? 'b' : 'w';
}


constexpr unsigned char as_index(figurine_colour_enum const colour) {
    assert(valid(colour));
    return static_cast<unsigned char>(colour);
}


constexpr logic_digit_type as_logic_digit(figurine_colour_enum const colour) {
    return as_bool(colour) ? on : off;
}


constexpr std::string_view as_description(
    figurine_colour_enum const colour
) {
    assert(valid(colour));
    return figurine_colour_descriptions[as_bool(colour)];
}


constexpr std::string_view as_side_to_move_initials(
    figurine_colour_enum const colour
) {
    assert(valid(colour));
    return figurine_colour_side_to_move_initials[as_bool(colour)];
}


constexpr epd::side_to_move_type as_side_to_move(
    figurine_colour_enum const colour
) {
    assert(valid(colour));
    return epd::side_to_move_type(figurine_colour_enum::black == colour);
}

constexpr void flip(figurine_colour_enum & colour) {
    colour = static_cast<figurine_colour_enum>(static_cast<unsigned char>(
     colour) ^ static_cast<unsigned char>(figurine_colour_enum::mask));
}


std::ostream & operator<<(std::ostream & out, figurine_colour_enum colour);


} // namespace representation;


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_COLOUR_ENUM_HPP
