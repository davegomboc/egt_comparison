#if !defined(EGT_COMPARISON_REPRESENTATION_EGT_POSITION_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_EGT_POSITION_TYPE_HPP


#include <cassert>
#include <cstdint>
#include <iosfwd>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include "epd/board_contents_type.hpp"
#include "epd/castling_rights_type.hpp"
#include "epd/extended_position_description_type.hpp"
#include "epd/side_to_move_type.hpp"

#include "representation/canonicalization_enum.hpp"
#include "representation/coordinate_type.hpp"
#include "representation/en_passant_information_type.hpp"
#include "representation/figurine_colour_type.hpp"
#include "representation/figurine_placements_type.hpp"
#include "representation/material_side_type.hpp"
#include "representation/material_signature_type.hpp"


namespace representation {


inline std::vector<bool> as_bool_vector_id(
    std::size_t const input_bit_pair_count,
    std::size_t const position_id
) {
    assert(0 == (input_bit_pair_count - 1) % 6);
    std::vector<bool> result;
    for (std::size_t bit_pair_n{}; bit_pair_n != input_bit_pair_count;
     ++bit_pair_n) {
        result.push_back(position_id & (1 << bit_pair_n) ? true : false);
    }
    std::reverse(result.begin(), result.end());
    return result;
}


inline std::vector<unsigned char> as_mtbdd_uchar_vector_id(
    std::size_t const input_bit_pair_count,
    std::size_t const position_id
) {
    assert(0 == (input_bit_pair_count - 1) % 6);
    std::vector<unsigned char> result;
    for (std::size_t bit_pair_n{}; bit_pair_n != input_bit_pair_count;
     ++bit_pair_n) {
        result.push_back(position_id & (1 << bit_pair_n) ? 1 : 0);
    }
    // The multiterminal binary decision diagram code uses an extra initial
    // unsigned char.
    result.push_back(0);

    std::reverse(result.begin(), result.end());
    return result;
}


inline std::size_t as_position_id(std::vector<bool> const id_as_bool_vector) {
    std::size_t result{};
    for (auto iter{id_as_bool_vector.cbegin()};
     iter != id_as_bool_vector.cend(); ++iter) {
        result <<= 1;
        result |= *iter;
    }
    return result;
}


inline std::size_t as_position_id(
    std::vector<unsigned char> const id_as_mtbdd_uchar_vector
) {
    std::size_t result{};
    for (auto iter{id_as_mtbdd_uchar_vector.cbegin()};
     iter != id_as_mtbdd_uchar_vector.cend(); ++iter) {
        result <<= 1;
        assert((1 == *iter) or (0 == *iter));
        result |= *iter ? 1u : 0u;
    }
    return result;
}


class egt_position_type {
public:
    egt_position_type & operator=(egt_position_type const &) = default;
    egt_position_type & operator=(egt_position_type &&) = default;
    egt_position_type(egt_position_type const &) = default;
    egt_position_type(egt_position_type &&) = default;
    ~egt_position_type() = default;

    egt_position_type(
        figurine_colour_type side_to_move,
        figurine_placements_type const & placements,
        coordinate_type en_passant_target,
        epd::castling_rights_type castling_rights
         = epd::castling_rights_type::prohibit_all());

    egt_position_type(
        epd::board_contents_type const & board_contents,
        epd::side_to_move_type const side_to_move,
        epd::castling_rights_type const castling_rights,
        epd::en_passant_target_type const en_passant_target);

    explicit egt_position_type(
        epd::extended_position_description_type const & epd);

    egt_position_type(
        material_signature_type const & material_signature,
        std::size_t const position_id
    ) : the_colour_to_move{figurine_colour_enum::black},
        the_placements{},
        the_en_passant_target{no_ep},
        the_castling_rights(epd::castling_rights_type::prohibit_all())
    {
        cube_schema_type const cube_schema{material_signature.cube_schema()};
        assert(position_id < cube_schema.position_count());
        std::size_t const wtm_position_count{
         cube_schema.single_side_to_move_position_count()};
        the_colour_to_move = (position_id < wtm_position_count) ? white : black;
        the_placements = figurine_placements_type(material_signature,
         position_id);
        the_en_passant_target = decode_illegal_pawn_placement();
    }

