#include "bit_region_uint_operators_test.hpp"


#include <bitset>

/*[prior commenting out]
TODO: Avoid calling uint_ref() when region data is nullptr in
apply_on_bit_run().

TEST(BitRegionUintOperators, ZeroBitsHasAllBitsSet) {
    using uint_type = std::uint64_t;
    std::array<uint_type, 0> bits{};
    bit_region_type<uint_type> region(0, bits.data());
    bool const result{all_ones<uint_type>(region)};
    EXPECT_TRUE(result);
    EXPECT_EQ(std::bitset<0>().all(), result);
}

TEST(BitRegionUintOperators, ZeroBitsHasNoBitsSet) {
    std::array<std::uint64_t, 0> bits{};
    bit_region_type region(0, bits.data());
    bool const result{all_zeroes(region)};
    EXPECT_TRUE(result);
    EXPECT_EQ(std::bitset<0>().none(), result);
}
*/

/*
TEST(BitRegionUintOperators, Popcount) {
    std::array<std::uint8_t, 4> bits{
     0b00010000, 0b00100100, 0b01100001, 0b00001111};
    bit_region_type region(32, bits.data());
    std::size_t const result{popcount_of_run(5, 32, region)};
    EXPECT_EQ(9, result);
}

TEST(BitRegionUintOperators, SingleFalseBitDoesNotHaveAllBitsSet) {
    std::array<std::uint64_t, 1> bits{UINT64_C(0x5000500050005000)};
    bit_region_type region(1, bits.data());
    bool const result{all_ones(region)};
    EXPECT_FALSE(result);
}

TEST(BitRegionUintOperators, SingleTrueBitHasNoBitSet) {
    std::array<std::uint64_t, 1> bits{UINT64_C(0x5000500050005000)};
    bit_region_type region(1, bits.data());
    bool const result{all_zeroes(region)};
    EXPECT_TRUE(result);
}

TEST(BitRegionUintOperators, SingleTrueBitHasAllBitsSet) {
    std::array<std::uint64_t, 1> bits{UINT64_C(0x0005000500050005)};
    bit_region_type region(1, bits.data());
    bool const result{all_ones(region)};
    EXPECT_TRUE(result);
}

TEST(BitRegionUintOperators, SingleTrueBitDoesNotHaveNoBitSet) {
    std::array<std::uint64_t, 1> bits{UINT64_C(0x0005000500050005)};
    bit_region_type region(1, bits.data());
    bool const result{all_zeroes(region)};
    EXPECT_FALSE(result);
}

TEST(BitRegionUintOperators, ThirtyTrueBitsHasAllBitsSet) {
    std::array<std::uint8_t, 4> bits{0xff, 0xff, 0xff, 0b00111111};
    bit_region_type region(32, bits.data());
    bool const result{run_is_all_ones(0, 30, region)};
    EXPECT_TRUE(result);
}

TEST(BitRegionUintOperators, ThirtyFalseBitsHasNoBitsSet) {
    std::array<std::uint8_t, 4> bits{0, 0, 0, 0b11000000};
    bit_region_type region(32, bits.data());
    bool const result{run_is_all_zeroes(0, 30, region)};
    EXPECT_TRUE(result);
}

TEST(BitRegionUintOperators, ResetLowestBitPair) {
    using uint_type = std::uint64_t;
    std::array<uint_type, 2> bits{
     UINT64_C(0xffffffffffffffff), UINT64_C(0xffffffffffffffff)};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    reset_run(0, 2, region);
    EXPECT_EQ(UINT64_C(0xfffffffffffffffc), bits[0]);
    EXPECT_EQ(UINT64_C(0xffffffffffffffff), bits[1]);
}

TEST(BitRegionUintOperators, SetLowestBitPair) {
    using uint_type = std::uint64_t;
    std::array<uint_type, 2> bits{};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    set_run(0, 2, region);
    EXPECT_EQ(3, bits[0]);
    EXPECT_EQ(0, bits[1]);
}

TEST(BitRegionUintOperators, ResetHighestBitPair) {
    using uint_type = std::uint32_t;
    std::array<uint_type, 2> bits{UINT32_C(0xffffffff), UINT32_C(0xffffffff)};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    reset_run(62, 64, region);
    EXPECT_EQ(UINT32_C(0xffffffff), bits[0]);
    EXPECT_EQ(UINT32_C(0x3fffffff), bits[1]);
}

TEST(BitRegionUintOperators, SetHighestBitPair) {
    using uint_type = std::uint32_t;
    std::array<uint_type, 2> bits{};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    set_run(62, 64, region);
    EXPECT_EQ(0x00000000, bits[0]);
    EXPECT_EQ(0xc0000000, bits[1]);
}

TEST(BitRegionUintOperators, ResetHighBitPairInLowElement) {
    using uint_type = std::uint16_t;
    std::array<uint_type, 2> bits{0xffff, 0xffff};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    reset_run(14, 16, region);
    EXPECT_EQ(0b0011111111111111, bits[0]);
    EXPECT_EQ(0b1111111111111111, bits[1]);
}

TEST(BitRegionUintOperators, SetHighBitPairInLowElement) {
    using uint_type = std::uint16_t;
    std::array<uint_type, 2> bits{};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    set_run(14, 16, region);
    EXPECT_EQ(0b1100000000000000, bits[0]);
    EXPECT_EQ(0b0000000000000000, bits[1]);
}

TEST(BitRegionUintOperators, ResetLowBitPairInHighElement) {
    using uint_type = std::uint16_t;
    std::array<uint_type, 2> bits{0xffff, 0xffff};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    reset_run(16, 18, region);
    EXPECT_EQ(0b1111111111111111, bits[0]);
    EXPECT_EQ(0b1111111111111100, bits[1]);
}

TEST(BitRegionUintOperators, SetLowBitPairInHighElement) {
    using uint_type = std::uint16_t;
    std::array<uint_type, 2> bits{};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    set_run(16, 18, region);
    EXPECT_EQ(0b0000000000000000, bits[0]);
    EXPECT_EQ(0b0000000000000011, bits[1]);
}

TEST(BitRegionUintOperators, ResetMiddleBitsSplitAcrossElements) {
    using uint_type = std::uint16_t;
    std::array<uint_type, 2> bits{0xffff, 0xffff};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    reset_run(15, 17, region);
    EXPECT_EQ(0b0111111111111111, bits[0]);
    EXPECT_EQ(0b1111111111111110, bits[1]);
}

TEST(BitRegionUintOperators, SetMiddleBitsSplitAcrossElements) {
    using uint_type = std::uint16_t;
    std::array<uint_type, 2> bits{};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    set_run(15, 17, region);
    bitwise_or_eq_type<uint_type> setter;
    setter.apply_on_bit_run(15, 17, region, all_bits_set<uint_type>);
    EXPECT_EQ(0b1000000000000000, bits[0]);
    EXPECT_EQ(0b0000000000000001, bits[1]);
}

TEST(BitRegionUintOperators, ResetAlignedMultipleUintsWorthOfBits) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 4> bits{0xff, 0xff, 0xff, 0xff};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    reset_run(8, 24, region);
    EXPECT_EQ(0b11111111, bits[0]);
    EXPECT_EQ(0b00000000, bits[1]);
    EXPECT_EQ(0b00000000, bits[2]);
    EXPECT_EQ(0b11111111, bits[3]);
}

TEST(BitRegionUintOperators, SetAlignedMultipleUintsWorthOfBits) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 4> bits{};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    set_run(8, 24, region);
    EXPECT_EQ(0b00000000, bits[0]);
    EXPECT_EQ(0b11111111, bits[1]);
    EXPECT_EQ(0b11111111, bits[2]);
    EXPECT_EQ(0b00000000, bits[3]);
}

TEST(BitRegionUintOperators, ResetMisalignedMultipleUintsWorthOfBits) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 4> bits{0xff, 0xff, 0xff, 0xff};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    reset_run(3, 25, region);
    EXPECT_EQ(0b00000111, bits[0]);
    EXPECT_EQ(0b00000000, bits[1]);
    EXPECT_EQ(0b00000000, bits[2]);
    EXPECT_EQ(0b11111110, bits[3]);
}

TEST(BitRegionUintOperators, SetMisalignedMultipleUintsWorthOfBits) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 4> bits{};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    set_run(3, 25, region);
    EXPECT_EQ(0b11111000, bits[0]);
    EXPECT_EQ(0b11111111, bits[1]);
    EXPECT_EQ(0b11111111, bits[2]);
    EXPECT_EQ(0b00000001, bits[3]);
}

TEST(BitRegionUintOperators, FlipWithinSingleUint) {
    using uint_type = std::uint32_t;
    std::array<uint_type, 1> bits{
     UINT32_C(0b1001'1001'1001'1001'1001'1001'1001'1001)};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    flip_run(2, 25, region);
    EXPECT_EQ(UINT32_C(0b1001'1000'0110'0110'0110'0110'0110'0101), bits[0]);
}

TEST(BitRegionUintOperators, FlipMisalignedMultipleUintsWorthOfBits) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 4> bits{
     0b01110001, 0b11000111, 0b00011100, 0b01110001};
    bit_region_type region(bits.size() * bits_per<uint_type>, bits.data());
    flip_run(3, 25, region);
    EXPECT_EQ(0b10001001, bits[0]);
    EXPECT_EQ(0b00111000, bits[1]);
    EXPECT_EQ(0b11100011, bits[2]);
    EXPECT_EQ(0b01110000, bits[3]);
}

TEST(BitRegionUintOperators, BitwiseAndDoesNotGiveAllZeroes) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 2> lhs_bits{0b01101010, 0b00000101};
    bit_region_type<uint_type> lhs_region(11, lhs_bits.data());
    std::array<uint_type, 2> rhs_bits{0b10010101, 0b00000011};
    bit_region_type<uint_type> rhs_region(11, rhs_bits.data());

    bool const result{bitwise_and_gives_all_zeroes<uint_type>(lhs_region,
     rhs_region)};

    EXPECT_FALSE(result);
}

TEST(BitRegionUintOperators, BitwiseAndGivesAllZeroes) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 2> lhs_bits{0b01101010, 0b00000100};
    bit_region_type<uint_type> lhs_region(11, lhs_bits.data());
    std::array<uint_type, 2> rhs_bits{0b10010101, 0b00000011};
    bit_region_type<uint_type> rhs_region(11, rhs_bits.data());

    bool const result{bitwise_and_gives_all_zeroes<uint_type>(lhs_region,
     rhs_region)};

    EXPECT_TRUE(result);
}

TEST(BitRegionUintOperators, BitwiseOrDoesNotGiveAllOnes) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 2> lhs_bits{0b01101010, 0b00000100};
    bit_region_type<uint_type const> lhs_region(11, lhs_bits.data());
    std::array<uint_type, 2> rhs_bits{0b10010101, 0b00000010};
    bit_region_type<uint_type const> rhs_region(11, rhs_bits.data());

    bool const result{bitwise_or_gives_all_ones<uint_type>(lhs_region,
     rhs_region)};

    EXPECT_FALSE(result);
}

TEST(BitRegionUintOperators, BitwiseOrGivesAllOnes) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 2> lhs_bits{0b01101010, 0b00000100};
    bit_region_type<uint_type const> lhs_region(11, lhs_bits.data());
    std::array<uint_type, 2> rhs_bits{0b10010101, 0b00000011};
    bit_region_type<uint_type const> rhs_region(11, rhs_bits.data());

    bool const result{bitwise_or_gives_all_ones<uint_type>(lhs_region,
     rhs_region)};

    EXPECT_TRUE(result);
}
*/

