#include "representation/figurine_shapes_type.hpp"


#include <algorithm>
#include <ostream>


namespace representation {


std::optional<figurine_shapes_type> figurine_shapes_type::create_opt(
    std::string_view const shapes_as_string_view
) {
    std::vector<figurine_shape_type> shapes;
    for (char const ch: shapes_as_string_view) {
        std::optional<figurine_shape_enum> shape_opt{
         figurine_shape_enum_opt(ch)};
        if (not shape_opt or not valid(*shape_opt)) {
            return std::nullopt;
        }
        shapes.emplace_back(*shape_opt);
    }
    return figurine_shapes_type(shapes);
}


std::string figurine_shapes_type::as_string() const {
    std::string result;
    for (figurine_shape_type const shape: the_shapes) {
        result.push_back(shape.as_char());
    }
    return result;
}


figurine_shape_type const & figurine_shapes_type::operator[](
    std::size_t const index
) const {
    assert(index < size());
    return the_shapes[index];
}


figurine_shape_type & figurine_shapes_type::operator[](
    std::size_t const index
) {
    assert(index < size());
    return the_shapes[index];
}


bool figurine_shapes_type::valid_material_side() const {
    std::array<int, 8> material_counts{};
    using counter_type = decltype(material_counts)::difference_type;
    for (figurine_shape_type const shape: the_shapes) {
        ++material_counts[shape.as_index()];
    }
    if (1 != material_counts[king.as_index()]) return false;
    counter_type const absent_pawns{8 - std::count(the_shapes.cbegin(),
     the_shapes.cend(), pawn)};
    counter_type promoted_pieces{};
    promoted_pieces += std::max(0, material_counts[queen.as_index()] - 1);
    promoted_pieces += std::max(0, material_counts[rook.as_index()] - 2);
    promoted_pieces += std::max(0, material_counts[bishop.as_index()] - 2);
    promoted_pieces += std::max(0, material_counts[knight.as_index()] - 2);
    return absent_pawns >= promoted_pieces;
}


void figurine_shapes_type::push_back(figurine_shape_type const & shape) {
    the_shapes.push_back(shape);
    assert(the_shapes.size() < 10);
}


void figurine_shapes_type::push_back(figurine_shape_type && shape) {
    the_shapes.push_back(std::move(shape));
    assert(the_shapes.size() < 10);
}


bool operator==(
    figurine_shapes_type const & lhs,
    figurine_shapes_type const & rhs
) {
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}


bool operator!=(
    figurine_shapes_type const & lhs,
    figurine_shapes_type const & rhs
) {
    return not (lhs == rhs);
}


bool operator<(
    figurine_shapes_type const & lhs,
    figurine_shapes_type const & rhs
) {
    if (lhs.size() != rhs.size()) {
        return lhs.size() > rhs.size();
    }
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(),
     rhs.cend());
}


bool operator>(
    figurine_shapes_type const & lhs,
    figurine_shapes_type const & rhs
) {
    return rhs < lhs;
}


bool operator<=(
    figurine_shapes_type const & lhs,
    figurine_shapes_type const & rhs
) {
    return not (rhs < lhs);
}


bool operator>=(
    figurine_shapes_type const & lhs,
    figurine_shapes_type const & rhs
) {
    return not (lhs < rhs);
}


std::ostream & operator<<(
    std::ostream & out,
    figurine_shapes_type const & shapes
) {
    return out << shapes.as_string();
}


} // namespace representation
