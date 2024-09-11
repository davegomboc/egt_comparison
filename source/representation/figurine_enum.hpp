#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_ENUM_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_ENUM_HPP


#include <bitset>
#include <iosfwd>
#include <optional>
#include <string_view>

#include "representation/figurine_colour_enum.hpp"
#include "representation/figurine_shape_enum.hpp"
#include "representation/logic_digits_type.hpp"


namespace representation {


enum class figurine_enum: unsigned char {
    white_king = static_cast<unsigned char>(figurine_colour_enum::white)
     | static_cast<unsigned char>(figurine_shape_enum::king),
    white_queen = static_cast<unsigned char>(figurine_colour_enum::white)
     | static_cast<unsigned char>(figurine_shape_enum::queen),
    white_rook = static_cast<unsigned char>(figurine_colour_enum::white)
     | static_cast<unsigned char>(figurine_shape_enum::rook),
    white_bishop = static_cast<unsigned char>(figurine_colour_enum::white)
     | static_cast<unsigned char>(figurine_shape_enum::bishop),
    white_knight = static_cast<unsigned char>(figurine_colour_enum::white)
     | static_cast<unsigned char>(figurine_shape_enum::knight),
    white_pawn = static_cast<unsigned char>(figurine_colour_enum::white)
     | static_cast<unsigned char>(figurine_shape_enum::pawn),
    black_king = static_cast<unsigned char>(figurine_colour_enum::black)
     | static_cast<unsigned char>(figurine_shape_enum::king),
    black_queen = static_cast<unsigned char>(figurine_colour_enum::black)
     | static_cast<unsigned char>(figurine_shape_enum::queen),
    black_rook = static_cast<unsigned char>(figurine_colour_enum::black)
     | static_cast<unsigned char>(figurine_shape_enum::rook),
    black_bishop = static_cast<unsigned char>(figurine_colour_enum::black)
     | static_cast<unsigned char>(figurine_shape_enum::bishop),
    black_knight = static_cast<unsigned char>(figurine_colour_enum::black)
     | static_cast<unsigned char>(figurine_shape_enum::knight),
    black_pawn = static_cast<unsigned char>(figurine_colour_enum::black)
     | static_cast<unsigned char>(figurine_shape_enum::pawn),
};


constexpr std::optional<figurine_enum> figurine_enum_opt(char const ch) {
    for (unsigned int i{1}; i != 7; ++i) {
        if (figurines_as_ascii_char[i] == ch) {
            return static_cast<figurine_enum>(i);
        }
        if (figurines_as_ascii_char[i + 8] == ch) {
            return static_cast<figurine_enum>(i + 8);
        }
    }
    return std::nullopt;
}


constexpr figurine_shape_enum just_shape(figurine_enum const figurine) {
    return static_cast<figurine_shape_enum>(static_cast<unsigned char>(figurine)
     & static_cast<unsigned char>(figurine_shape_enum::mask));
}


constexpr figurine_colour_enum just_colour(figurine_enum const figurine) {
    return static_cast<figurine_colour_enum>(static_cast<unsigned char>(
     figurine) & static_cast<unsigned char>(figurine_colour_enum::mask));
}


constexpr bool valid(figurine_enum const figurine) {
    unsigned char const index{static_cast<unsigned char>(figurine)};
    return (index < 16) and valid(just_shape(figurine)) and
     valid(just_colour(figurine));
}


constexpr unsigned char as_index(figurine_enum const figurine) {
    assert(valid(figurine));
    return static_cast<std::underlying_type_t<figurine_enum>>(figurine);
}


constexpr char as_char(std::optional<figurine_enum> const figurine_enum_opt) {
    if (not figurine_enum_opt) {
        return ' ';
    }

    assert(valid(*figurine_enum_opt));
    return figurines_as_ascii_char[as_index(*figurine_enum_opt)];
}


constexpr std::string_view as_standalone_utf8(
    std::optional<figurine_enum> const figurine_enum_opt
) {
    if (not figurine_enum_opt) {
        return standalone_figurines_as_utf8[0];
    }
    assert(valid(*figurine_enum_opt));
    return standalone_figurines_as_utf8[as_index(*figurine_enum_opt)];
}


constexpr figurine_enum operator|(
    figurine_colour_enum const colour,
    figurine_shape_enum const shape
) {
    assert(valid(colour));
    assert(valid(shape));
    return static_cast<figurine_enum>(
     static_cast<unsigned char>(colour) | static_cast<unsigned char>(shape));
}


constexpr std::bitset<4> as_bitset(figurine_enum const figurine) {
    return std::bitset<4>(static_cast<unsigned char>(figurine));
}


constexpr std::string_view disable_escape_code_formatting() {
    return "\x1b[0m";
}


inline std::string enable_colour_escape_code(
    bool const light_square,
    std::optional<figurine_enum> const figurine_enum_opt
) {
    std::string result{"\x1b[4"};
    result.push_back(light_square ? '6' : '1');
    if (figurine_enum_opt) {
        result += ";3";
        result.push_back(figurine_colour_enum::black
         == just_colour(*figurine_enum_opt) ? '0' : '7');
    }
    result.push_back('m');
    return result;
}


inline std::string as_board_diagram_square(
    std::optional<figurine_enum> const figurine_enum_opt,
    bool const light_square,
    bool const use_utf8
) {
    std::string result{enable_colour_escape_code(light_square,
     figurine_enum_opt)};
    if (figurine_enum_opt) {
        assert(valid(*figurine_enum_opt));
        unsigned char const uch{as_index(*figurine_enum_opt)};
        if (use_utf8) {
            result += foreground_figurines_as_utf8[uch];
        } else {
            result.push_back(figurines_as_ascii_char[uch]);
        }
    } else {
        result.push_back(' ');
    }
    result += disable_escape_code_formatting();
    return result;
}


logic_digits_type as_logic_digits(figurine_enum figurine);


std::ostream & operator<<(std::ostream & out, figurine_enum figurine);


constexpr figurine_enum flipped_colour(figurine_enum const figurine) {
    return static_cast<figurine_enum>(static_cast<unsigned char>(figurine)
     ^ static_cast<unsigned char>(figurine_colour_enum::mask));
}


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_ENUM_HPP