TEST(BitRegionUintOperators, SetLhsBitsAreNotSubsetOfSetRhsBits) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 2> lhs_bits{0b01101010, 0b00000101};
    bit_region_type<uint_type const> lhs_region(11, lhs_bits.data());
    std::array<uint_type, 2> rhs_bits{0b01101010, 0b00000100};
    bit_region_type<uint_type const> rhs_region(11, rhs_bits.data());

    bool const result{set_lhs_bits_are_subset_of_set_rhs_bits<uint_type>(
     lhs_region, rhs_region)};

    EXPECT_FALSE(result);
}

TEST(BitRegionUintOperators, SetLhsBitsAreSubsetOfSetRhsBits) {
    using uint_type = std::uint8_t;
    std::array<uint_type, 2> lhs_bits{0b01101010, 0b00000100};
    bit_region_type<uint_type const> lhs_region(11, lhs_bits.data());
    std::array<uint_type, 2> rhs_bits{0b01101010, 0b00000100};
    bit_region_type<uint_type const> rhs_region(11, rhs_bits.data());

    bool const result{set_lhs_bits_are_subset_of_set_rhs_bits<uint_type>(
     lhs_region, rhs_region)};

    EXPECT_TRUE(result);
}

/*
TEST(BitRegionUintOperators, CubeDistanceForNoBinaryVariables) {
    using uint_type = std::uint64_t;
    std::array<uint_type, 1> lhs_bits{UINT64_C(0b0'1)};
    bit_region_type<uint_type> lhs_region(2, lhs_bits.data());
    std::array<uint_type, 1> rhs_bits{UINT64_C(0b1'0)};
    bit_region_type<uint_type> rhs_region(2, rhs_bits.data());
    std::array<uint_type, 1> tmp_bits{};
    bit_region_type<uint_type> tmp_region(2, tmp_bits.data());

    std::size_t const result{cube_distance_for_binary_variables<uint_type>(0, 0,
     lhs_region, rhs_region, tmp_region)};

    EXPECT_EQ(0, result);
}

TEST(BitRegionUintOperators, CubeDistanceForOneBinaryVariableWithDistanceZero) {
    using uint_type = std::uint32_t;
    std::array<uint_type, 1> lhs_bits{UINT32_C(0b0'11)};
    bit_region_type<uint_type> lhs_region(3, lhs_bits.data());
    std::array<uint_type, 1> rhs_bits{UINT32_C(0b0'01)};
    bit_region_type<uint_type> rhs_region(3, rhs_bits.data());
    std::array<uint_type, 1> tmp_bits{};
    bit_region_type<uint_type> tmp_region(3, tmp_bits.data());

    std::size_t const result{cube_distance_for_binary_variables<uint_type>(0, 2,
     lhs_region, rhs_region, tmp_region)};

    EXPECT_EQ(0, result);
}

TEST(BitRegionUintOperators, CubeDistanceForOneBinaryVariableWithDistanceOne) {
    using uint_type = std::uint32_t;
    std::array<uint_type, 1> lhs_bits{UINT32_C(0b1'10)};
    bit_region_type<uint_type> lhs_region(3, lhs_bits.data());
    std::array<uint_type, 1> rhs_bits{UINT32_C(0b1'01)};
    bit_region_type<uint_type> rhs_region(3, rhs_bits.data());
    std::array<uint_type, 1> tmp_bits{};
    bit_region_type<uint_type> tmp_region(3, tmp_bits.data());

    std::size_t const result{cube_distance_for_binary_variables<uint_type>(0, 2,
     lhs_region, rhs_region, tmp_region)};

    EXPECT_EQ(1, result);
}

TEST(BitRegionUintOperators, CubeDistanceForManyBinaryVariablesTakingFullUint) {
    using uint_type = std::uint16_t;
    std::array<uint_type, 2> lhs_bits{
     UINT16_C(0b1001100101100110), UINT16_C(0b00100)};
    bit_region_type<uint_type> lhs_region(21, lhs_bits.data());
    std::array<uint_type, 2> rhs_bits{
     UINT16_C(0b0110100101101001), UINT16_C(0b00100)};
    bit_region_type<uint_type> rhs_region(21, rhs_bits.data());
    std::array<uint_type, 2> tmp_bits{};
    bit_region_type<uint_type> tmp_region(21, tmp_bits.data());

    std::size_t const result{cube_distance_for_binary_variables<uint_type>(0,
     16, lhs_region, rhs_region, tmp_region)};

    EXPECT_EQ(4, result);
}

TEST(BitRegionUintOperators,
 CubeDistanceForManyBinaryVariablesTakingPartialUint) {
    using uint_type = std::uint16_t;
    std::array<uint_type, 1> lhs_bits{UINT16_C(0b1001100101100110)};
    bit_region_type<uint_type> lhs_region(16, lhs_bits.data());
    std::array<uint_type, 1> rhs_bits{UINT16_C(0b0110100101101001)};
    bit_region_type<uint_type> rhs_region(16, rhs_bits.data());
    std::array<uint_type, 1> tmp_bits{};
    bit_region_type<uint_type> tmp_region(16, tmp_bits.data());

    std::size_t const result{cube_distance_for_binary_variables<uint_type>(0,
     14, lhs_region, rhs_region, tmp_region)};

    EXPECT_EQ(3, result);
}

TEST(BitRegionUintOperators, MarkBlockedBinaryVarRaises) {
    using uint_type = std::uint32_t;
    std::vector<bool> bvar_unraisable{
     0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
    std::vector<bool> bvar_might_be_freebie_raisable{
     1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1};
    std::array<uint_type, 2> expanding_cube_bits{
     0b10'10'01'01'10'01'10'10'01'11'10'01'11'01'01'10, 0b0'1'0'10'11'01};
    bit_region_type<uint_type> expanding_cube_main_region(41,
     expanding_cube_bits.data());
    std::array<uint_type, 2> blocking_cube_bits{
     0b10'01'01'11'10'01'10'11'11'10'10'01'01'01'01'10, 0b0'1'1'10'01'01};
    bit_region_type<uint_type> blocking_cube_main_region(41,
     blocking_cube_bits.data());
    std::array<uint_type, 2> work_buffer_bits{};
    bit_region_type<uint_type> work_buffer_main_region(41,
     work_buffer_bits.data());
    std::vector<bool> expected_new_bvar_unraisable{
     0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};

    mark_blocked_binary_var_raises(0, 38, bvar_might_be_freebie_raisable,
     bvar_unraisable, expanding_cube_main_region, blocking_cube_main_region,
     work_buffer_main_region);

    EXPECT_EQ(expected_new_bvar_unraisable, bvar_unraisable);
}
*/
