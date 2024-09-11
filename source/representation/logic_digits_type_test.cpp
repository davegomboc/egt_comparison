#include "representation/logic_digits_type_test.hpp"


#include <cstdint>
#include <sstream>
#include <vector>


using representation::logic_digits_type;
using representation::sng;
using representation::off;
using representation::on;
using representation::dc;


TEST(LogicDigitsType, DefaultConstruction) {
    logic_digits_type logic_digits;
    EXPECT_TRUE(logic_digits.empty());
    EXPECT_EQ(logic_digits.size(), 0);
}

TEST(LogicDigitsType, EmptyBraceInitialization) {
    logic_digits_type logic_digits{};
    EXPECT_TRUE(logic_digits.empty());
    EXPECT_EQ(logic_digits.size(), 0);
}

TEST(LogicDigitsType, SingleElementBraceInitialization) {
    logic_digits_type logic_digits{dc};
    EXPECT_FALSE(logic_digits.empty());
    EXPECT_EQ(logic_digits.size(), 1);
    EXPECT_EQ(logic_digits[0], dc);
}

TEST(LogicDigitsType, MultipleElementBraceInitialization) {
    logic_digits_type logic_digits{dc, off, on, sng};
    EXPECT_FALSE(logic_digits.empty());
    EXPECT_EQ(logic_digits.size(), 4);
    EXPECT_EQ(logic_digits[0], dc);
    EXPECT_EQ(logic_digits[1], off);
    EXPECT_EQ(logic_digits[2], on);
    EXPECT_EQ(logic_digits[3], sng);
}

TEST(LogicDigitsType, ToStringOfZeroElementsIsEmpty) {
    logic_digits_type logic_digits;
    EXPECT_TRUE(logic_digits.to_string().empty());
}

TEST(LogicDigitsType, ToStringOfSevenElements) {
    logic_digits_type logic_digits{off, off, on, dc, on, dc, on};
    EXPECT_EQ(logic_digits.to_string(), "001-1-1");
}

TEST(LogicDigitsType, ToUint8VectorOfZeroElementsIsEmpty) {
    logic_digits_type logic_digits;
    EXPECT_TRUE(logic_digits.to_vector<std::uint8_t>().empty());
}

TEST(LogicDigitsType, FromUint8VectorOfZeroElementsIsEmpty) {
    logic_digits_type const logic_digits((std::vector<std::uint8_t>()));
    EXPECT_TRUE(logic_digits.empty());
}

TEST(LogicDigitsType, ToUint16VectorOfZeroElementsIsEmpty) {
    logic_digits_type logic_digits;
    EXPECT_TRUE(logic_digits.to_vector<std::uint16_t>().empty());
}

TEST(LogicDigitsType, FromUint16VectorOfZeroElementsIsEmpty) {
    logic_digits_type const logic_digits((std::vector<std::uint16_t>()));
    EXPECT_TRUE(logic_digits.empty());
}

TEST(LogicDigitsType, ToUint32VectorOfZeroElementsIsEmpty) {
    logic_digits_type logic_digits;
    EXPECT_TRUE(logic_digits.to_vector<std::uint32_t>().empty());
}

TEST(LogicDigitsType, FromUint32VectorOfZeroElementsIsEmpty) {
    logic_digits_type const logic_digits((std::vector<std::uint32_t>()));
    EXPECT_TRUE(logic_digits.empty());
}

TEST(LogicDigitsType, ToUint64VectorOfZeroElementsIsEmpty) {
    logic_digits_type logic_digits;
    EXPECT_TRUE(logic_digits.to_vector<std::uint64_t>().empty());
}

TEST(LogicDigitsType, FromUint64VectorOfZeroElementsIsEmpty) {
    logic_digits_type const logic_digits((std::vector<std::uint64_t>()));
    EXPECT_TRUE(logic_digits.empty());
}

TEST(LogicDigitsType, ToUint64VectorOfOneElement) {
    std::vector<std::uint64_t> const uint64_ts{
     logic_digits_type{off}.to_vector<std::uint64_t>()};
    ASSERT_EQ(uint64_ts.size(), 1);
    EXPECT_EQ(uint64_ts[0],
     static_cast<std::uint64_t>(logic_digit_enum::reset));
}

