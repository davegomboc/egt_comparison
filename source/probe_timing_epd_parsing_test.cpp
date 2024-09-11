#include "probe_timing_epd_parsing_test.hpp"


TEST(ProbeTimingEpdParsingTest, SegmentEpdForKh5Qc5Nh2kg7btm) {
    std::string const epd{"8/7k/8/2Q4K/8/7N/8/8 b - - hmvc 0; fmvn 1; GTV l;"};
    std::vector<std::string> epd_segments(segment_epd(epd));
    EXPECT_EQ(epd_segment_limit, epd_segments.size());
    EXPECT_EQ("8/7k/8/2Q4K/8/7N/8/8",
     epd_segments[epd_fen_board_segment_index]);
    EXPECT_EQ("b", epd_segments[epd_fen_side_to_move_segment_index]);
    EXPECT_EQ("-", epd_segments[epd_fen_castling_rights_segment_index]);
    EXPECT_EQ("-", epd_segments[epd_fen_en_passant_rights_segment_index]);
    EXPECT_EQ("0", epd_segments[
     epd_half_moves_since_progress_count_segment_index]);
    EXPECT_EQ("1", epd_segments[epd_full_move_count_segment_index]);
    EXPECT_EQ("l", epd_segments[epd_game_theoretic_value_segment_index]);
}


TEST(ProbeTimingEpdParsingTest, ProbeReportFromEpdSegments) {
    probe_report_type const expected{loss};
    std::vector<std::string> epd_segments{
     "8/7k/8/2Q4K/8/7N/8/8", "b", "-", "-", "0", "1", "l"};
    std::optional<probe_report_type> const probe_report_opt{
     probe_report_opt_from(epd_segments)};
    ASSERT_TRUE(probe_report_opt);
    EXPECT_EQ(expected, *probe_report_opt);
}
