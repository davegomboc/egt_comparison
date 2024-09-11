#if !defined(EGT_COMPARISON_REPRESENTATION_BITBOARDS_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_BITBOARDS_TYPE_HPP


#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string_view>
#include <vector>

#include "epd/board_contents_type.hpp"

#include "representation/bitboard_type.hpp"
#include "representation/figurine_placement_type.hpp"
#include "representation/logic_digits_type.hpp"
#include "fathom.hpp"


namespace representation {


constexpr std::array<std::string_view, 8> bitboard_names = {
 "white", "black", "king", "queen", "rook", "bishop", "knight", "pawn"};


constexpr std::string_view bitboard_name(std::size_t const bitboard_index) {
    assert(bitboard_index < 8);
    return bitboard_names[bitboard_index];
}


class bitboards_type {
public:
    static constexpr unsigned int white_bits{};
    static constexpr unsigned int black_bits{1};
    static constexpr unsigned int king_bits{2};
    static constexpr unsigned int queen_bits{3};
    static constexpr unsigned int rook_bits{4};
    static constexpr unsigned int bishop_bits{5};
    static constexpr unsigned int knight_bits{6};
    static constexpr unsigned int pawn_bits{7};

    bitboards_type() {}
    bitboards_type(
        bitboard_type white,
        bitboard_type black,
        bitboard_type kings,
        bitboard_type queens,
        bitboard_type rooks,
        bitboard_type bishops,
        bitboard_type knights,
        bitboard_type pawns
    ) : the_bitboards{
     white, black, kings, queens, rooks, bishops, knights, pawns} {}
    bitboards_type(epd::board_contents_type board_contents);

    bitboard_type white() const { return the_bitboards[white_bits]; }
    bitboard_type black() const { return the_bitboards[black_bits]; }
    bitboard_type kings() const { return the_bitboards[king_bits]; }
    bitboard_type queens() const { return the_bitboards[queen_bits]; }
    bitboard_type rooks() const { return the_bitboards[rook_bits]; }
    bitboard_type bishops() const { return the_bitboards[bishop_bits]; }
    bitboard_type knights() const { return the_bitboards[knight_bits]; }
    bitboard_type pawns() const { return the_bitboards[pawn_bits]; }

    std::optional<epd::board_contents_type> as_board_contents() const;

    logic_digits_type as_logic_digits() const;

    tb_position_type as_tb_position_type(
        bool white_to_move = true,
        std::uint16_t move = 1,
        std::uint8_t en_passant = 0,
        std::uint8_t castling = 0,
        std::uint8_t rule50 = 0) const;

    /*
     * Places a figurine on the chessboard.  False is returned iff a piece is
     * already present on the specified square.
     */
    [[nodiscard]]
    bool place(figurine_placement_type const placement);

    /*
     * Removes a figurine from the chessboard.  False is returned iff the piece
     * was not already present on the specified square.
     */
    [[nodiscard]]
    bool unplace(figurine_placement_type const placement);

    bitboard_type const & operator[](std::size_t index) {
        assert(index <= pawn_bits);
        return the_bitboards[index];
    }

private:
    std::array<bitboard_type, 8> the_bitboards;
};


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_BITBOARDS_TYPE_HPP
