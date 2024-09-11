#include "bit_array_display_test.hpp"


#include <iostream>


TEST(BitArrayDisplay, EmitWtmKa8kh1Bits) {
    std::array<std::uint32_t, 1> bits{0b101010101010'010101010101'01};
    std::ostringstream out;
    debug_print_bit_pairs(out, bits.data(), 0, 26, bit_grouping_enum::normal);
    EXPECT_EQ("\x1B[37;40m0 000000 111111", out.str());
}
