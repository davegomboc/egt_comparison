#include "representation/material_signature_type.hpp"


#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ostream>
#include <regex>

#include "representation/figurine_type.hpp"
#include "representation/figurine_placements_type.hpp"


namespace representation {


std::optional<material_signature_type> material_signature_type::create_opt(
    figurine_shapes_type const & white_shapes,
    figurine_shapes_type const & black_shapes
) {
    std::optional<material_side_type> white_side_opt{
     material_side_type::create_opt(white_shapes)};
    if (not white_side_opt) return std::nullopt;

    std::optional<material_side_type> black_side_opt{
     material_side_type::create_opt(black_shapes)};
    if (not black_side_opt) return std::nullopt;

    return material_signature_type(std::move(*white_side_opt),
     std::move(*black_side_opt));
}


std::optional<material_signature_type> material_signature_type::create_opt(
    figurines_type const & potential_material_signature
) {
    return create_opt(potential_material_signature.shapes(white),
     potential_material_signature.shapes(black));
}


std::optional<material_signature_type> material_signature_type::create_opt(
    std::initializer_list<figurine_type> const potential_material_signature
) {
    figurines_type figurines(potential_material_signature);
    return create_opt(figurines.shapes(white), figurines.shapes(black));
}


std::optional<material_signature_type> material_signature_type::create_opt(
    std::string_view const potential_text_material_signature
) {
    std::optional<figurines_type> figurines_opt{
     figurines_type::create_opt(potential_text_material_signature)};
    if (not figurines_opt) {
        return std::nullopt;
    }

    return material_signature_type::create_opt(*figurines_opt);
}


std::optional<material_signature_type> material_signature_type::find_and_create_opt(
    std::string const & potentially_contains_material_signature
) {
    std::regex const material_signature_regex("K[QRBNP]*[_vs]*k[qrbnp]*");
    std::smatch match_results;
    if (not std::regex_search(potentially_contains_material_signature,
     match_results, material_signature_regex)) {
        return std::nullopt;
    }

    std::optional<figurines_type> figurines_opt{
     figurines_type::create_opt(match_results[0].str())};
    if (not figurines_opt) return std::nullopt;

    return material_signature_type::create_opt(*figurines_opt);
}


bool material_signature_type::canonical() const {
    return the_material_sides[white.as_bool()] <= the_material_sides[black.as_bool()];
}


void material_signature_type::flip_colours() {
    std::swap(the_material_sides[0], the_material_sides[1]);
}


void material_signature_type::canonicalize() {
    if (not canonical()) {
        flip_colours();
    }
}


figurines_type material_signature_type::as_figurines() const {
    return figurines_type(
     the_material_sides[white.as_bool()].as_figurine_shapes(),
     the_material_sides[black.as_bool()].as_figurine_shapes());
}


std::string material_signature_type::as_string() const {
    std::string result{the_material_sides[white.as_bool()].as_string(white)};
    result += "_vs_";
    result += the_material_sides[black.as_bool()].as_string(black);
    return result;
}


std::string material_signature_type::as_syzygy_string() const {
    std::string result{the_material_sides[white.as_bool()].as_string(white)};
    result += 'v';
    result += the_material_sides[black.as_bool()].as_string(white);
    return result;
}


bool material_signature_type::equal() const {
    return std::equal(
     the_material_sides[white.as_bool()].cbegin(),
     the_material_sides[white.as_bool()].cend(),
     the_material_sides[black.as_bool()].cbegin(),
     the_material_sides[black.as_bool()].cend());
}


bool material_signature_type::pawnless() const {
    return the_material_sides[white.as_bool()].pawnless()
     and the_material_sides[black.as_bool()].pawnless();
}


bool material_signature_type::castling_can_happen() const {
    return the_material_sides[white.as_bool()].castling_can_happen()
     or the_material_sides[black.as_bool()].castling_can_happen();
}


bool material_signature_type::en_passant_can_happen() const {
    return the_material_sides[white.as_bool()].pawnful()
     and the_material_sides[black.as_bool()].pawnful();
}


figurine_type material_signature_type::operator[](
    std::size_t const index
) const {
    assert(index < size());
    std::size_t const white_figurine_count{
     the_material_sides[white.as_bool()].size()};
    if (index < white_figurine_count) {
        return figurine_type(white, the_material_sides[white.as_bool()][index]);
    }
    return figurine_type(black,
     the_material_sides[black.as_bool()][index - white_figurine_count]);
}


bool operator==(
    material_signature_type const & lhs,
    material_signature_type const & rhs
) {
    return lhs.side(white) == rhs.side(white)
     and lhs.side(black) == rhs.side(black);
}


bool operator!=(
    material_signature_type const & lhs,
    material_signature_type const & rhs
) {
    return not (lhs == rhs);
}


bool operator<(
    material_signature_type const & lhs,
    material_signature_type const & rhs
) {
    material_side_type const & lhs_white_side{lhs.side(white)};
    std::size_t const lhs_white_side_size{lhs_white_side.size()};
    material_side_type const & lhs_black_side{lhs.side(black)};
    std::size_t const lhs_black_side_size{lhs_black_side.size()};
    material_side_type const & rhs_white_side{rhs.side(white)};
    std::size_t const rhs_white_side_size{rhs_white_side.size()};
    material_side_type const & rhs_black_side{rhs.side(black)};
    std::size_t const rhs_black_side_size{rhs_black_side.size()};
    std::size_t const lhs_size{lhs_white_side_size + lhs_black_side_size};
    std::size_t const rhs_size{rhs_white_side_size + rhs_black_side_size};

    if (lhs_size != rhs_size) {
        return lhs_size < rhs_size;
    }
    if (lhs_white_side_size != rhs_white_side_size) {
        return lhs_white_side_size > rhs_white_side_size;
    }
    if (lhs_white_side != rhs_white_side) {
        return lhs_white_side < rhs_white_side;
    }
    return lhs_black_side < rhs_black_side;
}


bool operator>(
    material_signature_type const & lhs,
    material_signature_type const & rhs
) {
    return not (rhs < lhs);
}


bool operator<=(
    material_signature_type const & lhs,
    material_signature_type const & rhs
) {
    return not (lhs > rhs);
}


bool operator>=(
    material_signature_type const & lhs,
    material_signature_type const & rhs
) {
    return not (lhs < rhs);
}


std::ostream & operator<<(
    std::ostream & out,
    material_signature_type const & material_signature
) {
    return out << material_signature.as_string();
}


} // namespace representation