TEST(LogicDigitsType, ToUint64VectorOfTwoElements) {
    std::vector<std::uint64_t> const uint64_ts{
     logic_digits_type{on, dc}.to_vector<std::uint64_t>()};
    ASSERT_EQ(uint64_ts.size(), 1);
    std::uint64_t const expected_value{
     (static_cast<std::uint64_t>(logic_digit_enum::set)         << 0) |
     (static_cast<std::uint64_t>(logic_digit_enum::do_not_care) << 2)};
    EXPECT_EQ(uint64_ts[0], expected_value);
    EXPECT_EQ(uint64_ts[0], UINT64_C(0b11'10));
}

TEST(LogicDigitsType, ToUint64VectorOfThreeElements) {
    std::vector<std::uint64_t> const uint64_ts{
     logic_digits_type{on, off, dc}.to_vector<std::uint64_t>()};
    ASSERT_EQ(uint64_ts.size(), 1);
    std::uint64_t const expected_value{
     (static_cast<std::uint64_t>(logic_digit_enum::set)         << 0) |
     (static_cast<std::uint64_t>(logic_digit_enum::reset)       << 2) |
     (static_cast<std::uint64_t>(logic_digit_enum::do_not_care) << 4)};
    EXPECT_EQ(uint64_ts[0], expected_value);
    EXPECT_EQ(uint64_ts[0], UINT64_C(0b11'01'10));
}

TEST(LogicDigitsType, ToUint8VectorOfSevenElements) {
    std::vector<std::uint8_t> const uint8_ts{logic_digits_type{
     off, off, on, dc, on, dc, on}.to_vector<std::uint8_t>()};
    ASSERT_EQ(uint8_ts.size(), 2);
    EXPECT_EQ(uint8_ts[0], UINT8_C(0b11'10'01'01));
    EXPECT_EQ(uint8_ts[1], UINT8_C(0b10'11'10));
}

TEST(LogicDigitsType, FromUint8VectorOfSevenElements) {
    std::vector<std::uint8_t> const uint8_ts{
     UINT8_C(0b11'10'01'01), UINT8_C(0b10'11'10)};
    logic_digits_type const logic_digits(uint8_ts);
    EXPECT_EQ((logic_digits_type{off, off, on, dc, on, dc, on, sng}),
     logic_digits);
}

TEST(LogicDigitsType, ToUint64VectorTsOfSevenElements) {
    std::vector<std::uint64_t> const uint64_ts{logic_digits_type{
     off, off, on, dc, on, dc, on}.to_vector<std::uint64_t>()};
    ASSERT_EQ(uint64_ts.size(), 1);
    EXPECT_EQ(uint64_ts[0], UINT64_C(0b10'11'10'11'10'01'01));
}

TEST(LogicDigitsType, FromUint64VectorOfSevenElements) {
    std::vector<std::uint64_t> const uint64_ts{
     UINT64_C(0b10'11'10'11'10'01'01)};
    logic_digits_type const logic_digits(uint64_ts);
    EXPECT_EQ((logic_digits_type{
     off, off,  on,  dc,  on,  dc,  on, sng,
     sng, sng, sng, sng, sng, sng, sng, sng,
     sng, sng, sng, sng, sng, sng, sng, sng,
     sng, sng, sng, sng, sng, sng, sng, sng}), logic_digits);
}

TEST(LogicDigitsType, ToUint8VectorOfFortyElements) {
    logic_digits_type logic_digits;
    for (std::size_t i{}; i != 16; ++i) {
        logic_digits.push_back(dc);
    }
    for (std::size_t i{}; i != 16; ++i) {
        logic_digits.push_back(off);
    }
    for (std::size_t i{}; i != 8; ++i) {
        logic_digits.push_back(on);
    }
    ASSERT_EQ(logic_digits.size(), 40);
    std::vector<std::uint8_t> const uint8_ts{
     logic_digits.to_vector<std::uint8_t>()};
    ASSERT_EQ(uint8_ts.size(), 10);
    for (std::size_t i{}; i != 4; ++i) {
        EXPECT_EQ(uint8_ts[i], UINT8_C(0b11111111));
    }
    for (std::size_t i{4}; i != 8; ++i) {
        EXPECT_EQ(uint8_ts[i], UINT8_C(0b01010101));
    }
    for (std::size_t i{8}; i != 10; ++i) {
        EXPECT_EQ(uint8_ts[i], UINT8_C(0b10101010));
    }
}

TEST(LogicDigitsType, FromUint8VectorOfFortyElements) {
    std::vector<std::uint8_t> const uint8_ts{
     UINT8_C(0b11111111), UINT8_C(0b11111111),
     UINT8_C(0b11111111), UINT8_C(0b11111111),
     UINT8_C(0b01010101), UINT8_C(0b01010101),
     UINT8_C(0b01010101), UINT8_C(0b01010101),
     UINT8_C(0b10101010), UINT8_C(0b10101010)};
    logic_digits_type const logic_digits(uint8_ts);
    EXPECT_EQ((logic_digits_type{
      dc,  dc,  dc,  dc,  dc,  dc,  dc,  dc,
      dc,  dc,  dc,  dc,  dc,  dc,  dc,  dc,
     off, off, off, off, off, off, off, off,
     off, off, off, off, off, off, off, off,
      on,  on,  on,  on,  on,  on,  on,  on}), logic_digits);
}

TEST(LogicDigitsType, ToUint64VectorOfFortyElements) {
    logic_digits_type logic_digits;
    for (std::size_t i{}; i != 16; ++i) {
        logic_digits.push_back(dc);
    }
    for (std::size_t i{}; i != 16; ++i) {
        logic_digits.push_back(off);
    }
    for (std::size_t i{}; i != 8; ++i) {
        logic_digits.push_back(on);
    }
    ASSERT_EQ(logic_digits.size(), 40);
    std::vector<std::uint64_t> const uint64_ts{
     logic_digits.to_vector<std::uint64_t>()};
    ASSERT_EQ(uint64_ts.size(), 2);
    EXPECT_EQ(uint64_ts[0], UINT64_C(
     0b0101010101010101010101010101010111111111111111111111111111111111));
    EXPECT_EQ(uint64_ts[1], UINT64_C(0b1010101010101010));
}

TEST(LogicDigitsType, FromUint64VectorOfFortyElements) {
    std::vector<std::uint64_t> const uint64_ts{UINT64_C(
     0b0101010101010101010101010101010111111111111111111111111111111111),
     UINT64_C(0b1010101010101010)};
    logic_digits_type const logic_digits(uint64_ts);
    EXPECT_EQ((logic_digits_type{
      dc,  dc,  dc,  dc,  dc,  dc,  dc,  dc,
      dc,  dc,  dc,  dc,  dc,  dc,  dc,  dc,
     off, off, off, off, off, off, off, off,
     off, off, off, off, off, off, off, off,
      on,  on,  on,  on,  on,  on,  on,  on,
     sng, sng, sng, sng, sng, sng, sng, sng,
     sng, sng, sng, sng, sng, sng, sng, sng,
     sng, sng, sng, sng, sng, sng, sng, sng}), logic_digits);
}

TEST(LogicDigitsType, Inserter) {
    std::ostringstream out;
    out << logic_digits_type{on, off, off, dc, dc, on, on};
    EXPECT_EQ("1 00--11", out.str());
}

TEST(LogicDigitsType, EmitAsUint64Vector) {
    logic_digits_type wtm_Ka8_kc8_F{
     off,
     off, off, off, off, off, off,
     off, off, off, off, on, off,
     off, on, off};
    std::vector<std::uint64_t> vec{wtm_Ka8_kc8_F.to_vector<std::uint64_t>()};
    ASSERT_EQ(1, vec.size());
    EXPECT_EQ(UINT64_C(0b011001'011001010101'010101010101'01), vec[0]);
}
