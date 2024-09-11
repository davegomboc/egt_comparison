#include "hex_display_test.hpp"


#include <vector>
#include <cstdint>


std::array<std::uint64_t, 2> constexpr test_data{
    0x0123456789abcdef, 0xdeadbeefcafe55aa
};
void const * const test_data_cbegin{test_data.data()};
void const * const test_data_cend{test_data.data() + test_data.size()};

std::uint64_t const * const uint64_test_data_cbegin{
 reinterpret_cast<std::uint64_t const *>(test_data_cbegin)};
std::uint64_t const * const uint64_test_data_cend{
 reinterpret_cast<std::uint64_t const *>(test_data_cend)};

std::uint32_t const * const uint32_test_data_cbegin{
 reinterpret_cast<std::uint32_t const *>(test_data_cbegin)};
std::uint32_t const * const uint32_test_data_cend{
 reinterpret_cast<std::uint32_t const *>(test_data_cend)};

std::uint16_t const * const uint16_test_data_cbegin{
 reinterpret_cast<std::uint16_t const *>(test_data_cbegin)};
std::uint16_t const * const uint16_test_data_cend{
 reinterpret_cast<std::uint16_t const *>(test_data_cend)};

std::uint8_t const * const uint8_test_data_cbegin{
 reinterpret_cast<std::uint8_t const *>(test_data_cbegin)};
std::uint8_t const * const uint8_test_data_cend{
 reinterpret_cast<std::uint8_t const *>(test_data_cend)};


TEST(HexDisplay, DebugPrintHexCharsWithUint8OfZero) {
    std::ostringstream out;
    print_hex<std::uint8_t>(out, 0);
    EXPECT_EQ(out.str(), "0x00");
}

TEST(HexDisplay, DebugPrintHexCharsWithUint16OfZero) {
    std::ostringstream out;
    print_hex<std::uint16_t>(out, 0);
    EXPECT_EQ(out.str(), "0x0000");
}

TEST(HexDisplay, DebugPrintHexCharsWithUint32OfZero) {
    std::ostringstream out;
    print_hex<std::uint32_t>(out, 0);
    EXPECT_EQ(out.str(), "0x00000000");
}

TEST(HexDisplay, DebugPrintHexCharsWithUint64OfZero) {
    std::ostringstream out;
    print_hex<std::uint64_t>(out, 0);
    EXPECT_EQ(out.str(), "0x0000000000000000");
}

TEST(HexDisplay, DebugPrintHexCharsWithEmptyUint8Vector) {
    std::ostringstream out;
    print_hex(out, uint8_test_data_cbegin,
     uint8_test_data_cbegin);
    EXPECT_TRUE(out.str().empty());
}

TEST(HexDisplay, DebugPrintHexCharsWithEmptyUint16Vector) {
    std::ostringstream out;
    print_hex(out, uint16_test_data_cbegin,
     uint16_test_data_cbegin);
    EXPECT_TRUE(out.str().empty());
}

TEST(HexDisplay, DebugPrintHexCharsWithEmptyUint32Vector) {
    std::ostringstream out;
    print_hex(out, uint32_test_data_cbegin,
     uint32_test_data_cbegin);
    EXPECT_TRUE(out.str().empty());
}

TEST(HexDisplay, DebugPrintHexCharsWithEmptyUint64Vector) {
    std::ostringstream out;
    print_hex(out, uint64_test_data_cbegin,
     uint64_test_data_cbegin);
    EXPECT_TRUE(out.str().empty());
}

TEST(HexDisplay, DebugPrintHexCharsWithUint8Vector) {
    std::ostringstream out;
    print_hex(out, uint8_test_data_cbegin,
     uint8_test_data_cend);
    EXPECT_EQ(out.str(), "0xef 0xcd 0xab 0x89 0x67 0x45 0x23 0x01"
     " 0xaa 0x55 0xfe 0xca 0xef 0xbe 0xad 0xde");
}

TEST(HexDisplay, DebugPrintHexCharsWithUint16Vector) {
    std::ostringstream out;
    print_hex(out, uint16_test_data_cbegin,
     uint16_test_data_cend);
    EXPECT_EQ(out.str(),
     "0xcdef 0x89ab 0x4567 0x0123 0x55aa 0xcafe 0xbeef 0xdead");
}

TEST(HexDisplay, DebugPrintHexCharsWithUint32Vector) {
    std::ostringstream out;
    print_hex(out, uint32_test_data_cbegin,
     uint32_test_data_cend);
    EXPECT_EQ(out.str(), "0x89abcdef 0x01234567 0xcafe55aa 0xdeadbeef");
}

TEST(HexDisplay, DebugPrintHexCharsWithUint64Vector) {
    std::ostringstream out;
    print_hex(out, uint64_test_data_cbegin,
     uint64_test_data_cend);
    EXPECT_EQ(out.str(), "0x0123456789abcdef 0xdeadbeefcafe55aa");
}

TEST(HexDisplay, DebugPrintHexCharsWithinUint8Vector) {
    std::ostringstream out;
    print_hex(out, std::next(uint8_test_data_cbegin),
     std::next(std::next(uint8_test_data_cbegin)));
    EXPECT_EQ(out.str(), "0xcd");
}

TEST(HexDisplay, DebugPrintHexCharsWithinUint16Vector) {
    std::ostringstream out;
    print_hex(out, std::next(uint16_test_data_cbegin),
     std::next(std::next(uint16_test_data_cbegin)));
    EXPECT_EQ(out.str(), "0x89ab");
}

TEST(HexDisplay, DebugPrintHexCharsWithinUint32Vector) {
    std::ostringstream out;
    print_hex(out, std::next(uint32_test_data_cbegin),
     std::next(std::next(uint32_test_data_cbegin)));
    EXPECT_EQ(out.str(), "0x01234567");
}

TEST(HexDisplay, DebugPrintHexCharsWithinUint64Vector) {
    std::ostringstream out;
    print_hex(out, std::next(uint64_test_data_cbegin),
     std::next(std::next(uint64_test_data_cbegin)));
    EXPECT_EQ(out.str(), "0xdeadbeefcafe55aa");
}
