#include "chesspresso_endgame_table_type_test.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <strstream>

#include "initialize_with.hpp"
#include "mutable_memory_stream_buffer_view_type.hpp"

#include "representation/cover_enum.hpp"
#include "representation/egt_position_type.hpp"

#include "chesspresso_endgame_table_parameterized_testing_type.hpp"
#include "paths.hpp"


using namespace representation;


std::array<std::uint64_t, 1> constexpr empty{};

std::array<std::uint64_t, 2> constexpr partial_header{0, 0};

std::array<std::uint64_t, 4> constexpr zero_cubes{0, 0, 0, 0};

std::array<std::uint64_t, 5> constexpr minimized_K_vs_k_F{
    29, 1, 0, 0,
    0b010'111111111111'111111111111'11,
};

std::array<std::uint64_t, 6> constexpr minimized_K_vs_k_FR{
    29, 1, 1, 0,
    0b010'111111111111'111111111111'11,
    0b101'111111111111'111111111111'11,
};


TEST(ChesspressoEndgameTableType, Empty) {
    std::strstreambuf buffer(reinterpret_cast<char const *>(empty.data()),
     empty.size() * sizeof(std::uint64_t));
    std::istream data_in(&buffer);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     chesspresso_endgame_table_type::create_ptr(data_in, 3)};
    EXPECT_EQ(nullptr, table_ptr);
}

TEST(ChesspressoEndgameTableType, PartialHeader) {
    std::strstreambuf buffer(reinterpret_cast<char const *>(
     partial_header.data()), partial_header.size() * sizeof(std::uint64_t));
    std::istream data_in(&buffer);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     chesspresso_endgame_table_type::create_ptr(data_in, 3)};
    EXPECT_EQ(nullptr, table_ptr);
}

TEST(ChesspressoEndgameTableType, ZeroCubes) {
    std::strstreambuf buffer(reinterpret_cast<char const *>(zero_cubes.data()),
     zero_cubes.size() * sizeof(std::uint64_t));
    std::istream data_in(&buffer);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     chesspresso_endgame_table_type::create_ptr(data_in, 3)};
    ASSERT_NE(nullptr, table_ptr);
    EXPECT_TRUE(table_ptr->cover(cover_enum::function).empty());
    EXPECT_TRUE(table_ptr->cover(cover_enum::reverse).empty());
    EXPECT_TRUE(table_ptr->cover(cover_enum::do_not_care).empty());
}

TEST(ChesspressoEndgameTableType, PreminimizedKVskF) {
    std::strstreambuf buffer(reinterpret_cast<char const *>(
     minimized_K_vs_k_F.data()),
     minimized_K_vs_k_F.size() * sizeof(std::uint64_t));
    std::istream data_in(&buffer);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     chesspresso_endgame_table_type::create_ptr(data_in, 3)};
    ASSERT_NE(nullptr, table_ptr);
    EXPECT_EQ(1, table_ptr->cover(cover_enum::function).size());
    EXPECT_TRUE(table_ptr->cover(cover_enum::reverse).empty());
    EXPECT_TRUE(table_ptr->cover(cover_enum::do_not_care).empty());
}

TEST(ChesspressoEndgameTableType, PrintPreminimizedKVskF) {
    std::strstreambuf buffer(reinterpret_cast<char const *>(
     minimized_K_vs_k_F.data()),
     minimized_K_vs_k_F.size() * sizeof(std::uint64_t));
    std::istream data_in(&buffer);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     chesspresso_endgame_table_type::create_ptr(data_in, 3)};
    ASSERT_NE(nullptr, table_ptr);
    ASSERT_EQ(1, table_ptr->cover(cover_enum::function).size());
    std::ostringstream out;
    bool failed{table_ptr->print(out, 1)};
    EXPECT_FALSE(failed);
    EXPECT_EQ(out.str(), R"(.type f
.i 13
.o 3
.p 1
------------- ~1~
.end
)");
}

TEST(ChesspressoEndgameTableType, PreminimizedKVskFR) {
    std::strstreambuf buffer(reinterpret_cast<char const *>(
     minimized_K_vs_k_FR.data()),
     minimized_K_vs_k_FR.size() * sizeof(std::uint64_t));
    std::istream data_in(&buffer);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     chesspresso_endgame_table_type::create_ptr(data_in, 3)};
    ASSERT_NE(nullptr, table_ptr);
    EXPECT_EQ(1, table_ptr->cover(cover_enum::function).size());
    EXPECT_EQ(1, table_ptr->cover(cover_enum::reverse).size());
    EXPECT_TRUE(table_ptr->cover(cover_enum::do_not_care).empty());
}

