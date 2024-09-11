#include "tb_position_type.hpp"


#include <bitset>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cstdlib>
#include <limits>
#include <ostream>

#include "tbprobe.h"


void replace_all(
    std::string & str,
    std::string_view const what,
    std::string_view const with
) {
    std::string::size_type position{};
    while (true) {
        position = str.find(what.data(), position, what.size());
        if (str.npos == position) break;
        str.replace(position, what.size(), with.data(), with.size());
        position += with.size();
    }
}

std::optional<tb_position_type> tb_position_type::create(
    epd::extended_position_description_type const & epd
) {
    std::optional<std::string> fen_opt{epd.as_fen_opt()};
    if (not fen_opt) {
        return std::nullopt;
    }
    return parse_fen(*fen_opt);
}


std::string tb_position_type::as_epd(
    std::optional<probe_report_type> const probe_report_opt,
    bool use_visual_style
) const {
    std::string result(fen_board_segment(use_visual_style));
    result += ' ';
    result += fen_side_to_move_segment();
    result += ' ';
    result += fen_castling_rights_segment();
    result += ' ';
    result += fen_en_passant_rights_segment();
    result += " hmvc ";
    result += fen_half_moves_since_progress_count_segment();
    result += "; fmvn ";
    result += fen_full_move_count_segment();
    if (probe_report_opt.has_value()) {
        result += "; GTV ";
        result += probe_report_opt->as_char();
        result += ";";
    }
    return result;
}

std::string tb_position_type::as_fen(bool const use_visual_style) const {
    std::string result(fen_board_segment(use_visual_style));
    result += ' ';
    result += fen_side_to_move_segment();
    result += ' ';
    result += fen_castling_rights_segment();
    result += ' ';
    result += fen_en_passant_rights_segment();
    result += ' ';
    result += fen_half_moves_since_progress_count_segment();
    result += ' ';
    result += fen_full_move_count_segment();
    return result;
}

std::string
tb_position_type::fen_board_segment(bool const use_visual_style) const {
    std::string result(64, '1');
    for (int rank{}; rank <= 7; ++rank) {
        for (int file{7}; file >= 0; --file) {
            std::uint64_t const single_set_bit{
             board_bit(static_cast<std::uint64_t>(rank * 8 + file))};
            char ch{'1'};
            if (single_set_bit & pawns) {
                ch = 'p';
            } else if (single_set_bit & kings) {
                ch = 'k';
            } else if (single_set_bit & queens) {
                ch = 'q';
            } else if (single_set_bit & rooks) {
                ch = 'r';
            } else if (single_set_bit & bishops) {
                ch = 'b';
            } else if (single_set_bit & knights) {
                ch = 'n';
            }
            if (single_set_bit & white) {
                ch = static_cast<char>(std::toupper(ch));
            }
            result[(7 - static_cast<unsigned>(rank)) * 8 +
             static_cast<unsigned>(file)] = ch;
        }
    }
    for (int rank{7}; rank != 0; --rank) {
        result.insert(static_cast<unsigned>(rank) * 8, 1, '/');
    }
    if (not use_visual_style) {
        // Replace all juxtaposed '1' characters (e.g., 11111111 -> 8, 11 -> 2).
        replace_all(result, "11111111", "8");
        replace_all(result, "1111111", "7");
        replace_all(result, "111111", "6");
        replace_all(result, "11111", "5");
        replace_all(result, "1111", "4");
        replace_all(result, "111", "3");
        replace_all(result, "11", "2");
    }
    return result;
}

std::string
tb_position_type::fen_side_to_move_segment() const {
    return white_to_move ? "w" : "b";
}

std::string
tb_position_type::fen_castling_rights_segment() const {
    std::string result;
    if (castling & TB_CASTLING_K) {
        result += 'K';
    }
    if (castling & TB_CASTLING_Q) {
        result += 'Q';
    }
    if (castling & TB_CASTLING_k) {
        result += 'k';
    }
    if (castling & TB_CASTLING_q) {
        result += 'q';
    }
    if (result.empty()) {
        result = "-";
    }
    return result;
}

