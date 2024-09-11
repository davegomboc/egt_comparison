#include "generic_probe_timing_test.hpp"


#include <filesystem>
#include <fstream>

#include "epd/extended_position_description_type.hpp"
#include "unxz.hpp"


TEST(GenericProbeTimingTest, IngestBadEpdString) {
    std::istringstream in{"junk\n"};
    std::vector<epd::extended_position_description_type> epds;
    std::optional<std::string> const bad_epd_line_opt{
     ingest_lines_of_epd_strings(in, epds)};
    EXPECT_TRUE(bad_epd_line_opt);
    EXPECT_EQ("junk", *bad_epd_line_opt);
    EXPECT_TRUE(epds.empty());
}


TEST(GenericProbeTimingTest, IngestLinesOfTypicalEpdStrings) {
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
    EXPECT_FALSE(bad_epd_line_opt);
    EXPECT_EQ(10, epds.size());
}


TEST(GenericProbeTimingTest, DetermineExpectedProbeReportsFromPerspectiveOfSideToMove) {
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
     determine_expected_probe_reports(epds, expected_probe_reports, false)};
    EXPECT_FALSE(epd_without_GTV_opt);
    EXPECT_EQ(10, expected_probe_reports.size());
    EXPECT_EQ((std::vector<probe_report_type>{draw, loss, win, draw, loss, win,
     loss, draw, draw, win}), expected_probe_reports);
}


TEST(GenericProbeTimingTest, DetermineExpectedProbeReportsFromPerspectiveOfWhite) {
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
     determine_expected_probe_reports(epds, expected_probe_reports, true)};
    EXPECT_FALSE(epd_without_GTV_opt);
    EXPECT_EQ(10, expected_probe_reports.size());
    EXPECT_EQ((std::vector<probe_report_type>{draw, win, win, draw, loss, loss,
     win, draw, draw, win}), expected_probe_reports);
}


TEST(GenericProbeTimingTest, ReadProbeTimings) {
    std::istringstream in{
R"(duration (ns)   position #   extended position description
       155441            0   8/8/2P5/2k5/4B3/8/6N1/3K4 w - - GTV w; fmvn 1; hmvc 0;
      1243066            2   7r/8/2N5/8/1K6/6k1/8/5Q2 w - - GTV w; fmvn 1; hmvc 0;
         5500            4   8/8/6K1/8/2n5/3N1k2/8/6N1 b - - GTV d; fmvn 1; hmvc 0;
        18608            6   8/8/2B4k/3K2p1/4B3/8/8/8 w - - GTV d; fmvn 1; hmvc 0;
        55015            8   4K3/8/1R6/b7/4k3/8/8/2B5 b - - GTV d; fmvn 1; hmvc 0;
)"};
    std::vector<bool> has_been_read_in(10);
    std::vector<std::chrono::steady_clock::duration> probe_durations(10);
    std::vector<epd::extended_position_description_type> epds(10);

    read_probe_timings(in, has_been_read_in, probe_durations, epds, 0);

    EXPECT_TRUE(has_been_read_in[0]);
    EXPECT_EQ(155441, probe_durations[0].count());
    EXPECT_FALSE(has_been_read_in[1]);
    EXPECT_TRUE(has_been_read_in[2]);
    EXPECT_EQ(1243066, probe_durations[2].count());
    EXPECT_FALSE(has_been_read_in[3]);
    EXPECT_TRUE(has_been_read_in[4]);
    EXPECT_EQ(5500, probe_durations[4].count());
    EXPECT_FALSE(has_been_read_in[5]);
    EXPECT_TRUE(has_been_read_in[6]);
    EXPECT_EQ(18608, probe_durations[6].count());
    EXPECT_FALSE(has_been_read_in[7]);
    EXPECT_TRUE(has_been_read_in[8]);
    EXPECT_EQ(55015, probe_durations[8].count());
    EXPECT_FALSE(has_been_read_in[9]);
}
