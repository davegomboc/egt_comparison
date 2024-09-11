#include "representation/bitboards_type.hpp"


#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <locale>

#include "fathom.hpp"

#include "representation/figurine_colour_type.hpp"


namespace representation {


bitboards_type::bitboards_type(epd::board_contents_type board_contents) {
    for (unsigned char nth_bit{}; nth_bit != 64; ++nth_bit) {
        bitboard_type const just_coordinate_bit_set{coordinate_type{nth_bit}};
        char const current_char{board_contents[nth_bit]};
        if ('1' == current_char) continue;
        char const current_char_lowered{
         std::tolower(current_char, std::locale::classic())};
        size_t colour_bitboard_index{white_bits};
        if (current_char_lowered == current_char) {
            colour_bitboard_index = black_bits;
        }
        size_t shape_bitboard_index;
        switch (current_char_lowered) {
        case 'b':
            shape_bitboard_index = bishop_bits;
            break;
        case 'r':
            shape_bitboard_index = rook_bits;
            break;
        case 'q':
            shape_bitboard_index = queen_bits;
            break;
        case 'k':
            shape_bitboard_index = king_bits;
            break;
        case 'n':
            shape_bitboard_index = knight_bits;
            break;
        case 'p':
            shape_bitboard_index = pawn_bits;
            break;
        default:
            assert(false);
            std::exit(EXIT_FAILURE);
        }
        the_bitboards[colour_bitboard_index] |= just_coordinate_bit_set;
        the_bitboards[shape_bitboard_index] |= just_coordinate_bit_set;
    }
}


std::optional<epd::board_contents_type>
bitboards_type::as_board_contents() const {
    if (black() & white()) {
        return std::nullopt;
    }
    bitboard_type const colour_specified{black() | white()};
    bitboard_type const shapes_specified{
     bishops() | rooks() | queens() | kings() | knights() | pawns()};
    if (colour_specified != shapes_specified) {
        return std::nullopt;
    }
    std::string parseable_into_board_contents(64, '1');
    for (unsigned char nth_bit{}; nth_bit != 64; ++nth_bit) {
        bitboard_type const coordinate_bit{coordinate_type{nth_bit}};
        if (not (colour_specified & coordinate_bit)) {
            continue;
        }
        if (the_bitboards[pawn_bits] & coordinate_bit) {
            parseable_into_board_contents[nth_bit] = 'P';
        } else if (the_bitboards[knight_bits] & coordinate_bit) {
            parseable_into_board_contents[nth_bit] = 'N';
        } else if (the_bitboards[bishop_bits] & coordinate_bit) {
            parseable_into_board_contents[nth_bit] = 'B';
        } else if (the_bitboards[rook_bits] & coordinate_bit) {
            parseable_into_board_contents[nth_bit] = 'R';
        } else if (the_bitboards[queen_bits] & coordinate_bit) {
            parseable_into_board_contents[nth_bit] = 'Q';
        } else if (the_bitboards[king_bits] & coordinate_bit) {
            parseable_into_board_contents[nth_bit] = 'K';
        } else {
            assert(false);
        }
        if (black() & coordinate_bit) {
            parseable_into_board_contents[nth_bit] = std::tolower(
             parseable_into_board_contents[nth_bit], std::locale::classic());
        }
    }
    return epd::board_contents_type::create(parseable_into_board_contents);
}


// reserved*3 white*64 black*64 B*64 R*64 Q*64 K*64 N*64 P*64
// and each 64-bit pack indicates occupancy
// (TODO: Consider using 16 unused bits in P*64 instead of reserving more.)
logic_digits_type bitboards_type::as_logic_digits() const {
    logic_digits_type result;
    result.reserve(3 + 512);
    result.push_back(off);
    result.push_back(off);
    result.push_back(off);
    for (std::size_t nth_bitboard{}; nth_bitboard != 8; ++nth_bitboard) {
        logic_digits_type const current_logic_digits{
         the_bitboards[nth_bitboard].as_logic_digits()};
        result.append(current_logic_digits);
    }
    return result;
}


tb_position_type bitboards_type::as_tb_position_type(
    bool white_to_move,
    std::uint16_t full_move_count,
    std::uint8_t en_passant,
    std::uint8_t castling,
    std::uint8_t rule50
) const {
    tb_position_type result;
    result.white = white().as_syzygy_bitboard_value();
    result.black = black().as_syzygy_bitboard_value();
    result.kings = kings().as_syzygy_bitboard_value();
    result.queens = queens().as_syzygy_bitboard_value();
    result.rooks = rooks().as_syzygy_bitboard_value();
    result.bishops = bishops().as_syzygy_bitboard_value();
    result.knights = knights().as_syzygy_bitboard_value();
    result.pawns = pawns().as_syzygy_bitboard_value();
    result.castling = castling;
    result.rule50 = rule50;
    result.en_passant = en_passant;
    result.white_to_move = white_to_move;
    result.full_move_count = full_move_count;
    return result;
}


bool bitboards_type::place(figurine_placement_type placement) {
    bitboard_type const just_coordinate_bit_set{
     bitboard_type(placement.coordinate())};
    if ((the_bitboards[white_bits] | the_bitboards[black_bits])
     & just_coordinate_bit_set) {
        return false;
    }
    figurine_type const figurine{placement.figurine()};
    unsigned int const colour_bitboard_index{
     representation::white == figurine.colour() ? white_bits : black_bits};
    unsigned int const shape_bitboard_index{1u + figurine.shape().as_index()};
    the_bitboards[colour_bitboard_index] |= just_coordinate_bit_set;
    the_bitboards[shape_bitboard_index] |= just_coordinate_bit_set;
    return true;
}


bool bitboards_type::unplace(figurine_placement_type placement) {
    bitboard_type const coordinate_mask{~bitboard_type{placement.coordinate()}};
    figurine_type const figurine{placement.figurine()};
    unsigned int const colour_bitboard_index{
     representation::white == figurine.colour() ? white_bits : black_bits};
    unsigned int const shape_bitboard_index{1u + figurine.shape().as_index()};
    bitboard_type & colour_bitboard{the_bitboards[colour_bitboard_index]};
    bitboard_type & shape_bitboard{the_bitboards[shape_bitboard_index]};
    if ((colour_bitboard & coordinate_mask).none() or
     ((shape_bitboard & coordinate_mask).none())) {
        return false;
    }
    colour_bitboard &= coordinate_mask;
    shape_bitboard &= coordinate_mask;
    return true;
}


} // namespace representation
