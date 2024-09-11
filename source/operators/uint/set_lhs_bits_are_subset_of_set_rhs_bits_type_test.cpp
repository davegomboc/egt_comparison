#include "operators/uint/set_lhs_bits_are_subset_of_set_rhs_bits_type_test.hpp"


TEST(SetLhsBitsAreSubsetOfSetRhsBitsType, ApplyOnFullUintFalseCase) {
    std::array<unsigned char, 2> lhs_bits{0b01111010, 0b00000101};
    bit_region_type<unsigned char> lhs_region(11, lhs_bits.data());
    std::array<unsigned char, 2> rhs_bits{0b10010101, 0b00000010};
    bit_region_type<unsigned char> rhs_region(11, rhs_bits.data());

    set_lhs_bits_are_subset_of_set_rhs_bits_type<unsigned char>
     set_lhs_bits_are_subset_of_set_rhs_bits;
    bool result{set_lhs_bits_are_subset_of_set_rhs_bits.apply_on_full_uint(
     lhs_region.uint_ref(0), rhs_region.uint_ref(0))};
    EXPECT_FALSE(result);
}

TEST(SetLhsBitsAreSubsetOfSetRhsBitsType, ApplyOnFullUintTrueCase) {
    std::array<unsigned char, 2> lhs_bits{0b01101010, 0b00000101};
    bit_region_type<unsigned char> lhs_region(11, lhs_bits.data());
    std::array<unsigned char, 2> rhs_bits{0b01111010, 0b00000010};
    bit_region_type<unsigned char const> rhs_region(11, rhs_bits.data());

    set_lhs_bits_are_subset_of_set_rhs_bits_type<unsigned char>
     set_lhs_bits_are_subset_of_set_rhs_bits;
    auto const lhs_value{lhs_region.uint_ref(0)};
    auto const rhs_value{rhs_region.uint_ref(0)};
    bool result{set_lhs_bits_are_subset_of_set_rhs_bits.apply_on_full_uint(
     lhs_value, rhs_value)};
    EXPECT_TRUE(result);
}

TEST(SetLhsBitsAreSubsetOfSetRhsBitsType, ApplyOnPartialUintFalseCase) {
    std::array<unsigned char, 2> lhs_bits{0b01101010, 0b00000110};
    bit_region_type<unsigned char const> lhs_region(11, lhs_bits.data());
    std::array<unsigned char, 2> rhs_bits{0b10010101, 0b00000010};
    bit_region_type<unsigned char> rhs_region(11, rhs_bits.data());

    set_lhs_bits_are_subset_of_set_rhs_bits_type<unsigned char>
     set_lhs_bits_are_subset_of_set_rhs_bits;
    bool result{set_lhs_bits_are_subset_of_set_rhs_bits.apply_on_partial_uint(
     0b111, lhs_region.uint_ref(1), rhs_region.uint_ref(1))};
    EXPECT_FALSE(result);
}

TEST(SetLhsBitsAreSubsetOfSetRhsBitsType, ApplyOnPartialUintTrueCase) {
    std::array<unsigned char, 2> lhs_bits{0b01101010, 0b00000101};
    bit_region_type<unsigned char const> lhs_region(11, lhs_bits.data());
    std::array<unsigned char, 2> rhs_bits{0b10000101, 0b00000111};
    bit_region_type<unsigned char const> rhs_region(11, rhs_bits.data());

    set_lhs_bits_are_subset_of_set_rhs_bits_type<unsigned char>
     set_lhs_bits_are_subset_of_set_rhs_bits;
    bool result{set_lhs_bits_are_subset_of_set_rhs_bits.apply_on_partial_uint(
     0b111, lhs_region.uint_ref(1), rhs_region.uint_ref(1))};
    EXPECT_TRUE(result);
}
