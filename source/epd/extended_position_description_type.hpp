#if !defined(EGT_COMPARISON_EPD_EXTENDED_POSITION_DESCRIPTION_TYPE_HPP)
#define EGT_COMPARISON_EPD_EXTENDED_POSITION_DESCRIPTION_TYPE_HPP


#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>


#include "epd/board_contents_type.hpp"
#include "epd/castling_rights_type.hpp"
#include "epd/en_passant_target_type.hpp"
#include "epd/operations_type.hpp"
#include "epd/side_to_move_type.hpp"


namespace epd {


// Removes any prefix of leading whitespace, then the prefix that specifies the
// board contents from epd_text.
std::optional<board_contents_type>
extract_board_contents_opt(std::string_view & epd_text);

// Removes any prefix of leading whitespace, then the prefix that specifies the
// side to move from remaining_epd_text.
std::optional<side_to_move_type>
extract_side_to_move_opt(std::string_view & remaining_epd_text);

// Removes any prefix of leading whitespace, then the prefix that specifies the
// castling rights from remaining_epd_text.
std::optional<castling_rights_type>
extract_castling_rights_opt(std::string_view & remaining_epd_text);

// Removes any prefix of leading whitespace, then the prefix that specifies the
// en passant target square from remaining_epd_text.
std::optional<en_passant_target_type>
extract_en_passant_target_opt(std::string_view & remaining_epd_text);


class extended_position_description_type {
public:
    extended_position_description_type &
     operator=(extended_position_description_type const &) = default;
    extended_position_description_type &
     operator=(extended_position_description_type &&) = default;
    extended_position_description_type(
     extended_position_description_type const &) = default;
    extended_position_description_type(
     extended_position_description_type &&) = default;
    ~extended_position_description_type() = default;

    extended_position_description_type();

    extended_position_description_type(
     board_contents_type const & board_contents,
     side_to_move_type const side_to_move,
     castling_rights_type const & castling_rights,
     en_passant_target_type const en_passant_target,
     operations_type const & operations);

    static std::optional<extended_position_description_type> create(
        std::string_view const epd);
    static std::optional<extended_position_description_type> create_from_fen(
        std::string_view const fen);

    board_contents_type const & board_contents() const {
        return the_board_contents;
    }
    board_contents_type & board_contents() {
        return the_board_contents;
    }

    side_to_move_type const & side_to_move() const {
        return the_side_to_move;
    }
    side_to_move_type & side_to_move() {
        return the_side_to_move;
    }

    castling_rights_type const & castling_rights() const {
        return the_castling_rights;
    }
    castling_rights_type & castling_rights() {
        return the_castling_rights;
    }

    en_passant_target_type const & en_passant_target() const {
        return the_en_passant_target;
    }
    en_passant_target_type & en_passant_target() {
        return the_en_passant_target;
    }

    operations_type const & operations() const {
        return the_operations;
    }
    operations_type & operations() {
        return the_operations;
    }

    std::string as_epd() const;
    std::optional<std::string> as_fen_opt() const;

private:
    static const opcode_type the_hmvc_opcode;
    static const opcode_type the_fmvn_opcode;

    extended_position_description_type(
        board_contents_type && board_contents,
        side_to_move_type && side_to_move,
        castling_rights_type && castling_rights,
        en_passant_target_type && en_passant_target,
        operations_type && operations
    ) : the_board_contents(std::move(board_contents)),
        the_side_to_move(std::move(side_to_move)),
        the_castling_rights(std::move(castling_rights)),
        the_en_passant_target(std::move(en_passant_target)),
        the_operations(std::move(operations)) {}

    board_contents_type the_board_contents;
    side_to_move_type the_side_to_move;
    castling_rights_type the_castling_rights;
    en_passant_target_type the_en_passant_target;
    operations_type the_operations;
};


std::ostream & operator<<(
    std::ostream & out,
    extended_position_description_type const & extended_position_description);


} // namespace epd


#endif // EGT_COMPARISON_EPD_EXTENDED_POSITION_DESCRIPTION_TYPE_HPP
