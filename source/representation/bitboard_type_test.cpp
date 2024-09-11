#include "representation/bitboard_type_test.hpp"


#include <sstream>

#include "fathom.hpp"
#include "representation/coordinate_type.hpp"


using namespace representation;


TEST(BitboardType, Constructa8) {
    bitboard_type const bitboard(a8);
    std::bitset<64> bits_a8{
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001ull};
    bitboard_type const bitboard_from_bits{bits_a8};
    EXPECT_EQ(bitboard, bitboard_from_bits);
}

TEST(BitboardType, Constructh8) {
    bitboard_type const bitboard(h8);
    std::bitset<64> bits_h8{
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000000ull};
    bitboard_type const bitboard_from_bits{bits_h8};
    EXPECT_EQ(bitboard, bitboard_from_bits);
}

TEST(BitboardType, Constructa1) {
    bitboard_type const bitboard(a1);
    std::bitset<64> bits_a1{
     0b00000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull};
    bitboard_type const bitboard_from_bits{bits_a1};
    EXPECT_EQ(bitboard, bitboard_from_bits);
}

TEST(BitboardType, Constructh1) {
    bitboard_type const bitboard(h1);
    std::bitset<64> bits_h1{
     0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull};
    bitboard_type const bitboard_from_bits{bits_h1};
    EXPECT_EQ(bitboard, bitboard_from_bits);
}

TEST(BitboardType, AsSyzygyBitboardValuea8) {
    bitboard_type const bitboard(a8);
    std::optional<tb_position_type> tb_position_opt{
     parse_fen("K7/8/8/8/8/8/8/8 w - - 0 1")};
    std::uint64_t const syzygy_bitboard{bitboard.as_syzygy_bitboard_value()};
    ASSERT_TRUE(tb_position_opt);
    EXPECT_EQ(syzygy_bitboard, tb_position_opt->white);
    EXPECT_EQ(syzygy_bitboard, tb_position_opt->kings);
}

TEST(BitboardType, AsSyzygyBitboardValueh8) {
    bitboard_type const bitboard(h8);
    std::optional<tb_position_type> tb_position_opt{
     parse_fen("7K/8/8/8/8/8/8/8 w - - 0 1")};
    std::uint64_t const syzygy_bitboard{bitboard.as_syzygy_bitboard_value()};
    ASSERT_TRUE(tb_position_opt);
    EXPECT_EQ(syzygy_bitboard, tb_position_opt->white);
    EXPECT_EQ(syzygy_bitboard, tb_position_opt->kings);
}

TEST(BitboardType, AsSyzygyBitboardValuea1) {
    bitboard_type const bitboard(a1);
    std::optional<tb_position_type> tb_position_opt{
     parse_fen("8/8/8/8/8/8/8/K7 w - - 0 1")};
    std::uint64_t const syzygy_bitboard{bitboard.as_syzygy_bitboard_value()};
    ASSERT_TRUE(tb_position_opt);
    EXPECT_EQ(syzygy_bitboard, tb_position_opt->white);
    EXPECT_EQ(syzygy_bitboard, tb_position_opt->kings);
}

TEST(BitboardType, AsSyzygyBitboardValueh1) {
    bitboard_type const bitboard(h1);
    std::optional<tb_position_type> tb_position_opt{
     parse_fen("8/8/8/8/8/8/8/7K w - - 0 1")};
    std::uint64_t const syzygy_bitboard{bitboard.as_syzygy_bitboard_value()};
    ASSERT_TRUE(tb_position_opt);
    EXPECT_EQ(syzygy_bitboard, tb_position_opt->white);
    EXPECT_EQ(syzygy_bitboard, tb_position_opt->kings);
}