std::string
tb_position_type::fen_en_passant_rights_segment() const {
    switch (en_passant) {
    case 0:
    default:
        return "-";
    case 16:
        return "a3";
    case 17:
        return "b3";
    case 18:
        return "c3";
    case 19:
        return "d3";
    case 20:
        return "e3";
    case 21:
        return "f3";
    case 22:
        return "g3";
    case 23:
        return "h3";
    case 40:
        return "a6";
    case 41:
        return "b6";
    case 42:
        return "c6";
    case 43:
        return "d6";
    case 44:
        return "e6";
    case 45:
        return "f6";
    case 46:
        return "g6";
    case 47:
        return "h6";
    }
}

std::string
tb_position_type::fen_half_moves_since_progress_count_segment() const {
    return std::to_string(static_cast<unsigned>(rule50));
}

std::string tb_position_type::fen_full_move_count_segment() const {
    return std::to_string(static_cast<unsigned>(full_move_count));
}

void tb_position_type::dump_2d_fields(std::ostream & out) const {
    out << "DD:   white = " << std::bitset<64>(white)
     << "\nDD:   black = " << std::bitset<64>(black)
     << "\nDD:   kings = " << std::bitset<64>(kings)
     << "\nDD:  queens = " << std::bitset<64>(queens)
     << "\nDD:   rooks = " << std::bitset<64>(rooks)
     << "\nDD: bishops = " << std::bitset<64>(bishops)
     << "\nDD: knights = " << std::bitset<64>(knights)
     << "\nDD:   pawns = " << std::bitset<64>(pawns)
     << "\nDD: castling = " << std::bitset<8>(castling)
     << "\nDD: rule50 = " << static_cast<int>(rule50)
     << "\nDD: en_passant = " << std::bitset<8>(en_passant)
     << "\nDD: white_to_move = " << static_cast<int>(white_to_move)
     << "\nDD: full_move_count = " << static_cast<int>(full_move_count) << '\n';
}

std::ostream & operator<<(
    std::ostream & out,
    tb_position_type const & tb_position
) {
    tb_position.dump_2d_fields(out);
    return out;
}

bool is_en_passant(
    tb_position_type const * pos,
    std::uint8_t from,
    std::uint8_t to)
{
    std::uint64_t us = (pos->white_to_move ? pos->white : pos->black);
    if (pos->en_passant == 0) return false;
    if (to != pos->en_passant) return false;
    if ((board_bit(from) & us & pos->pawns) == 0) return false;
    return true;
}

bool is_check(
    tb_position_type const * pos,
    bool test_if_our_king_is_in_check)
{
    std::uint64_t occupied = pos->white | pos->black;
    std::uint64_t ours = pos->white_to_move ? pos->white : pos->black;
    std::uint64_t theirs = pos->white_to_move ? pos->black : pos->white;
    std::uint64_t attackers = test_if_our_king_is_in_check ? theirs : ours;
    std::uint64_t defenders = test_if_our_king_is_in_check ? ours : theirs;
    std::uint64_t defending_king = pos->kings & defenders;
    unsigned int square_of_defending_king = tb_lsb(defending_king);
    std::uint64_t rook_attacks = tb_rook_attacks(square_of_defending_king, occupied);
    std::uint64_t bishop_attacks =
        tb_bishop_attacks(square_of_defending_king, occupied);
    std::uint64_t queen_attacks = rook_attacks | bishop_attacks;
    std::uint64_t knight_attacks = tb_knight_attacks(square_of_defending_king);
    std::uint64_t pawn_attacks =
        tb_pawn_attacks(square_of_defending_king, !pos->white_to_move);
    std::uint64_t king_attacks = tb_king_attacks(square_of_defending_king);

    if (queen_attacks & pos->queens & attackers) return true;
    if (rook_attacks & pos->rooks & attackers) return true;
    if (bishop_attacks & pos->bishops & attackers) return true;
    if (knight_attacks & pos->knights & attackers) return true;
    if (pawn_attacks & pos->pawns & attackers) return true;
    if (king_attacks & pos->kings & attackers) return true;
    return false;
}

