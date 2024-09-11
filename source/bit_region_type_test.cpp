#include "bit_region_type_test.hpp"


#include <array>
#include <bitset>
#include <cstdint>


TEST(BitRegionType, ComparisonOfTwoEmptyRegions) {
    std::array<unsigned int, 0> a;
    bit_region_type region_a(0, a.data());
    std::array<unsigned int, 0> b;
    bit_region_type region_b(0, b.data());

    EXPECT_TRUE(region_a == region_b);
    EXPECT_FALSE(region_a != region_b);
    EXPECT_FALSE(region_a < region_b);
    EXPECT_TRUE(region_a <= region_b);
    EXPECT_FALSE(region_a > region_b);
    EXPECT_TRUE(region_a >= region_b);
}

TEST(BitRegionType, ComparisonOfEmptyVersus0) {
    std::array<unsigned int, 0> a;
    bit_region_type region_a(0, a.data());
    std::array<unsigned int, 1> b{0b1};
    bit_region_type region_b(1, b.data());

    EXPECT_FALSE(region_a == region_b);
    EXPECT_TRUE(region_a != region_b);
    EXPECT_TRUE(region_a < region_b);
    EXPECT_TRUE(region_a <= region_b);
    EXPECT_FALSE(region_a > region_b);
    EXPECT_FALSE(region_a >= region_b);
}

TEST(BitRegionType, ComparisonOf0Versus0) {
    std::array<unsigned int, 1> a{};
    bit_region_type region_a(1, a.data());
    std::array<unsigned int, 1> b{};
    bit_region_type region_b(1, b.data());

    EXPECT_TRUE(region_a == region_b);
    EXPECT_FALSE(region_a != region_b);
    EXPECT_FALSE(region_a < region_b);
    EXPECT_TRUE(region_a <= region_b);
    EXPECT_FALSE(region_a > region_b);
    EXPECT_TRUE(region_a >= region_b);
}

TEST(BitRegionType, ComparisonOf1Versus1) {
    std::array<unsigned int, 1> a{0b0};
    bit_region_type region_a(1, a.data());
    std::array<unsigned int, 1> b{0b0};
    bit_region_type region_b(1, b.data());

    EXPECT_TRUE(region_a == region_b);
    EXPECT_FALSE(region_a != region_b);
    EXPECT_FALSE(region_a < region_b);
    EXPECT_TRUE(region_a <= region_b);
    EXPECT_FALSE(region_a > region_b);
    EXPECT_TRUE(region_a >= region_b);
}

TEST(BitRegionType, ComparisonOf0Versus1) {
    std::array<unsigned int, 1> a{0b0};
    bit_region_type region_a(1, a.data());
    std::array<unsigned int, 1> b{0b1};
    bit_region_type region_b(1, b.data());

    EXPECT_FALSE(region_a == region_b);
    EXPECT_TRUE(region_a != region_b);
    EXPECT_TRUE(region_a < region_b);
    EXPECT_TRUE(region_a <= region_b);
    EXPECT_FALSE(region_a > region_b);
    EXPECT_FALSE(region_a >= region_b);
}

TEST(BitRegionType, ComparisonOf32BitsUsing0Versus0) {
    std::array<unsigned int, 1> a{};
    bit_region_type region_a(32, a.data());
    std::array<unsigned int, 1> b{};
    bit_region_type region_b(32, b.data());

    EXPECT_TRUE(region_a == region_b);
    EXPECT_FALSE(region_a != region_b);
    EXPECT_FALSE(region_a < region_b);
    EXPECT_TRUE(region_a <= region_b);
    EXPECT_FALSE(region_a > region_b);
    EXPECT_TRUE(region_a >= region_b);
}

TEST(BitRegionType, ComparisonOf63BitsUsing0x7fffffffVersus0xffffffff) {
    std::array<unsigned int, 2> a{0xffffffff, 0x7fffffff};
    bit_region_type region_a(63, a.data());
    std::array<unsigned int, 2> b{0xffffffff, 0xffffffff};
    bit_region_type region_b(63, b.data());

    EXPECT_TRUE(region_a == region_b);
    EXPECT_FALSE(region_a != region_b);
    EXPECT_FALSE(region_a < region_b);
    EXPECT_TRUE(region_a <= region_b);
    EXPECT_FALSE(region_a > region_b);
    EXPECT_TRUE(region_a >= region_b);
}

TEST(BitRegionType, ComparisonOf64BitsUsing0x7fffffffVersus0xffffffff) {
    std::array<unsigned int, 2> a = {0xffffffff, 0x7fffffff};
    bit_region_type region_a(64, a.data());
    std::array<unsigned int, 2> b = {0xffffffff, 0xffffffff};
    bit_region_type region_b(64, b.data());

    EXPECT_FALSE(region_a == region_b);
    EXPECT_TRUE(region_a != region_b);
    EXPECT_TRUE(region_a < region_b);
    EXPECT_TRUE(region_a <= region_b);
    EXPECT_FALSE(region_a > region_b);
    EXPECT_FALSE(region_a >= region_b);
}

TEST(BitRegionType, IndicesOfNoSetBits) {
    std::array<std::uint8_t, 5> bits{0, 0, 0, 0, 0};
    bit_region_type region(bits.size() * bits_per<decltype(bits)::value_type>,
     bits.data());
    std::vector<std::size_t> the_set_bits{region.indices_of_set_bits()};
    EXPECT_TRUE(the_set_bits.empty());
}

TEST(BitRegionType, IndexOfSingleSetBitWhenFirst) {
    std::array<std::uint8_t, 5> bits{0b00000001, 0, 0, 0, 0};
    bit_region_type region(bits.size() * bits_per<decltype(bits)::value_type>,
     bits.data());
    std::vector<std::size_t> the_set_bits{region.indices_of_set_bits()};
    EXPECT_EQ(1, the_set_bits.size());
    EXPECT_EQ(0, the_set_bits[0]);
}

TEST(BitRegionType, IndexOfSingleSetBitWhenLast) {
    std::array<std::uint8_t, 5> bits{0, 0, 0, 0, 0b10000000};
    bit_region_type region(bits.size() * bits_per<decltype(bits)::value_type>,
     bits.data());
    std::vector<std::size_t> the_set_bits{region.indices_of_set_bits()};
    EXPECT_EQ(1, the_set_bits.size());
    EXPECT_EQ(39, the_set_bits[0]);
}

TEST(BitRegionType, IndicesOfAllSetBits) {
    std::array<std::uint8_t, 5> bits{0xff, 0xff, 0xff, 0xff, 0xff};
    bit_region_type region(bits.size() * bits_per<decltype(bits)::value_type>,
     bits.data());
    std::vector<std::size_t> the_set_bits{region.indices_of_set_bits()};
    EXPECT_EQ(40, the_set_bits.size());
    for (std::size_t bit_index{}; bit_index != 40; ++bit_index) {
        EXPECT_NE(the_set_bits.end(),
         std::find(the_set_bits.cbegin(), the_set_bits.cend(), bit_index));
    }
}