TEST(ChesspressoEndgameTableType, PrintPreminimizedKVskFR) {
    std::strstreambuf buffer(reinterpret_cast<char const *>(
     minimized_K_vs_k_FR.data()),
     minimized_K_vs_k_FR.size() * sizeof(std::uint64_t));
    std::istream data_in(&buffer);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     chesspresso_endgame_table_type::create_ptr(data_in, 3)};
    ASSERT_NE(nullptr, table_ptr);
    ASSERT_EQ(1, table_ptr->cover(cover_enum::function).size());
    ASSERT_EQ(1, table_ptr->cover(cover_enum::reverse).size());
    std::ostringstream out;
    bool failed{table_ptr->print(out, 2)};
    EXPECT_FALSE(failed);
    EXPECT_EQ(out.str(), R"(.type fr
.i 13
.o 3
.p 2
------------- ~1~
------------- 0~0
.end
)");
}

TEST(ChesspressoEndgameTableType, RoundTrippingPreminimizedKVsKF) {
    std::size_t constexpr data_char_count{
     minimized_K_vs_k_F.size() * sizeof(std::uint64_t)};
    char const * const chars_in_begin{reinterpret_cast<char const *>(
     minimized_K_vs_k_F.data())};
    char const * const chars_in_end{chars_in_begin + data_char_count};
    std::istrstream data_in(reinterpret_cast<char const *>(
     minimized_K_vs_k_F.data()), data_char_count);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     chesspresso_endgame_table_type::create_ptr(data_in, 3)};
    std::array<char, data_char_count> reserialized_data;
    mutable_memory_stream_buffer_view_type reserialized_view(
     reserialized_data.begin(), reserialized_data.end());
    std::ostream data_out(&reserialized_view);
    bool const write_failed{table_ptr->write(data_out, false)};
    ASSERT_FALSE(write_failed);
    EXPECT_TRUE(std::equal(chars_in_begin, chars_in_end,
     reserialized_data.cbegin(), reserialized_data.cend()));
}

TEST(ChesspressoEndgameTableType, ReadInThreeUnminimizedCoversForKVsk) {
    auto K_vs_k_opt{material_signature_type::find_and_create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt.has_value());
    std::filesystem::path const K_vs_k_plb_xz_path{chesspresso_path(
     std::nullopt, *K_vs_k_opt, std::nullopt, true, true)};
    std::unique_ptr<chesspresso_endgame_table_type> K_vs_k_egt_ptr{
     read_chesspresso_endgame_table(K_vs_k_plb_xz_path, 3)};
    ASSERT_NE(nullptr, K_vs_k_egt_ptr);
    cover_type const & F(K_vs_k_egt_ptr->cover(cover_enum::function));
    cover_type const & R(K_vs_k_egt_ptr->cover(cover_enum::reverse));
    cover_type const & D(K_vs_k_egt_ptr->cover(cover_enum::do_not_care));
    EXPECT_EQ(462, F.size());
    EXPECT_EQ(462, R.size());
    EXPECT_EQ(7730, D.size());
}

TEST(ChesspressoEndgameTableType, ReadInTwoUnminimizedCoversForKVsk) {
    auto K_vs_k_opt{material_signature_type::find_and_create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt.has_value());
    std::filesystem::path const K_vs_k_plb_xz_path{chesspresso_path(
     std::nullopt, *K_vs_k_opt, std::nullopt, true, true)};
    std::unique_ptr<chesspresso_endgame_table_type> K_vs_k_egt_ptr{
     read_chesspresso_endgame_table(K_vs_k_plb_xz_path, 2)};
    ASSERT_NE(nullptr, K_vs_k_egt_ptr);
    EXPECT_TRUE(K_vs_k_egt_ptr->cover(cover_enum::do_not_care).empty());
    cover_type const & F(K_vs_k_egt_ptr->cover(cover_enum::function));
    cover_type const & R(K_vs_k_egt_ptr->cover(cover_enum::reverse));
    EXPECT_EQ(462, F.size());
    EXPECT_EQ(462, R.size());
}

TEST(ChesspressoEndgameTableType, ReadInSingleUnminimizedCoverForKQVsk) {
    auto KQ_vs_k_opt{material_signature_type::find_and_create_opt("KQ_vs_k")};
    ASSERT_TRUE(KQ_vs_k_opt.has_value());
    std::filesystem::path const KQ_vs_k_plb_xz_path{chesspresso_path(
     std::nullopt, *KQ_vs_k_opt, std::nullopt, true, true)};
    std::unique_ptr<chesspresso_endgame_table_type> KQ_vs_k_egt_ptr{
     read_chesspresso_endgame_table(KQ_vs_k_plb_xz_path, 1)};
    ASSERT_NE(nullptr, KQ_vs_k_egt_ptr);
    EXPECT_TRUE(KQ_vs_k_egt_ptr->cover(cover_enum::reverse).empty());
    EXPECT_TRUE(KQ_vs_k_egt_ptr->cover(cover_enum::do_not_care).empty());
    cover_type const & F(KQ_vs_k_egt_ptr->cover(cover_enum::function));
    EXPECT_EQ(46137, F.size());
}


INSTANTIATE_TEST_SUITE_P(ChesspressoEndgameTableType,
 chesspresso_endgame_table_parameterized_testing_type,
 testing::ValuesIn(legal_wcdbl_test_positions()));
