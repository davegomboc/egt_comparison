#include "probe_timing_epd_parsing.hpp"


#include <sstream>


std::vector<std::string> segment_epd(std::string const & current_epd_line) {
    std::istringstream in(current_epd_line);
    std::vector<std::string> result;
    std::string epd_fen_board_segment;
    std::getline(in, epd_fen_board_segment, ' ');
    result.push_back(epd_fen_board_segment);
    std::string epd_fen_side_to_move_segment;
    std::getline(in, epd_fen_side_to_move_segment, ' ');
    result.push_back(epd_fen_side_to_move_segment);
    std::string epd_fen_castling_rights_segment;
    std::getline(in, epd_fen_castling_rights_segment, ' ');
    result.push_back(epd_fen_castling_rights_segment);
    std::string epd_fen_en_passant_rights_segment;
    std::getline(in, epd_fen_en_passant_rights_segment, ' ');
    result.push_back(epd_fen_en_passant_rights_segment);
    std::string discard;
    std::getline(in, discard, ' ');
    assert("hmvc" == discard);
    std::string epd_half_moves_since_progress_count_segment;
    std::getline(in, epd_half_moves_since_progress_count_segment, ';');
    result.push_back(epd_half_moves_since_progress_count_segment);
    in.get();
    std::getline(in, discard, ' ');
    assert("fmvn" == discard);
    std::string epd_full_move_count_segment;
    std::getline(in, epd_full_move_count_segment, ';');
    result.push_back(epd_full_move_count_segment);
    in.get();
    std::getline(in, discard, ' ');
    assert("GTV" == discard);
    std::string epd_game_theoretic_value_segment;
    std::getline(in, epd_game_theoretic_value_segment, ';');
    result.push_back(epd_game_theoretic_value_segment);
    assert(epd_segment_limit == result.size());
    return result;
}


std::optional<probe_report_type> probe_report_opt_from(
    std::vector<std::string> const & epd_segments
) {
    assert(epd_segment_limit == epd_segments.size());
    return to_probe_report_type_opt(epd_segments[
     epd_game_theoretic_value_segment_index]);
}