std::optional<std::vector<std::string_view>> segment_fen(std::string_view fen) {
    std::string_view const whitespace{" \f\n\r\t\v"};
    std::vector<std::string_view> fen_segments;
    std::string_view::size_type current_offset{};
    for (int nth_field{}; nth_field < 6; ++nth_field) {
        std::string_view::size_type segment_beginning{
         fen.find_first_not_of(whitespace, current_offset)};
        if (std::string_view::npos == segment_beginning) {
            return std::nullopt;
        }
        std::string_view::size_type segment_end{
         fen.find_first_of(whitespace, current_offset + 1)};
        if (5 == nth_field) {
            if (std::string_view::npos == segment_end) {
                segment_end = fen.size();
            }
        } else if (std::string_view::npos == segment_end) {
            return std::nullopt;
        }
        std::string_view segment(fen);
        segment.remove_prefix(segment_beginning);
        segment.remove_suffix(fen.size() - segment_end);
        fen_segments.push_back(segment);
        current_offset = segment_end + 1;
    }
    return fen_segments;
}

bool parse_fen_board(
    tb_position_type & tb_position,
    std::string_view fen_board_segment
) {
    tb_position.white = 0;
    tb_position.black = 0;
    tb_position.kings = 0;
    tb_position.queens = 0;
    tb_position.rooks = 0;
    tb_position.bishops = 0;
    tb_position.knights = 0;
    tb_position.pawns = 0;
    for (int rank{7}; rank >= 0; --rank) {
        for (int file{}; file <= 7; ++file) {
            if (fen_board_segment.empty()) {
                return false;
            }
            char const ch{fen_board_segment[0]};
            fen_board_segment.remove_prefix(1);
            std::uint64_t const single_set_bit{
             board_bit(static_cast<std::uint64_t>(rank * 8 + file))};
            switch(ch) {
            case '8':
                ++file;
                [[fallthrough]];
            case '7':
                ++file;
                [[fallthrough]];
            case '6':
                ++file;
                [[fallthrough]];
            case '5':
                ++file;
                [[fallthrough]];
            case '4':
                ++file;
                [[fallthrough]];
            case '3':
                ++file;
                [[fallthrough]];
            case '2':
                ++file;
                [[fallthrough]];
            case '1':
                continue;
            case '/':
                --file;
                continue;
            case 'k':
                tb_position.kings |= single_set_bit;
                tb_position.black |= single_set_bit;
                continue;
            case 'K':
                tb_position.kings |= single_set_bit;
                tb_position.white |= single_set_bit;
                continue;
            case 'q':
                tb_position.queens |= single_set_bit;
                tb_position.black |= single_set_bit;
                continue;
            case 'Q':
                tb_position.queens |= single_set_bit;
                tb_position.white |= single_set_bit;
                continue;
            case 'r':
                tb_position.rooks |= single_set_bit;
                tb_position.black |= single_set_bit;
                continue;
            case 'R':
                tb_position.rooks |= single_set_bit;
                tb_position.white |= single_set_bit;
                continue;
            case 'b':
                tb_position.bishops |= single_set_bit;
                tb_position.black |= single_set_bit;
                continue;
            case 'B':
                tb_position.bishops |= single_set_bit;
                tb_position.white |= single_set_bit;
                continue;
            case 'n':
                tb_position.knights |= single_set_bit;
                tb_position.black |= single_set_bit;
                continue;
            case 'N':
                tb_position.knights |= single_set_bit;
                tb_position.white |= single_set_bit;
                continue;
            case 'p':
                tb_position.pawns |= single_set_bit;
                tb_position.black |= single_set_bit;
                continue;
            case 'P':
                tb_position.pawns |= single_set_bit;
                tb_position.white |= single_set_bit;
                continue;
            default:
                return false;
            }
        }
    }
    return true;
}

