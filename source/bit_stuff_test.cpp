#include "bit_stuff_test.hpp"

#include <cstdint>


TEST(BitStuff, Uint8OneToBinaryString) {
    std::uint8_t const value{1};
    std::string expected(8, '0');
    expected[8 - 1] = '1';
    std::string const str{to_binary_string(value)};
    EXPECT_EQ(str, expected);
}

TEST(BitStuff, Uint16OneToBinaryString) {
    std::uint16_t const value{1};
    std::string expected(16, '0');
    expected[16 - 1] = '1';
    std::string const str{to_binary_string(value)};
    EXPECT_EQ(str, expected);
}

TEST(BitStuff, Uint32OneToBinaryString) {
    std::uint32_t const value{1};
    std::string expected(32, '0');
    expected[32 - 1] = '1';
    std::string const str{to_binary_string(value)};
    EXPECT_EQ(str, expected);
}

TEST(BitStuff, Uint64OneToBinaryString) {
    std::uint64_t const value{1};
    std::string expected(64, '0');
    expected[64 - 1] = '1';
    std::string const str{to_binary_string(value)};
    EXPECT_EQ(str, expected);
}
