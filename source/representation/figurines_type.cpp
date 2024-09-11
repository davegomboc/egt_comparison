#include "representation/figurines_type.hpp"


#include <algorithm>
#include <iostream>


namespace representation {


figurines_type::figurines_type(
    figurine_colour_type const colour,
    figurine_shapes_type const & shapes
) {
    for (figurine_shape_type const shape: shapes) {
        the_figurines.emplace_back(colour, shape);
    }
}


figurines_type::figurines_type(
    figurine_shapes_type const & white_shapes,
    figurine_shapes_type const & black_shapes
) {
    for (figurine_shape_type const shape: white_shapes) {
        the_figurines.emplace_back(white, shape);
    }
    for (figurine_shape_type const shape: black_shapes) {
        the_figurines.emplace_back(black, shape);
    }
}


figurines_type::figurines_type(
    epd::board_contents_type const & board_contents
) {
    for (unsigned char square{}; square != 64; ++square) {
        std::optional<figurine_type> figurine_type_opt{
         figurine_type::create_opt(board_contents[square])};
        if (figurine_type_opt) {
            the_figurines.push_back(std::move(*figurine_type_opt));
        }
    }
}


std::optional<figurines_type> figurines_type::create_opt(
    std::string_view const figurines_as_string_view
) {
    std::vector<figurine_type> figurines;
    for (char const ch: figurines_as_string_view) {
        // For convenience, we ignore characters within "_vs_".
        if (('_' == ch) or ('v' == ch) or ('s' == ch)) continue;
        std::optional<figurine_enum> figurine_opt{figurine_enum_opt(ch)};
        if (not figurine_opt or not valid(*figurine_opt)) {
            return std::nullopt;
        }
        figurines.emplace_back(*figurine_opt);
    }
    return figurines_type(figurines);
}


std::string figurines_type::as_string() const {
    std::string result;
    for (figurine_type const figurine: the_figurines) {
        result.push_back(figurine.as_char());
    }
    return result;
}


figurine_shapes_type figurines_type::shapes(
    figurine_colour_type const colour
) const {
    std::vector<figurine_shape_type> shapes_of_specified_colour;
    for (figurine_type const figurine: the_figurines) {
        if (colour == figurine.colour()) {
            shapes_of_specified_colour.push_back(figurine.shape());
        }
    }
    return figurine_shapes_type(std::move(shapes_of_specified_colour));
}


figurine_type const & figurines_type::operator[](
    std::size_t const index
) const {
    assert(index < size());
    return the_figurines[index];
}


figurine_type & figurines_type::operator[](std::size_t index) {
    assert(index < size());
    return the_figurines[index];
}


bool figurines_type::en_passant_can_happen() const {
    return shapes(white).contains(pawn) and shapes(black).contains(pawn);
}


bool figurines_type::valid_material_balance() const {
    return shapes(white).valid_material_side()
     and shapes(black).valid_material_side();
}


void figurines_type::flip_colours() {
    std::for_each(the_figurines.begin(), the_figurines.end(),
     [&](figurine_type & figurine) { figurine.flip_colour(); });
}


void figurines_type::push_back(figurine_type const & figurine) {
    the_figurines.push_back(figurine);
}


void figurines_type::push_back(figurine_type && figurine) {
    the_figurines.push_back(figurine);
}


bool operator==(figurines_type const & lhs, figurines_type const & rhs) {
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}


bool operator!=(figurines_type const & lhs, figurines_type const & rhs) {
    return not (lhs == rhs);
}


bool operator<(figurines_type const & lhs, figurines_type const & rhs) {
    if (lhs.size() != rhs.size()) {
        return lhs.size() < rhs.size();
    }
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(),
     rhs.cend());
}


bool operator>(figurines_type const & lhs, figurines_type const & rhs) {
    return rhs < lhs;
}


bool operator<=(figurines_type const & lhs, figurines_type const & rhs) {
    return not (rhs < lhs);
}


bool operator>=(figurines_type const & lhs, figurines_type const & rhs) {
    return not (lhs < rhs);
}


std::ostream & operator<<(
    std::ostream & out,
    figurines_type const & figurines
) {
    return out << figurines.as_string();
}


} // namespace representation