bool parse_fen_side_to_move(
    tb_position_type & tb_position,
    std::string_view fen_side_to_move_segment
) {
    if ("w" == fen_side_to_move_segment) {
        tb_position.white_to_move = true;
        return true;
    }
    if ("b" == fen_side_to_move_segment) {
        tb_position.white_to_move = false;
        return true;
    }
    return false;
}

bool parse_fen_castling_rights(
    tb_position_type & tb_position,
    std::string_view fen_castling_rights_segment
) {
    if (1 == fen_castling_rights_segment.size()) {
        if ("-" == fen_castling_rights_segment) {
            tb_position.castling = 0;
            return true;
        }
    }
    if ((fen_castling_rights_segment.size() < 2) or
     (4 < fen_castling_rights_segment.size())) {
        return false;
    }
    if (std::string_view::npos !=
     fen_castling_rights_segment.find_first_not_of("KQkq")) {
        return false;
    }
    tb_position.castling = 0;
    for (char const castling_right: fen_castling_rights_segment) {
        switch (castling_right) {
        case 'K':
            tb_position.castling |= TB_CASTLING_K;
            break;
        case 'Q':
            tb_position.castling |= TB_CASTLING_Q;
            break;
        case 'k':
            tb_position.castling |= TB_CASTLING_k;
            break;
        case 'q':
            tb_position.castling |= TB_CASTLING_q;
            break;
        default:
            __builtin_unreachable();
        }
    }
    return true;
}

// Returns true upon success.
// Does not consider the board contents.
bool parse_fen_en_passant_rights(
    tb_position_type & tb_position,
    std::string_view fen_en_passant_rights_segment
) {
    if (1 == fen_en_passant_rights_segment.size()) {
        if ('-' == fen_en_passant_rights_segment[0]) {
            tb_position.en_passant = 0;
            return true;
        }
        return false;
    }
    if (2 != fen_en_passant_rights_segment.size()) {
        return false;
    }
    char const file_as_char{fen_en_passant_rights_segment[0]};
    if ((file_as_char < 'a') or ('h' < file_as_char)) {
        return false;
    }
    unsigned char const file{static_cast<unsigned char>(file_as_char - 'a')};

    char const rank_as_char{fen_en_passant_rights_segment[1]};
    if (('3' != rank_as_char) and ('6' != rank_as_char)) {
        return false;
    }
    unsigned char const rank{static_cast<unsigned char>(rank_as_char - '1')};
    tb_position.en_passant = square(rank, file);
    return true;
}

// Returns true upon success.
bool parse_fen_half_moves_since_progress_count(
    tb_position_type & tb_position,
    std::string_view fen_half_moves_since_progress_count_segment
) {
    int half_move_count_as_int;
    std::from_chars_result const from_chars_result{
      std::from_chars(fen_half_moves_since_progress_count_segment.cbegin(),
      fen_half_moves_since_progress_count_segment.cend(),
      half_move_count_as_int)};
    if (std::errc() != from_chars_result.ec) {
        return false;
    }
    if (half_move_count_as_int < 0) {
        return false;
    }
    unsigned const half_move_count{
     static_cast<unsigned>(half_move_count_as_int)};
    using target_type = decltype(tb_position.rule50);
    if (std::numeric_limits<target_type>::max() < half_move_count) {
        return false;
    }
    tb_position.rule50 = static_cast<target_type>(half_move_count);
    return true;
}

// Returns true upon success.
bool parse_fen_full_move_count(
    tb_position_type & tb_position,
    std::string_view fen_full_move_count_segment
) {
    int full_move_count_as_int;
    std::from_chars_result const from_chars_result{std::from_chars(
     fen_full_move_count_segment.cbegin(), fen_full_move_count_segment.cend(),
     full_move_count_as_int)};
    if (std::errc() != from_chars_result.ec) {
        return false;
    }
    if (full_move_count_as_int < 1) {
        return false;
    }
    unsigned const full_move_count{
     static_cast<unsigned>(full_move_count_as_int)};
    using target_type = decltype(tb_position.full_move_count);
    if (std::numeric_limits<target_type>::max() < full_move_count) {
        return false;
    }
    tb_position.full_move_count = static_cast<target_type>(full_move_count);
    return true;
}

