#include "cube_binary_inputs_test.hpp"

#include <array>
#include <cstdint>
#include <random>
#include <sstream>
#include <string>

#include "logic_digit_enum.hpp"


TEST(CubeBinaryInputs, Inserter19) {
    std::uint64_t const cube_value{92783928745};
    cube_binary_inputs_tt<19> const binary_inputs(cube_value);
    std::ostringstream out;
    out << binary_inputs;
    std::string const generated_string{out.str()};
    EXPECT_EQ("0 111001 011001 101000", generated_string);
}


TEST(CubeBinaryInputs, Inserter32) {
    std::uint64_t const cube_value{92783928745};
    cube_binary_inputs_tt<32> const binary_inputs(cube_value);
    std::ostringstream out;
    out << binary_inputs;
    std::string const generated_string{out.str()};
    EXPECT_EQ("0 111001 011001 101000 ______ ______ _", generated_string);
}


TEST(CubeBinaryInputs, InitializeFromLogicDigitEnumsMostSignificantFirst) {
    auto const binary_inputs{cube_binary_inputs_tt<13>({
     lde_off, lde_off, lde_off, lde_off, lde_off, lde_on, lde_on, lde_on,
     lde_on, lde_on, lde_on, lde_on, lde_on}, false)};

    EXPECT_EQ(binary_inputs[12], lde_off);
    EXPECT_EQ(binary_inputs[11], lde_off);
    EXPECT_EQ(binary_inputs[10], lde_off);
    EXPECT_EQ(binary_inputs[ 9], lde_off);
    EXPECT_EQ(binary_inputs[ 8], lde_off);
    EXPECT_EQ(binary_inputs[ 7], lde_on);
    EXPECT_EQ(binary_inputs[ 6], lde_on);
    EXPECT_EQ(binary_inputs[ 5], lde_on);
    EXPECT_EQ(binary_inputs[ 4], lde_on);
    EXPECT_EQ(binary_inputs[ 3], lde_on);
    EXPECT_EQ(binary_inputs[ 2], lde_on);
    EXPECT_EQ(binary_inputs[ 1], lde_on);
    EXPECT_EQ(binary_inputs[ 0], lde_on);
}


TEST(CubeBinaryInputs, InitializeFromLogicDigitEnumsLeastSignificantFirst) {
    auto const binary_inputs{cube_binary_inputs_tt<13>({
     lde_off, lde_off, lde_off, lde_off, lde_off, lde_on, lde_on, lde_on,
     lde_on, lde_on, lde_on, lde_on, lde_on}, true)};

    EXPECT_EQ(binary_inputs[ 0], lde_off);
    EXPECT_EQ(binary_inputs[ 1], lde_off);
    EXPECT_EQ(binary_inputs[ 2], lde_off);
    EXPECT_EQ(binary_inputs[ 3], lde_off);
    EXPECT_EQ(binary_inputs[ 4], lde_off);
    EXPECT_EQ(binary_inputs[ 5], lde_on);
    EXPECT_EQ(binary_inputs[ 6], lde_on);
    EXPECT_EQ(binary_inputs[ 7], lde_on);
    EXPECT_EQ(binary_inputs[ 8], lde_on);
    EXPECT_EQ(binary_inputs[ 9], lde_on);
    EXPECT_EQ(binary_inputs[10], lde_on);
    EXPECT_EQ(binary_inputs[11], lde_on);
    EXPECT_EQ(binary_inputs[12], lde_on);
}


TEST(CubeBinaryInputs, GeneratedOrderingExcludesDoNotCareAndSingular) {
    auto const binary_inputs{cube_binary_inputs_tt<8>({
     lde_off, lde_sng, lde_on, lde_sng, lde_dc, lde_off, lde_on, lde_dc},
     false)};

    std::mt19937_64 generator;
    auto const ordering{
     binary_inputs.generate_raise_attempt_ordering(generator)};
    EXPECT_EQ(4, ordering.size());
    EXPECT_NE(lde_dc, binary_inputs[ordering[0]]);
    EXPECT_NE(lde_sng, binary_inputs[ordering[0]]);
    EXPECT_NE(lde_dc, binary_inputs[ordering[1]]);
    EXPECT_NE(lde_sng, binary_inputs[ordering[1]]);
    EXPECT_NE(lde_dc, binary_inputs[ordering[2]]);
    EXPECT_NE(lde_sng, binary_inputs[ordering[2]]);
    EXPECT_NE(lde_dc, binary_inputs[ordering[3]]);
    EXPECT_NE(lde_sng, binary_inputs[ordering[3]]);
}
