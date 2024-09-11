#include "representation/material_side_type.hpp"


#include <algorithm>
#include <cassert>
#include <ostream>

#include "representation/figurine_type.hpp"


namespace representation {


material_side_type::material_side_type(figurine_shapes_type const & shapes)
  : the_material_side(shapes) {
    assert(shapes.valid_material_side());
    std::sort(the_material_side.begin(), the_material_side.end());
}


std::optional<material_side_type> material_side_type::create_opt(
    figurine_shapes_type const & potential_material_side
) {
    if (not potential_material_side.valid_material_side()) return std::nullopt;
    return material_side_type(potential_material_side);
}


std::optional<material_side_type> material_side_type::create_opt(
    figurine_shapes_type && potential_material_side
) {
    if (not potential_material_side.valid_material_side()) return std::nullopt;
    return material_side_type(std::move(potential_material_side));
}


std::optional<material_side_type> material_side_type::create_opt(
    std::initializer_list<figurine_shape_type> const potential_material_side
) {
    figurine_shapes_type shapes(potential_material_side);
    if (not shapes.valid_material_side()) return std::nullopt;
    return material_side_type(std::move(shapes));
}


std::optional<material_side_type> material_side_type::create_opt(
    std::string_view const potential_text_material_side
) {
    std::optional<figurine_shapes_type> shapes_opt{
     figurine_shapes_type::create_opt(potential_text_material_side)};
    if (not shapes_opt or not shapes_opt->valid_material_side()) {
        return std::nullopt;
    }
    figurine_shapes_type const & shapes{*shapes_opt};
    std::vector<figurine_shape_type> shape_vector(shapes.size(), king);
    for (std::size_t i{}; i != shapes.size(); ++i) {
        shape_vector[i] = shapes[i];
    }
    return material_side_type(std::move(shape_vector));
}


std::string material_side_type::as_string(figurine_colour_type colour) const {
    std::string result;
    for (figurine_shape_type const shape: the_material_side) {
        result.push_back(figurine_type(colour, shape).as_char());
    }
    return result;
}


bool material_side_type::castling_can_happen() const {
    assert(the_material_side.contains(king));
    return the_material_side.contains(rook);
}


bool material_side_type::pawnful() const {
    return the_material_side.contains(pawn);
}


bool material_side_type::pawnless() const {
    return not pawnful();
}


figurine_shape_type const & material_side_type::operator[](
    std::size_t const index
) const {
    assert(index < size());
    return the_material_side[index];
}


figurine_shape_type & material_side_type::operator[](std::size_t const index) {
    assert(index < size());
    return the_material_side[index];
}


bool operator==(
    material_side_type const & lhs,
    material_side_type const & rhs
) {
    return lhs.as_figurine_shapes() == rhs.as_figurine_shapes();
}


bool operator!=(
    material_side_type const & lhs,
    material_side_type const & rhs
) {
    return not (lhs == rhs);
}


bool operator<(
    material_side_type const & lhs,
    material_side_type const & rhs
) {
    return lhs.as_figurine_shapes() < rhs.as_figurine_shapes();
}


bool operator>(
    material_side_type const & lhs,
    material_side_type const & rhs
) {
    return rhs < lhs;
}


bool operator<=(
    material_side_type const & lhs,
    material_side_type const & rhs
) {
    return not (lhs > rhs);
}


bool operator>=(
    material_side_type const & lhs,
    material_side_type const & rhs
) {
    return not (lhs < rhs);
}


std::ostream & operator<<(
    std::ostream & out,
    material_side_type const & material_side
) {
    return out << material_side.as_string();
}


} // namespace representation