std::optional<tb_position_type>
parse_fen(std::optional<std::vector<std::string_view>> fen_segments_opt) {
    if (std::nullopt == fen_segments_opt) {
        return std::nullopt;
    }
    tb_position_type tb_position;
    if (not parse_fen_board(tb_position, (*fen_segments_opt)[0])) {
        return std::nullopt;
    }
    if (not parse_fen_side_to_move(tb_position, (*fen_segments_opt)[1])) {
        return std::nullopt;
    }
    if (not parse_fen_castling_rights(tb_position, (*fen_segments_opt)[2])) {
        return std::nullopt;
    }
    if (not parse_fen_en_passant_rights(tb_position, (*fen_segments_opt)[3])) {
        return std::nullopt;
    }
    if (not parse_fen_half_moves_since_progress_count(tb_position,
     (*fen_segments_opt)[4])) {
        return std::nullopt;
    }
    if (not parse_fen_full_move_count(tb_position, (*fen_segments_opt)[5])) {
        return std::nullopt;
    }

    // TODO: Check castling validity.
    // TODO: Check en passant validity.
    return tb_position;
}

/*
 * Parse a position specified in Forsyth-Edwards Notation.
 */
std::optional<tb_position_type> parse_fen(std::string_view fen) {
    std::optional<std::vector<std::string_view>> fen_segments_opt{
     segment_fen(fen)};
    return parse_fen(fen_segments_opt);
}

void move_parts_to_str(
    tb_position_type const * pos,
    int from_as_int,
    int to_as_int,
    int promotes,
    char * str)
{
    assert(0 <= from_as_int);
    assert(from_as_int <= std::numeric_limits<std::uint8_t>::max());
    std::uint8_t const from{static_cast<std::uint8_t>(from_as_int)};
    assert(0 <= to_as_int);
    assert(to_as_int <= std::numeric_limits<std::uint8_t>::max());
    std::uint8_t const to{static_cast<std::uint8_t>(to_as_int)};
    unsigned int r = rank(from);
    unsigned int f = file(from);
    std::uint64_t occ{pos->black | pos->white};
    std::uint64_t us{pos->white_to_move ? pos->white : pos->black};
    bool capture{(occ & board_bit(to)) != 0 || is_en_passant(pos, from, to)};
    std::uint64_t b{board_bit(from)};
    std::uint64_t att{};

    if (b & pos->kings)
    {
        *str++ = 'K';
    }
    else if (b & pos->queens)
    {
        *str++ = 'Q';
        att = tb_queen_attacks(to, occ) & us & pos->queens;
    }
    else if (b & pos->rooks)
    {
        *str++ = 'R';
        att = tb_rook_attacks(to, occ) & us & pos->rooks;
    }
    else if (b & pos->bishops)
    {
        *str++ = 'B';
        att = tb_bishop_attacks(to, occ) & us & pos->bishops;
    }
    else if (b & pos->knights)
    {
        *str++ = 'N';
        att = tb_knight_attacks(to) & us & pos->knights;
    }
    else
    {
        att = tb_pawn_attacks(to, !pos->white_to_move) & us & pos->pawns;
    }
    if ((b & pos->pawns) && capture)
    {
        *str++ = static_cast<char>('a' + f);
    }
    else if (tb_pop_count(att) > 1)
    {
        if (tb_pop_count(att & (board_file_a >> f)) == 1)
        {
            *str++ = static_cast<char>('a' + f);
        }
        else if (tb_pop_count(att & (board_rank_1 << (8 * r))) == 1)
        {
            *str++ = static_cast<char>('1' + r);
        }
        else
        {
            *str++ = static_cast<char>('a' + f);
            *str++ = static_cast<char>('1' + r);
        }
    }
    if (capture) *str++ = 'x';
    *str++ = static_cast<char>('a' + file(to));
    *str++ = static_cast<char>('1' + rank(to));
    if (promotes != TB_PROMOTES_NONE)
    {
        *str++ = '=';
        switch (promotes)
        {
            case TB_PROMOTES_QUEEN:
                *str++ = 'Q';
                break;
            case TB_PROMOTES_ROOK:
                *str++ = 'R';
                break;
            case TB_PROMOTES_BISHOP:
                *str++ = 'B';
                break;
            case TB_PROMOTES_KNIGHT:
                *str++ = 'N';
                break;
            default:
                __builtin_unreachable();
        }
    }
    *str++ = '\0';
}

