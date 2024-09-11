#if !defined(EGT_COMPARISON_PROBE_TIMING_EPD_PARSING_HPP)
#define EGT_COMPARISON_PROBE_TIMING_EPD_PARSING_HPP


#include <optional>
#include <string>
#include <vector>

#include "probe_report_type.hpp"
#include "tb_position_type.hpp"


static constexpr unsigned int epd_fen_board_segment_index{};
static constexpr unsigned int epd_fen_side_to_move_segment_index{1};
static constexpr unsigned int epd_fen_castling_rights_segment_index{2};
static constexpr unsigned int epd_fen_en_passant_rights_segment_index{3};
static constexpr unsigned int epd_half_moves_since_progress_count_segment_index{
 4};
static constexpr unsigned int epd_full_move_count_segment_index{5};
static constexpr unsigned int epd_game_theoretic_value_segment_index{6};
static constexpr unsigned int epd_segment_limit{7};


std::vector<std::string> segment_epd(std::string const & current_epd_line);


std::optional<probe_report_type> probe_report_opt_from(
    std::vector<std::string> const & epd_segments);


template <typename position_type>
std::optional<position_type> position_opt_from(
    std::vector<std::string> const & epd_segments);


template <>
inline std::optional<tb_position_type> position_opt_from(
    std::vector<std::string> const & epd_segments
) {
    assert(epd_segment_limit == epd_segments.size());
    tb_position_type tb_position;
    if (not parse_fen_board(tb_position, epd_segments[
     epd_fen_board_segment_index])) {
        return std::nullopt;
    }
    if (not parse_fen_side_to_move(tb_position, epd_segments[
     epd_fen_side_to_move_segment_index])) {
        return std::nullopt;
    }
    if (not parse_fen_castling_rights(tb_position, epd_segments[
     epd_fen_castling_rights_segment_index])) {
        return std::nullopt;
    }
    if (not parse_fen_en_passant_rights(tb_position, epd_segments[
     epd_fen_en_passant_rights_segment_index])) {
        return std::nullopt;
    }
    if (not parse_fen_half_moves_since_progress_count(tb_position, epd_segments[
     epd_half_moves_since_progress_count_segment_index])) {
        return std::nullopt;
    }
    if (not parse_fen_full_move_count(tb_position, epd_segments[
     epd_full_move_count_segment_index])) {
        return std::nullopt;
    }
    return tb_position;
}


#endif // EGT_COMPARISON_PROBE_TIMING_EPD_PARSING_HPP
