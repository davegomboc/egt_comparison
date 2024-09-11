#include "syzygy_probe_timing_test.hpp"


#include "generic_probe_timing.hpp"


TEST(SyzygyProbeTimingTest, DetermineTablebasePositions) {
    std::istringstream in{
R"(1k6/8/8/1b6/8/5K2/8/4B3 b - - hmvc 0; fmvn 1; GTV d;
8/7k/8/2Q4K/8/7N/8/8 b - - hmvc 0; fmvn 1; GTV l;
8/2K1k2P/8/8/8/5n2/8/8 w - - hmvc 0; fmvn 1; GTV w;
8/8/B7/5K2/5q2/8/1k6/8 w - - hmvc 0; fmvn 1; GTV d;
2n5/8/k7/8/p1K5/8/8/8 w - - hmvc 0; fmvn 1; GTV l;
1K6/1P6/8/8/7q/8/8/6k1 b - - hmvc 0; fmvn 1; GTV w;
4KB2/8/8/7B/8/8/k7/8 b - - hmvc 0; fmvn 1; GTV l;
1n6/8/8/7k/8/8/7N/3K4 b - - hmvc 0; fmvn 1; GTV d;
8/k7/8/3p4/8/8/8/N5K1 b - - hmvc 0; fmvn 1; GTV d;
8/8/8/8/6B1/8/8/K4N1k w - - hmvc 0; fmvn 1; GTV w;
)"};
    std::vector<epd::extended_position_description_type> epds;
    std::optional<std::string> const bad_epd_line_opt{
     ingest_lines_of_epd_strings(in, epds)};
    ASSERT_FALSE(bad_epd_line_opt);
    ASSERT_EQ(10, epds.size());
    std::vector<tb_position_type> positions;
    std::optional<epd::extended_position_description_type> epd_missing_info_opt{
     determine_tb_positions(epds, positions)};
    EXPECT_FALSE(epd_missing_info_opt);
    EXPECT_EQ(10, positions.size());
}


/*
TEST(SyzygyProbeTimingTest, VetProbeReports) {
    std::istringstream in{
R"(1k6/8/8/1b6/8/5K2/8/4B3 b - - hmvc 0; fmvn 1; GTV d;
8/7k/8/2Q4K/8/7N/8/8 b - - hmvc 0; fmvn 1; GTV l;
8/2K1k2P/8/8/8/5n2/8/8 w - - hmvc 0; fmvn 1; GTV w;
8/8/B7/5K2/5q2/8/1k6/8 w - - hmvc 0; fmvn 1; GTV d;
2n5/8/k7/8/p1K5/8/8/8 w - - hmvc 0; fmvn 1; GTV l;
1K6/1P6/8/8/7q/8/8/6k1 b - - hmvc 0; fmvn 1; GTV w;
4KB2/8/8/7B/8/8/k7/8 b - - hmvc 0; fmvn 1; GTV l;
1n6/8/8/7k/8/8/7N/3K4 b - - hmvc 0; fmvn 1; GTV d;
8/k7/8/3p4/8/8/8/N5K1 b - - hmvc 0; fmvn 1; GTV d;
8/8/8/8/6B1/8/8/K4N1k w - - hmvc 0; fmvn 1; GTV w;
)"};
    std::vector<epd::extended_position_description_type> epds;
    std::optional<std::string> const bad_epd_line_opt{
     ingest_lines_of_epd_strings(in, epds)};
    ASSERT_FALSE(bad_epd_line_opt);
    ASSERT_EQ(10, epds.size());
    std::vector<probe_report_type> expected_probe_reports;
    std::optional<epd::extended_position_description_type> epd_without_GTV_opt{
     determine_expected_probe_reports(epds, expected_probe_reports)};
    ASSERT_FALSE(epd_without_GTV_opt);
    ASSERT_EQ(10, expected_probe_reports.size());
    ASSERT_EQ((std::vector<probe_report_type>{draw, loss, win, draw, loss, win,
     loss, draw, draw, win}), expected_probe_reports);
    std::vector<tb_position_type> positions;
    std::optional<epd::extended_position_description_type> epd_missing_info_opt{
     determine_tb_positions(epds, positions)};
    ASSERT_FALSE(epd_missing_info_opt);
    ASSERT_EQ(expected_probe_reports.size(), positions.size());

    std::vector<probe_report_type> probe_reports(expected_probe_reports.size());
    std::vector<std::chrono::steady_clock::duration> vetting_probe_durations(expected_probe_reports.size());
    bool const mismatch_found{vet_syzygy_probe_reports(positions, epds,
     expected_probe_reports, probe_reports, vetting_probe_durations)};
    EXPECT_FALSE(mismatch_found);
}
*/
