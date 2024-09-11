#include "prober_type_test.hpp"


/* The first four tests are intended to only be run standalone via
   --gtest_filter.

// Assumes that the environment variable SYZYGY_PATH is appropriately set.
TEST(ProberType, CreateSucceeds) {
    std::optional<prober_type const> prober_opt{prober_type::create_once_opt()};
    EXPECT_NE(std::nullopt, prober_opt);
}

// Assumes that the directory /nonexistent does not actually both exist and
// contain Syzygy endgame tables.
TEST(ProberType, CreateFails) {
    std::optional<prober_type const> prober_opt{
     prober_type::create_once_opt("/nonexistent")};
    EXPECT_EQ(std::nullopt, prober_opt);
}

// Assumes that the environment variable SYZYGY_PATH is appropriately set, and
// also that the directory /nonexistent does not actually both exist and contain
// Syzygy endgame tables.
TEST(ProberType, CreateFailsThenSucceeds) {
    std::optional<prober_type const> first_prober_opt{
     prober_type::create_once_opt("/nonexistent")};
    std::optional<prober_type const> second_prober_opt{
     prober_type::create_once_opt()};
    EXPECT_EQ(std::nullopt, first_prober_opt);
    EXPECT_NE(std::nullopt, second_prober_opt);
}

// Assumes that the environment variable SYZYGY_PATH is appropriately set, and
// also that the directory /nonexistent does not actually both exist and contain
// Syzygy endgame tables.
TEST(ProberType, CreateSucceedsThenReturnsCached) {
    std::optional<prober_type const> first_prober_opt{
     prober_type::create_once_opt()};
    std::optional<prober_type const> second_prober_opt{
     prober_type::create_once_opt("/nonexistent")};
    EXPECT_NE(std::nullopt, first_prober_opt);
    EXPECT_NE(std::nullopt, second_prober_opt);
}
*/

// Many of the following positions used to trigger undefined behaviour.
TEST(ProberType, Kc6Re7Ng6kb4btmNormalFEN) {
    std::optional<prober_type const> prober_opt{
     prober_type::create_once_opt("")};
    ASSERT_NE(std::nullopt, prober_opt);
    prober_type const & prober{*prober_opt};
    probe_report_type const probe_report{
     prober.probe("8/4R3/2K3N1/8/1k6/8/8/8 b - - 0 1")};
    EXPECT_EQ(loss, probe_report);
}

TEST(ProberType, Kc6Re7Ng6kb4btmVisualFEN) {
    std::optional<prober_type const> prober_opt{prober_type::create_once_opt()};
    ASSERT_NE(std::nullopt, prober_opt);
    prober_type const & prober{*prober_opt};
    probe_report_type const probe_report{prober.probe("11111111/1111R111/"
     "11K111N1/11111111/1k111111/11111111/11111111/11111111 b - - 0 1")};
    EXPECT_EQ(loss, probe_report);
}

TEST(ProberType, Ke4kc6rf1pg3NormalFEN) {
    std::optional<prober_type const> prober_opt{
     prober_type::create_once_opt("")};
    ASSERT_NE(std::nullopt, prober_opt);
    prober_type const & prober{*prober_opt};
    probe_report_type const probe_report{
     prober.probe("8/8/2k5/8/4K3/6p1/8/5r2 b - - 0 1")};
    EXPECT_EQ(win, probe_report);
}

TEST(ProberType, Ke4kc6rf1pg3VisualFEN) {
    std::optional<prober_type const> prober_opt{
     prober_type::create_once_opt("")};
    ASSERT_NE(std::nullopt, prober_opt);
    prober_type const & prober{*prober_opt};
    probe_report_type const probe_report{prober.probe("11111111/11111111/"
     "11k11111/11111111/1111K111/111111p1/11111111/11111r11 b - - 0 1")};
    EXPECT_EQ(win, probe_report);
}
