#if !defined(EGT_COMPARISON_CHESSPRESSO_PROBE_TIMING_EPD_PARSING_HPP)
#define EGT_COMPARISON_CHESSPRESSO_PROBE_TIMING_EPD_PARSING_HPP


#include <optional>
#include <string>
#include <vector>

#include "probe_timing_epd_parsing.hpp"

#include "representation/board_8x8_type.hpp"
#include "representation/egt_position_type.hpp"


template <>
inline std::optional<representation::egt_position_type> position_opt_from(
    std::vector<std::string> const & epd_segments
) {
    assert(epd_segment_limit == epd_segments.size());
    std::optional<epd::board_contents_type> const board_contents_opt{
     epd::board_contents_type::create(
     epd_segments[epd_fen_board_segment_index])};
    if (not board_contents_opt) {
        return std::nullopt;
    }
    std::string const & side_to_move_segment{
     epd_segments[epd_fen_side_to_move_segment_index]};
    if (("b" != side_to_move_segment) and ("w" != side_to_move_segment)) {
        return std::nullopt;
    }
    // TODO: Support castling.
    if ("-" != epd_segments[epd_fen_castling_rights_segment_index]) {
        return std::nullopt;
    }
    std::string_view const en_passant_target_view{epd_segments[
     epd_fen_en_passant_rights_segment_index]};
    std::optional<representation::coordinate_type> en_passant_target_opt;
    if ("-" == en_passant_target_view) {
        en_passant_target_opt = representation::no_ep;
    } else {
        en_passant_target_opt = representation::coordinate_type::create_opt(
         en_passant_target_view);
        if (std::nullopt == en_passant_target_opt) {
            return std::nullopt;
        }
    }

    representation::board_8x8_type const board(*board_contents_opt);
    representation::figurine_colour_type const side_to_move{"b"
     == side_to_move_segment ? representation::black : representation::white};
    return representation::egt_position_type(side_to_move,
     board.as_placements(), *en_passant_target_opt);
}


#endif // EGT_COMPARISON_CHESSPRESSO_PROBE_TIMING_EPD_PARSING_HPP
