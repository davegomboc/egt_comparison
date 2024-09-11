#ifndef EGT_COMPARISON_LOGIC_DIGIT_ENUM_HPP
#define EGT_COMPARISON_LOGIC_DIGIT_ENUM_HPP


#include <array>
#include <cassert>
#include <istream>
#include <ostream>
#include <optional>


enum class logic_digit_enum: unsigned char {
    // For a binary variable input, singular means that the situation is
    // impossible (there is no such cube).  For an output variable, singular
    // means that the cube has no information regarding that output.
    singular    = 0b00,
    // a.k.a., negated
    reset       = 0b01,
    // a.k.a., affirmed
    set         = 0b10,
    // a.k.a., irrelevant
    do_not_care = 0b11
};

static logic_digit_enum constexpr lde_sng{logic_digit_enum::singular};
static logic_digit_enum constexpr lde_off{logic_digit_enum::reset};
static logic_digit_enum constexpr lde_on{logic_digit_enum::set};
static logic_digit_enum constexpr lde_dc{logic_digit_enum::do_not_care};


constexpr bool singular(logic_digit_enum const bit_pair) {
    return lde_sng == bit_pair;
}

constexpr bool valid(logic_digit_enum const bit_pair) {
    return 3 >= static_cast<unsigned char>(bit_pair);
}

constexpr bool valid_and_nonsingular(logic_digit_enum const bit_pair) {
    return valid(bit_pair) and not singular(bit_pair);
}

constexpr unsigned char as_uchar(logic_digit_enum const bit_pair) {
    assert(valid(bit_pair));
    return static_cast<unsigned char>(bit_pair);
}

constexpr char as_char(logic_digit_enum const bit_pair) {
    std::array<char, 4> constexpr symbols{'_', '0', '1', '-'};
    return symbols[as_uchar(bit_pair)];
}

constexpr logic_digit_enum flipped(logic_digit_enum const bit_pair) {
    std::array<logic_digit_enum, 4> constexpr after_flipping{
     lde_sng, lde_on, lde_off, lde_dc};
    return after_flipping[as_uchar(bit_pair)];
}

/*
 * Converts a single character (as found within .pla files) to the appropriate
 * logic_digit_enum:
 *
 * '_', '~', and '?': impossible / no_information / singular
 * '0', '3': negated / reset
 * '1', '4': affirmed / set
 * '2', '-', 'X', 'x': irrelevant / do_not_care
 *
 * Returns std::nullopt for all other characters.
 */
constexpr std::optional<logic_digit_enum> as_logic_digit_enum_opt(
    std::istream::int_type const ch
) {
    switch (ch) {
    case '_':
    case '~':
    case '?':
        return lde_sng;
    case '0':
    case '3':
        return lde_off;
    case '1':
    case '4':
        return lde_on;
    case '2':
    case '-':
    case 'X':
    case 'x':
        return lde_dc;
    default:
        return std::nullopt;
    }
}


/*
 * Consumes leading tabs, spaces, and vertical bars ('|').  If an interpretable
 * character is reached, returns the appropriate logic_digit_enum.  If an
 * attempt to examine what the next character will be ever fails, returns
 * std::nullopt.  If the next character is determined but is not an understood
 * character (even '\n'), then returns std::nullopt with that character ready to
 * be read next from the stream.
 */
std::optional<logic_digit_enum> read_until_logic_digit_enum_opt(
    std::istream & in);


/* Emits the corresponding char (using as_char()) to the output stream. */
inline std::ostream & operator<<(
    std::ostream & out,
    logic_digit_enum bit_pair
) {
    return out.put(as_char(bit_pair));
}


#endif // EGT_COMPARISON_LOGIC_DIGIT_ENUM_HPP
