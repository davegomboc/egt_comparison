#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPE_ENUM_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPE_ENUM_HPP


#include <cctype>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <type_traits>

#include "representation/logic_digits_type.hpp"


namespace representation {


// Use when rendering figurine shapes, but the display font does not include
// bitmaps for Unicode Chess figurines.
constexpr std::array<char, 8> const figurine_shapes_as_ascii_char = {
    '\0', 'K', 'Q', 'R', 'B', 'N', 'P', '\0',
};

// Use when rendering figurine shapes.
constexpr std::array<std::string_view, 8> const figurine_shapes_as_utf8 = {
    "",
    "\xe2\x99\x9a",
    "\xe2\x99\x9b",
    "\xe2\x99\x9c",
    "\xe2\x99\x9d",
    "\xe2\x99\x9e",
    "\xe2\x99\x9f",
    "",
};

constexpr std::array<std::string_view, 8> const figurine_shape_descriptions = {
    "", "king", "queen", "rook", "bishop", "knight", "pawn", "",
};

// Use when rendering figurines, but the display font does not include bitmaps
// for Unicode Chess figurines.
constexpr std::array<char, 16> const figurines_as_ascii_char = {
    '\0', 'K', 'Q', 'R', 'B', 'N', 'P', '\0',
    '\0', 'k', 'q', 'r', 'b', 'n', 'p', '\0',
};

// Use when rendering figurines as done for analysis in Chess books when Unicode
// Chess figurines are not supported.  Different ASCII symbols are used for the
// white and black figurines.
constexpr std::array<std::string_view, 16> const figurines_as_ascii = {
     "", "K", "Q", "R", "B", "N", "P", "",
     "", "k", "q", "r", "b", "n", "p", "",
};

// Use when rendering figurines as done for analysis in Chess books.  Different
// (non-ASCII) Unicode symbols are used for the white and black figurines.
constexpr std::array<std::string_view, 16> const standalone_figurines_as_utf8 = {
    "",
    "\xe2\x99\x94",
    "\xe2\x99\x95",
    "\xe2\x99\x96",
    "\xe2\x99\x97",
    "\xe2\x99\x98",
    "\xe2\x99\x99",
    "",
    "",
    "\xe2\x99\x9a",
    "\xe2\x99\x9b",
    "\xe2\x99\x9c",
    "\xe2\x99\x9d",
    "\xe2\x99\x9e",
    "\xe2\x99\x9f",
    "",
};

// Use when rendering figurines in Chess board diagrams.
constexpr std::array<std::string_view, 16> const foreground_figurines_as_utf8 = {
    "",
    "\xe2\x99\x9a",
    "\xe2\x99\x9b",
    "\xe2\x99\x9c",
    "\xe2\x99\x9d",
    "\xe2\x99\x9e",
    "\xe2\x99\x9f",
    "",
    "",
    "\xe2\x99\x9a",
    "\xe2\x99\x9b",
    "\xe2\x99\x9c",
    "\xe2\x99\x9d",
    "\xe2\x99\x9e",
    "\xe2\x99\x9f",
    "",
};


enum class figurine_shape_enum: unsigned char {
    king = 0x01,
    queen = 0x02,
    rook = 0x03,
    bishop = 0x04,
    knight = 0x05,
    pawn = 0x06,
    mask = 0x07,
};


// We intentionally accept both lower and upper-case ASCII characters.
constexpr std::optional<figurine_shape_enum> figurine_shape_enum_opt(
    char const ch
) {
    unsigned char const uch{static_cast<unsigned char>(ch)};
    for (unsigned int i{1}; i != 15; ++i) {
        if ((7 == i) or (8 == i)) {
            continue;
        }
        if (uch == static_cast<unsigned char>(figurines_as_ascii_char[i])) {
            return static_cast<figurine_shape_enum>(i < 8 ? i : i - 8);
        }
    }
    return std::nullopt;
}


constexpr bool valid(figurine_shape_enum const shape) {
    unsigned char const index{static_cast<unsigned char>(shape)};
    return (0 < index) and (index < 7);
}


constexpr unsigned char as_index(figurine_shape_enum const shape) {
    assert(valid(shape));
    return static_cast<std::underlying_type_t<figurine_shape_enum>>(shape);
}


constexpr char as_char(figurine_shape_enum const shape) {
    assert(valid(shape));
    return figurine_shapes_as_ascii_char[as_index(shape)];
}


constexpr std::string_view as_description(figurine_shape_enum const shape) {
    assert(valid(shape));
    return figurine_shape_descriptions[as_index(shape)];
}


bool next(figurine_shape_enum & shape);
bool prev(figurine_shape_enum & shape);
figurine_shape_enum & operator++(figurine_shape_enum & shape);
figurine_shape_enum & operator--(figurine_shape_enum & shape);


logic_digits_type as_logic_digits(figurine_shape_enum shape);
logic_digits_type as_one_hot_logic_digits(figurine_shape_enum shape);


std::ostream & operator<<(std::ostream & out, figurine_shape_enum shape);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPE_ENUM_HPP
