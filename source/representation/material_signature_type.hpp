#if !defined(EGT_COMPARISON_REPRESENTATION_MATERIAL_SIGNATURE_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_MATERIAL_SIGNATURE_TYPE_HPP


#include <array>
#include <functional>
#include <initializer_list>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "representation/cube_schema_type.hpp"
#include "representation/figurine_type.hpp"
#include "representation/figurines_type.hpp"
#include "representation/material_side_type.hpp"


namespace representation {


class material_signature_type {
public:
    material_signature_type & operator=(
     material_signature_type const &) = default;
    material_signature_type & operator=(
     material_signature_type &&) = default;
    material_signature_type(material_signature_type const &) = default;
    material_signature_type(material_signature_type &&) = default;
    ~material_signature_type() = default;

    material_signature_type()
      : the_material_sides{material_side_type(), material_side_type()} {}
    material_signature_type(
        material_side_type && white_side,
        material_side_type && black_side
    ) : the_material_sides{white_side, black_side} {}

    static std::optional<material_signature_type> create_opt(
        figurine_shapes_type const & white_shapes,
        figurine_shapes_type const & black_shapes);
    static std::optional<material_signature_type> create_opt(
        figurines_type const &);
    static std::optional<material_signature_type> create_opt(
        std::initializer_list<figurine_type>);
    static std::optional<material_signature_type> create_opt(std::string_view);

    static std::optional<material_signature_type> find_and_create_opt(
        std::string const &);

    std::size_t size() const {
        return the_material_sides[0].size() + the_material_sides[1].size();
    }

    material_side_type const & side(figurine_colour_type const colour) const {
        return the_material_sides[colour.as_bool()];
    }

    bool canonical() const;
    void flip_colours();
    void canonicalize();

    figurines_type as_figurines() const;

    std::string as_string() const;
    std::string as_syzygy_string() const;

    bool equal() const;
    bool pawnless() const;
    bool castling_can_happen() const;
    bool en_passant_can_happen() const;

    figurine_type operator[](std::size_t index) const;

    cube_schema_type cube_schema() const {
        std::size_t const side_to_move_input_count{1};
        std::size_t const per_figurine_input_count{6};
        std::size_t const output_count{3};
        return cube_schema_type(side_to_move_input_count
         + per_figurine_input_count * size(), output_count);
    }

private:
    std::array<material_side_type, 2> the_material_sides;
};


bool operator==(
 material_signature_type const & lhs, material_signature_type const & rhs);
bool operator!=(
 material_signature_type const & lhs, material_signature_type const & rhs);
bool operator<(
 material_signature_type const & lhs, material_signature_type const & rhs);
bool operator>(
 material_signature_type const & lhs, material_signature_type const & rhs);
bool operator<=(
 material_signature_type const & lhs, material_signature_type const & rhs);
bool operator>=(
 material_signature_type const & lhs, material_signature_type const & rhs);


std::ostream & operator<<(
    std::ostream & out,
    material_signature_type const & material_signature);


} // namespace representation


namespace std {


template <> class hash<representation::material_signature_type> {
public:
    std::size_t operator()(
        representation::material_signature_type const & material_signature
    ) const {
        return hash<string>()(material_signature.as_string());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_MATERIAL_SIGNATURE_TYPE_HPP