    template <typename uint_type>
    egt_position_type(
        material_signature_type const & material_signature,
        std::vector<uint_type> & vector_id
    ) : the_colour_to_move{figurine_colour_enum::black},
        the_placements{},
        the_en_passant_target{no_ep},
        the_castling_rights(epd::castling_rights_type::prohibit_all())
    {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
        assert(1 + 6 * material_signature.size() <= vector_id.size());
        the_placements = figurine_placements_type(material_signature, vector_id);
        the_colour_to_move = figurine_colour_type(0 != vector_id.back());
        vector_id.pop_back();
        assert(vector_id.empty());
        the_en_passant_target = decode_illegal_pawn_placement();
    }

    bool black_to_move() const {
        return black == the_colour_to_move;
    }
    bool white_to_move() const {
        return white == the_colour_to_move;
    }

    figurine_colour_type colour_to_move() const {
        return the_colour_to_move;
    }

    epd::side_to_move_type side_to_move() const {
        return the_colour_to_move.as_side_to_move();
    }

    representation::figurine_placements_type const & placements() const {
        return the_placements;
    }

    epd::castling_rights_type castling_rights() const {
        return the_castling_rights;
    }

    representation::coordinate_type en_passant_target() const {
        return the_en_passant_target;
    }

    std::optional<epd::extended_position_description_type>
    as_extended_position_description_opt() const;

    std::size_t as_position_id() const;
    std::vector<bool> as_bool_vector_id() const;
    std::vector<unsigned char> as_mtbdd_uchar_vector_id() const;

    logic_digits_type as_logic_digits() const;

    representation::material_side_type as_material_side(
        representation::figurine_colour_type) const;

    representation::material_signature_type as_material_signature() const;

    std::optional<tb_position_type> as_tb_position_opt() const;

    std::string as_string() const;

    std::vector<egt_position_type> symmetrical_positions() const;
    egt_position_type canonical_position(canonicalization_enum) const;
    void canonicalize(canonicalization_enum);

    // N.B. These five functions don't themselves maintain the invariant of
    // the_placements.
    void flip_a8h1s();
    void flip_colours();
    void flip_files();
    void flip_ranks();
    void invert();
    void restore_ordering_invariant();

    // When an illegally-placed pawn might reasonably represent a potential en
    // passant capturing opportunity, then the object is updated appopriately,
    // and the identified en passant target square is returned.  Otherwise, the
    // object remains unchanged, and no_ep is returned.
    coordinate_type decode_illegal_pawn_placement();

private:
    std::optional<en_passant_information_type> en_passant_information_opt() const;

    // Useful when a) converting an egt_position to logic digits and b) probing
    // an egt_position.
    figurine_placements_type encoded_placements() const;

    // Invert the above decoding when en passant applies.
    void encode_illegal_pawn_placement();

    std::vector<egt_position_type> pawnful_symmetrical_positions() const;
    std::vector<egt_position_type>
     equal_material_pawnless_symmetrical_positions() const;
    std::vector<egt_position_type>
     nonequal_material_pawnless_symmetrical_positions() const;

    figurine_colour_type the_colour_to_move;
    representation::figurine_placements_type the_placements;
    representation::coordinate_type the_en_passant_target;
    epd::castling_rights_type the_castling_rights;
};


bool operator==(egt_position_type const & lhs, egt_position_type const & rhs);
bool operator!=(egt_position_type const & lhs, egt_position_type const & rhs);
bool operator<(egt_position_type const & lhs, egt_position_type const & rhs);
bool operator>(egt_position_type const & lhs, egt_position_type const & rhs);
bool operator<=(egt_position_type const & lhs, egt_position_type const & rhs);
bool operator>=(egt_position_type const & lhs, egt_position_type const & rhs);


std::ostream & operator<<(
    std::ostream & out,
    egt_position_type const & position);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_EGT_POSITION_TYPE_HPP
