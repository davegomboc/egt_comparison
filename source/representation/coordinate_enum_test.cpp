#include "representation/coordinate_enum_test.hpp"


#include <sstream>


using representation::coordinate_enum;
using representation::file_enum;
using representation::rank_enum;
using representation::remove_coordinate;


TEST(CoordinateEnum, ZeroIsValid) {
    EXPECT_TRUE(valid(static_cast<coordinate_enum>(0)));
}

TEST(CoordinateEnum, SixtyThreeIsValid) {
    EXPECT_TRUE(valid(static_cast<coordinate_enum>(63)));
}

TEST(CoordinateEnum, SixtyFourIsInvalid) {
    EXPECT_FALSE(valid(static_cast<coordinate_enum>(64)));
}

TEST(CoordinateEnum, Positive255IsInvalid) {
    EXPECT_FALSE(valid(static_cast<coordinate_enum>(255)));
}

TEST(CoordinateEnum, PredecrementGoesFromb8Toa8) {
    coordinate_enum a8{file_enum::a_file | rank_enum::rank_8};
    coordinate_enum b8{file_enum::b_file | rank_enum::rank_8};
    EXPECT_EQ(--b8, a8);
}

TEST(CoordinateEnum, PredecrementGoesFroma7Toh8) {
    coordinate_enum a7{file_enum::a_file | rank_enum::rank_7};
    coordinate_enum h8{file_enum::h_file | rank_enum::rank_8};
    EXPECT_EQ(--a7, h8);
}

TEST(CoordinateEnum, PredecrementGoesFrome4Tod4) {
    coordinate_enum d4{file_enum::d_file | rank_enum::rank_4};
    coordinate_enum e4{file_enum::e_file | rank_enum::rank_4};
    EXPECT_EQ(--e4, d4);
}

TEST(CoordinateEnum, PredecrementGoesFroma8Toh1) {
    coordinate_enum a8{file_enum::a_file | rank_enum::rank_8};
    coordinate_enum h1{file_enum::h_file | rank_enum::rank_1};
    EXPECT_EQ(--a8, h1);
}

TEST(CoordinateEnum, PreincrementGoesFroma8Tob8) {
    coordinate_enum a8{file_enum::a_file | rank_enum::rank_8};
    coordinate_enum b8{file_enum::b_file | rank_enum::rank_8};
    EXPECT_EQ(b8, ++a8);
}

TEST(CoordinateEnum, PreincrementGoesFromh8Toa7) {
    coordinate_enum a7{file_enum::a_file | rank_enum::rank_7};
    coordinate_enum h8{file_enum::h_file | rank_enum::rank_8};
    EXPECT_EQ(a7, ++h8);
}

TEST(CoordinateEnum, PreincrementGoesFromd4Toe4) {
    coordinate_enum d4{file_enum::d_file | rank_enum::rank_4};
    coordinate_enum e4{file_enum::e_file | rank_enum::rank_4};
    EXPECT_EQ(e4, ++d4);
}

TEST(CoordinateEnum, PreincrementGoesFromh1Toa8) {
    coordinate_enum a8{file_enum::a_file | rank_enum::rank_8};
    coordinate_enum h1{file_enum::h_file | rank_enum::rank_1};
    EXPECT_EQ(a8, ++h1);
}

TEST(CoordinateEnum, InserterWitha8Andh1) {
    coordinate_enum a8{file_enum::a_file | rank_enum::rank_8};
    coordinate_enum h1{file_enum::h_file | rank_enum::rank_1};
    std::ostringstream out;
    out << a8 << h1;
    EXPECT_EQ("a8h1", out.str());
}

TEST(CoordinateEnum, AppendToPositionId) {
    coordinate_enum f2{file_enum::f_file | rank_enum::rank_2};
    std::size_t position_id{1};
    append_coordinate(position_id, f2);
    EXPECT_EQ(position_id, 0b1'110'101);
}

TEST(CoordinateEnum, RemoveFromPositionId) {
    std::size_t position_id{0b1'110'101};
    coordinate_enum coordinate{remove_coordinate(position_id)};
    EXPECT_EQ(coordinate, file_enum::f_file | rank_enum::rank_2);
    EXPECT_EQ(position_id, 1);
}

TEST(CoordinateEnum, AppendToBoolVector) {
    coordinate_enum g4{file_enum::g_file | rank_enum::rank_4};
    std::vector<bool> expected_bits{true, false, false, true, true, false};
    std::vector<bool> bits{};
    append_coordinate(bits, g4);
    EXPECT_TRUE(std::equal(expected_bits.cbegin(), expected_bits.cend(),
     bits.cbegin(), bits.cend()));
}

TEST(CoordinateEnum, AppendToUnsignedCharVector) {
    coordinate_enum g4{file_enum::g_file | rank_enum::rank_4};
    std::vector<unsigned char> expected_bits_as_uchars{
     true, false, false, true, true, false};
    std::vector<unsigned char> bits_as_uchars{};
    append_coordinate(bits_as_uchars, g4);
    EXPECT_TRUE(std::equal(expected_bits_as_uchars.cbegin(),
     expected_bits_as_uchars.cend(), bits_as_uchars.cbegin(),
     bits_as_uchars.cend()));
}

TEST(CoordinateEnum, InitializeFromIdAsBoolVector) {
    coordinate_enum g4{file_enum::g_file | rank_enum::rank_4};
    std::vector<bool> bits{true, false, false, true, true, false};
    coordinate_enum coordinate{remove_coordinate(bits)};
    EXPECT_TRUE(bits.empty());
    EXPECT_EQ(g4, coordinate);
}

TEST(CoordinateEnum, InitializeFromIdAsUnsignedCharVector) {
    coordinate_enum g4{file_enum::g_file | rank_enum::rank_4};
    std::vector<unsigned char> bits_as_uchars{
     true, false, false, true, true, false};
    coordinate_enum coordinate{remove_coordinate(bits_as_uchars)};
    EXPECT_TRUE(bits_as_uchars.empty());
    EXPECT_EQ(g4, coordinate);
}