void move_to_str(
    tb_position_type const * pos,
    unsigned int move,
    char * str)
{
    move_parts_to_str(pos, static_cast<int>(TB_GET_FROM(move)),
     static_cast<int>(TB_GET_TO(move)), static_cast<int>(TB_GET_PROMOTES(move)),
     str);
}

/*
 * Do a move.  Does not support castling.
 */
#define do_bb_move(b, from, to)                       \
    (((b) & (~board_bit(to)) & (~board_bit(from))) |  \
        ((((b) >> (from)) & 0x1) << (to)))
void do_move(tb_position_type * pos, unsigned int move)
{
    unsigned int from     = TB_GET_FROM(move);
    unsigned int to       = TB_GET_TO(move);
    unsigned int promotes = TB_GET_PROMOTES(move);
    bool white_to_move = !pos->white_to_move;
    std::uint64_t white   = do_bb_move(pos->white, from, to);
    std::uint64_t black   = do_bb_move(pos->black, from, to);
    std::uint64_t kings   = do_bb_move(pos->kings, from, to);
    std::uint64_t queens  = do_bb_move(pos->queens, from, to);
    std::uint64_t rooks   = do_bb_move(pos->rooks, from, to);
    std::uint64_t bishops = do_bb_move(pos->bishops, from, to);
    std::uint64_t knights = do_bb_move(pos->knights, from, to);
    std::uint64_t pawns   = do_bb_move(pos->pawns, from, to);
    std::uint8_t en_passant{};
    std::uint8_t rule50{pos->rule50};
    if (promotes != TB_PROMOTES_NONE)
    {
        pawns &= ~board_bit(to);
        switch (promotes)
        {
            case TB_PROMOTES_QUEEN:
                queens |= board_bit(to);
                break;
            case TB_PROMOTES_ROOK:
                rooks |= board_bit(to);
                break;
            case TB_PROMOTES_BISHOP:
                bishops |= board_bit(to);
                break;
            case TB_PROMOTES_KNIGHT:
                knights |= board_bit(to);
                break;
            default:
                __builtin_unreachable();
        }
        rule50 = 0;
    }
    else if ((board_bit(from) & pos->pawns) != 0)
    {
        rule50 = 0;
        if (rank(from) == 1 && rank(to) == 3 &&
            (tb_pawn_attacks(from + 8, true) & pos->pawns & pos->black) != 0)
        {
            en_passant = static_cast<std::uint8_t>(from + 8);
        }
        else if (rank(from) == 6 && rank(to) == 4 &&
            (tb_pawn_attacks(from - 8, false) & pos->pawns & pos->white) != 0)
        {
            en_passant = static_cast<std::uint8_t>(from - 8);
        }
        else if (TB_GET_EP(move))
        {
            unsigned int en_passant_to = (pos->white_to_move ? to - 8 : to + 8);
            std::uint64_t en_passant_mask = ~board_bit(en_passant_to);
            white &= en_passant_mask;
            black &= en_passant_mask;
            pawns &= en_passant_mask;
        }
    }
    else if ((board_bit(to) & (pos->white | pos->black)) != 0)
    {
        rule50 = 0;
    }
    else
    {
        ++rule50;
    }
    pos->white   = white;
    pos->black   = black;
    pos->kings   = kings;
    pos->queens  = queens;
    pos->rooks   = rooks;
    pos->bishops = bishops;
    pos->knights = knights;
    pos->pawns   = pawns;
    pos->en_passant = en_passant;
    pos->rule50  = rule50;
    pos->white_to_move = white_to_move;
    pos->full_move_count += white_to_move;
}
