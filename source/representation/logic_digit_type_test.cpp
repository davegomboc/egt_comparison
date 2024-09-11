#include "representation/logic_digit_type_test.hpp"


#include <sstream>

#include "logic_digit_enum.hpp"


using representation::dc;
using representation::logic_digit_type;
using representation::off;
using representation::on;
using representation::sng;


TEST(LogicDigitType, ConstructFromReset) {
    EXPECT_EQ(off, logic_digit_type(logic_digit_enum::reset));
}

TEST(LogicDigitType, ConstructFromSet) {
    EXPECT_EQ(on, logic_digit_type(logic_digit_enum::set));
}

TEST(LogicDigitType, ConstructFromDoNotCare) {
    EXPECT_EQ(dc, logic_digit_type(logic_digit_enum::do_not_care));
}

TEST(LogicDigitType, ConstructFromFalse) {
    EXPECT_EQ(off, logic_digit_type(false));
}

TEST(LogicDigitType, ConstructFromTrue) {
    EXPECT_EQ(on, logic_digit_type(true));
}

TEST(LogicDigitType, CreateOptFromUnderscore) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('_'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(logic_digit_opt->singular());
}

TEST(LogicDigitType, CreateOptFromQuestionMark) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('?'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(logic_digit_opt->singular());
}

TEST(LogicDigitType, CreateOptFromTilde) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('~'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(logic_digit_opt->singular());
}

TEST(LogicDigitType, CreateOptFromZero) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('0'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_EQ(off, *logic_digit_opt);
}

TEST(LogicDigitType, CreateOptFromOne) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('1'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_EQ(on, *logic_digit_opt);
}

TEST(LogicDigitType, CreateOptFromTwo) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('2'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_EQ(dc, *logic_digit_opt);
}

TEST(LogicDigitType, CreateOptFromThree) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('3'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_EQ(off, *logic_digit_opt);
}

TEST(LogicDigitType, CreateOptFromFour) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('4'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_EQ(on, *logic_digit_opt);
}

TEST(LogicDigitType, CreateOptFromFive) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('5'));
    ASSERT_FALSE(logic_digit_opt.has_value());
}

TEST(LogicDigitType, CreateOptFromHyphen) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('-'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_EQ(dc, *logic_digit_opt);
}

TEST(LogicDigitType, CreateOptFromLowerCaseX) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('x'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_EQ(dc, *logic_digit_opt);
}

TEST(LogicDigitType, CreateOptFromUpperCaseX) {
    std::optional<logic_digit_type> logic_digit_opt(
     logic_digit_type::create_opt('X'));
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_EQ(dc, *logic_digit_opt);
}

TEST(LogicDigitType, SingularAsChar) {
    EXPECT_EQ(sng.as_char(), '_');
}

TEST(LogicDigitType, OffAsChar) {
    EXPECT_EQ(off.as_char(), '0');
}

TEST(LogicDigitType, OnAsChar) {
    EXPECT_EQ(on.as_char(), '1');
}

TEST(LogicDigitType, DoNotCareAsChar) {
    EXPECT_EQ(dc.as_char(), '-');
}

TEST(LogicDigitType, InserterWithSingular) {
    std::ostringstream out;
    out << sng;
    EXPECT_EQ("_", out.str());
}

TEST(LogicDigitType, InserterWithReset) {
    std::ostringstream out;
    out << off;
    EXPECT_EQ("0", out.str());
}

TEST(LogicDigitType, InserterWithSet) {
    std::ostringstream out;
    out << on;
    EXPECT_EQ("1", out.str());
}

TEST(LogicDigitType, InserterWithDoNotCare) {
    std::ostringstream out;
    out << dc;
    EXPECT_EQ("-", out.str());
}
