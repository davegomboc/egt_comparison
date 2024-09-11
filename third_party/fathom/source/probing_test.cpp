#include "probing_test.hpp"


TEST(ProbingInit, SyzygyTableFilesAvailable) {
    unsigned int const figurine_count_available{probing_init()};
    EXPECT_NE(0, figurine_count_available);
}

TEST(ProbingInit, SyzygyTableFilesNotAvailable) {
    unsigned int const figurine_count_available{probing_init("/nonexistent")};
    EXPECT_EQ(0, figurine_count_available);
}

TEST(EgtProbe, EmptyFenString) {
    probing_init("");
    probe_report_type const probe_report{egt_probe("")};
    EXPECT_EQ(invalid, probe_report);
}

TEST(EgtProbe, Ka8wtm) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("K7/8/8/8/8/8/8/8 w - - 0 1")};
    EXPECT_EQ(illegal, probe_report);
}

TEST(EgtProbe, Kc8ka8wtm) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("k1K5/8/8/8/8/8/8/8 w - - 0 1")};
    EXPECT_EQ(draw, probe_report);
}

TEST(EgtProbe, Kc8Qb8ka8wtm) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("kQK5/8/8/8/8/8/8/8 w - - 0 1")};
    EXPECT_EQ(illegal, probe_report);
}

TEST(EgtProbe, Kc8Qd8ka8wtm) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("k1KQ4/8/8/8/8/8/8/8 w - - 0 1")};
    EXPECT_EQ(win, probe_report);
}

TEST(EgtProbe, Kc8Rb8ka8btm) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("kRK5/8/8/8/8/8/8/8 b - - 0 1")};
    EXPECT_EQ(loss, probe_report);
}

TEST(EgtProbe, Kc8Bb8ka8btm) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("kBK5/8/8/8/8/8/8/8 b - - 0 1")};
    EXPECT_EQ(draw, probe_report);
}

TEST(EgtProbe, Kc8Nb8ka8btm) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("kNK5/8/8/8/8/8/8/8 b - - 0 1")};
    EXPECT_EQ(draw, probe_report);
}

TEST(EgtProbe, Kc8Pb8ka8btm) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("kPK5/8/8/8/8/8/8/8 b - - 0 1")};
    EXPECT_EQ(illegal, probe_report);
}

TEST(EgtProbe, KRBkrMateIn51) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("8/8/1r2B3/8/8/5K2/6R1/7k w - - 0 1")};
    EXPECT_EQ(cursed_win, probe_report);
}

TEST(EgtProbe, KRBkrMatedIn50) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("8/8/1r2B3/8/8/5K2/4R3/7k b - - 0 1")};
    EXPECT_EQ(loss, probe_report);
}

TEST(EgtProbe, Kc6Re7Ng6kb4btmNormalFEN) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("8/4R3/2K3N1/8/1k6/8/8/8 b - - 0 1")};
    EXPECT_EQ(loss, probe_report);
}

TEST(EgtProbe, Kc6Re7Ng6kb4btmVisualFEN) {
    probing_init("");
    probe_report_type const probe_report{egt_probe("11111111/1111R111/11K111N1/"
     "11111111/1k111111/11111111/11111111/11111111 b - - 0 1")};
    EXPECT_EQ(loss, probe_report);
}

// This test formerly triggered undefined behaviour in tbchess.c.
TEST(EgtProbe, Ke4kc6rf1pg3Fen) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("8/8/2k5/8/4K3/6p1/8/5r2 b - - 0 1")};
    EXPECT_EQ(win, probe_report);
}

TEST(EgtProbe, OnlyEnPassantCaptureYieldsBestResultForSideToMovePositionOne) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("8/8/8/8/3Pp3/1K6/8/k7 b - d3 0 1")};
    EXPECT_EQ(draw, probe_report);
}

TEST(EgtProbe, OnlyEnPassantCaptureYieldsBestResultForSideToMovePositionTwo) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("7k/8/8/8/4pP2/8/1K6/8 b - f3 0 1")};
    EXPECT_EQ(win, probe_report);
}

TEST(EgtProbe, OnlyEnPassantCaptureYieldsBestResultForSideToMovePositionThree) {
    probing_init("");
    probe_report_type const probe_report{
     egt_probe("8/8/8/8/3Pp3/1K6/8/k7 b - d3 0 1")};
    EXPECT_EQ(draw, probe_report);
}
