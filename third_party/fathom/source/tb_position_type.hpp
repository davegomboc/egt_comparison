#if !defined(TB_POSITION_TYPE_HPP)
#define TB_POSITION_TYPE_HPP


#include <cstdint>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "epd/extended_position_description_type.hpp"

#include "probe_report_type.hpp"


struct tb_position_type {
    std::uint64_t white;
    std::uint64_t black;
    std::uint64_t kings;
    std::uint64_t queens;
    std::uint64_t rooks;
    std::uint64_t bishops;
    std::uint64_t knights;
    std::uint64_t pawns;
    std::uint8_t castling;
    std::uint8_t rule50;
    std::uint8_t en_passant;
    bool white_to_move;
    std::uint16_t full_move_count;

    static std::optional<tb_position_type> create(
        epd::extended_position_description_type const & epd);

    std::string as_epd(
        std::optional<probe_report_type> probe_report_opt,
        bool use_visual_style = false) const;
    std::string as_fen(bool use_visual_style = false) const;
    std::string fen_board_segment(bool use_visual_style = false) const;
    std::string fen_side_to_move_segment() const;
    std::string fen_castling_rights_segment() const;
    std::string fen_en_passant_rights_segment() const;
    std::string fen_half_moves_since_progress_count_segment() const;
    std::string fen_full_move_count_segment() const;

    void dump_2d_fields(std::ostream & out) const;
};

std::ostream & operator<<(
    std::ostream & out,
    tb_position_type const & tb_position);


static constexpr std::uint64_t board_rank_1{UINT64_C(0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'1111)};
static constexpr std::uint64_t board_rank_8{UINT64_C(0b1111'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000)};
static constexpr std::uint64_t board_file_a{UINT64_C(0b1000'0000'1000'0000'1000'0000'1000'0000'1000'0000'1000'0000'1000'0000'1000'0000)};

static constexpr std::uint64_t alternating_nybbles  {UINT64_C(0b0000'1111'0000'1111'0000'1111'0000'1111'0000'1111'0000'1111'0000'1111'0000'1111)};
static constexpr std::uint64_t alternating_bit_pairs{UINT64_C(0b0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011)};
static constexpr std::uint64_t alternating_bits     {UINT64_C(0b0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101)};


template <typename T>
constexpr std::uint8_t square(T const rank, T const file) {
    return static_cast<std::uint8_t>(8 * rank + file);
}

template <typename T>
constexpr std::uint8_t rank(T const square) {
    return static_cast<std::uint8_t>(square >> 3);
}

template <typename T>
constexpr std::uint8_t file(T const square) {
    return static_cast<std::uint8_t>(square & 0x07);
}

template <typename T>
constexpr std::uint64_t board_bit(T const square) {
    return static_cast<std::uint64_t>(UINT64_C(1) << square);
}

constexpr bool exactly_one_bit_is_set(std::uint64_t value) {
    if (0 == value) return false;
    value &= (value - 1);
    return 0 == value;
}

/*
 * Interchanges file pairs: a and h; b and g; c and f; d and e.
 */
constexpr std::uint64_t flip_files(std::uint64_t bitboard) {
    // Swap within each bit pair.
    bitboard = ((bitboard >> 1) & alternating_bits) |
     ((bitboard & alternating_bits) << 1);
    // Swap within each nybble.
    bitboard = ((bitboard >> 2) & alternating_bit_pairs) |
     ((bitboard & alternating_bit_pairs) << 2);
    // Swap within each byte.
    bitboard = ((bitboard >> 4) & alternating_nybbles) |
     ((bitboard & alternating_nybbles) << 4);
    return bitboard;
}

constexpr bool each_side_has_exactly_one_king(
    tb_position_type const * const pos
) {
    return exactly_one_bit_is_set(pos->kings & pos->white)
     and exactly_one_bit_is_set(pos->kings & pos->black);
}

constexpr bool pawn_on_first_or_last_rank(tb_position_type const * const pos) {
    return 0 != (pos->pawns & (board_rank_1 | board_rank_8));
}

/*
 * Test if the given move is an en passant capture.
 */
bool is_en_passant(
    tb_position_type const * pos,
    std::uint8_t from,
    std::uint8_t to);

/*
 * Test to see if a particular side's king is in check.
 *
 * More complete support for in-check testing has been added so that Fathom can
 * detect illegal positions (e.g., kings being next to each other) in the
 * complete absence of a chess engine.
 */
bool is_check(
    tb_position_type const * pos,
    bool test_if_our_king_is_in_check);

/*
 * Parse a position specified in Forsyth-Edwards Notation.
 */
std::optional<tb_position_type> parse_fen(std::string_view fen);

std::optional<std::vector<std::string_view>> segment_fen(std::string_view fen);

bool parse_fen_board(
    tb_position_type & tb_position,
    std::string_view fen_board_segment);

bool parse_fen_side_to_move(
    tb_position_type & tb_position,
    std::string_view fen_side_to_move_segment);

bool parse_fen_castling_rights(
    tb_position_type & tb_position,
    std::string_view fen_castling_rights_segment);

bool parse_fen_en_passant_rights(
    tb_position_type & tb_position,
    std::string_view fen_en_passant_rights_segment);

bool parse_fen_half_moves_since_progress_count(
    tb_position_type & tb_position,
    std::string_view fen_half_moves_since_progress_count_segment);

bool parse_fen_full_move_count(
    tb_position_type & tb_position,
    std::string_view fen_full_move_count_segment);

/*
 * Convert a move into a string.
 */
void move_parts_to_str(
    tb_position_type const * pos,
    int from_as_int,
    int to_as_int,
    int promotes,
    char * str);

void move_to_str(tb_position_type const * pos, unsigned int move, char * str);

/*
 * Do a move.  Does not support castling.
 */
void do_move(tb_position_type * pos, unsigned int move);


#endif // TB_POSITION_TYPE_HPP
