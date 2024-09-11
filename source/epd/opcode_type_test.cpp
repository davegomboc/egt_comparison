#include "epd/opcode_type_test.hpp"


using namespace epd;


TEST(OpcodeType, DigitFollowedByLetters) {
    std::string const opcode("1abc");
    std::optional<opcode_type> opcode_opt{opcode_type::create(opcode)};
    ASSERT_FALSE(opcode_opt);
}


TEST(OpcodeType, EmptyString) {
    std::string const opcode("");
    std::optional<opcode_type> opcode_opt{opcode_type::create(opcode)};
    ASSERT_FALSE(opcode_opt);
}


TEST(OpcodeType, FullMoveNumber) {
    std::string const opcode("fmvn");
    std::optional<opcode_type> opcode_opt{opcode_type::create(opcode)};
    ASSERT_TRUE(opcode_opt);
    ASSERT_EQ(opcode, opcode_opt->as_epd());
}


TEST(OpcodeType, GameTheoreticValue) {
    std::string const opcode("GTV");
    std::optional<opcode_type> opcode_opt{opcode_type::create(opcode)};
    ASSERT_TRUE(opcode_opt);
    ASSERT_EQ(opcode, opcode_opt->as_epd());
}


TEST(OpcodeType, HalfMoveCount) {
    std::string const opcode("hmvc");
    std::optional<opcode_type> opcode_opt{opcode_type::create(opcode)};
    ASSERT_TRUE(opcode_opt);
    ASSERT_EQ(opcode, opcode_opt->as_epd());
}


TEST(OpcodeType, LetterFollowedByDigits) {
    std::string const opcode("a123");
    std::optional<opcode_type> opcode_opt{opcode_type::create(opcode)};
    ASSERT_TRUE(opcode_opt);
    ASSERT_EQ(opcode, opcode_opt->as_epd());
}
