#include "logic_digit_enum_test.hpp"


#include <sstream>


TEST(LogicDigitEnum, ZeroIsSingular) {
    EXPECT_TRUE(singular(static_cast<logic_digit_enum>(0)));
}

TEST(LogicDigitEnum, OneIsNonsingular) {
    EXPECT_FALSE(singular(static_cast<logic_digit_enum>(1)));
}

TEST(LogicDigitEnum, Positive255IsNonsingular) {
    EXPECT_FALSE(singular(static_cast<logic_digit_enum>(255)));
}

TEST(LogicDigitEnum, ZeroIsValid) {
    EXPECT_TRUE(valid(static_cast<logic_digit_enum>(0)));
}

TEST(LogicDigitEnum, OneIsValid) {
    EXPECT_TRUE(valid(static_cast<logic_digit_enum>(1)));
}

TEST(LogicDigitEnum, TwoIsValid) {
    EXPECT_TRUE(valid(static_cast<logic_digit_enum>(2)));
}

TEST(LogicDigitEnum, ThreeIsValid) {
    EXPECT_TRUE(valid(static_cast<logic_digit_enum>(3)));
}

TEST(LogicDigitEnum, FourIsInvalid) {
    EXPECT_FALSE(valid(static_cast<logic_digit_enum>(4)));
}

TEST(LogicDigitEnum, Positive255IsInvalid) {
    EXPECT_FALSE(valid(static_cast<logic_digit_enum>(255)));
}

TEST(LogicDigitEnum, ZeroIsInvalidOrSingular) {
    EXPECT_FALSE(valid_and_nonsingular(static_cast<logic_digit_enum>(0)));
}

TEST(LogicDigitEnum, OneIsValidAndNonsingular) {
    EXPECT_TRUE(valid_and_nonsingular(static_cast<logic_digit_enum>(1)));
}

TEST(LogicDigitEnum, TwoIsValidAndNonsingular) {
    EXPECT_TRUE(valid_and_nonsingular(static_cast<logic_digit_enum>(2)));
}

TEST(LogicDigitEnum, ThreeIsValidAndNonsingular) {
    EXPECT_TRUE(valid_and_nonsingular(static_cast<logic_digit_enum>(3)));
}

TEST(LogicDigitEnum, FourIsInvalidOrSingular) {
    EXPECT_FALSE(valid_and_nonsingular(static_cast<logic_digit_enum>(4)));
}

TEST(LogicDigitEnum, Positive255IsInvalidOrSingular) {
    EXPECT_FALSE(valid_and_nonsingular(static_cast<logic_digit_enum>(255)));
}

TEST(LogicDigitEnum, SingularAsChar) {
    EXPECT_EQ(as_char(logic_digit_enum::singular), '_');
}

TEST(LogicDigitEnum, ResetAsChar) {
    EXPECT_EQ(as_char(logic_digit_enum::reset), '0');
}

TEST(LogicDigitEnum, SetAsChar) {
    EXPECT_EQ(as_char(logic_digit_enum::set), '1');
}

TEST(LogicDigitEnum, DoNotCareAsChar) {
    EXPECT_EQ(as_char(logic_digit_enum::do_not_care), '-');
}

TEST(LogicDigitEnum, InserterWithSingular) {
    std::ostringstream out;
    out << logic_digit_enum::singular;
    EXPECT_EQ("_", out.str());
}

TEST(LogicDigitEnum, InserterWithReset) {
    std::ostringstream out;
    out << logic_digit_enum::reset;
    EXPECT_EQ("0", out.str());
}

TEST(LogicDigitEnum, InserterWithSet) {
    std::ostringstream out;
    out << logic_digit_enum::set;
    EXPECT_EQ("1", out.str());
}

TEST(LogicDigitEnum, InserterWithDoNotCare) {
    std::ostringstream out;
    out << logic_digit_enum::do_not_care;
    EXPECT_EQ("-", out.str());
}

TEST(LogicDigitEnum, FiveCharacterIsNotInterpreted) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('5')};
    EXPECT_FALSE(logic_digit_opt.has_value());
}

TEST(LogicDigitEnum, NullCharacterIsNotInterpreted) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('\0')};
    EXPECT_FALSE(logic_digit_opt.has_value());
}

TEST(LogicDigitEnum, LowerCaseQCharacterIsNotInterpreted) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('q')};
    EXPECT_FALSE(logic_digit_opt.has_value());
}

TEST(LogicDigitEnum, UnderscoreCharacterIsInterpretedAsSingular) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('_')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::singular);
}

TEST(LogicDigitEnum, QuestionMarkCharacterIsInterpretedAsSingular) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('?')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::singular);
}

TEST(LogicDigitEnum, TildeCharacterIsInterpretedAsSingular) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('~')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::singular);
}

TEST(LogicDigitEnum, ZeroCharacterIsInterpretedAsReset) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('0')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::reset);
}

TEST(LogicDigitEnum, ThreeCharacterIsInterpretedAsReset) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('3')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::reset);
}

TEST(LogicDigitEnum, OneCharacterIsInterpretedAsSet) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('1')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::set);
}

TEST(LogicDigitEnum, FourCharacterIsInterpretedAsSet) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('4')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::set);
}

TEST(LogicDigitEnum, TwoCharacterIsInterpretedAsDoNotCare) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('2')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::do_not_care);
}

TEST(LogicDigitEnum, MinusCharacterIsInterpretedAsDoNotCare) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('-')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::do_not_care);
}

TEST(LogicDigitEnum, LowerCaseXCharacterIsInterpretedAsDoNotCare) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('x')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::do_not_care);
}

TEST(LogicDigitEnum, UpperCaseXCharacterIsInterpretedAsDoNotCare) {
    std::optional<logic_digit_enum> logic_digit_opt{
     as_logic_digit_enum_opt('X')};
    ASSERT_TRUE(logic_digit_opt.has_value());
    EXPECT_TRUE(*logic_digit_opt == logic_digit_enum::do_not_care);
}

TEST(LogicDigitEnum, FlippedOfSingularYieldsSingular) {
    EXPECT_EQ(logic_digit_enum::singular, flipped(logic_digit_enum::singular));
}

TEST(LogicDigitEnum, FlippedOfResetYieldsSet) {
    EXPECT_EQ(logic_digit_enum::set, flipped(logic_digit_enum::reset));
}

TEST(LogicDigitEnum, FlippedOfSetYieldsReset) {
    EXPECT_EQ(logic_digit_enum::reset, flipped(logic_digit_enum::set));
}

TEST(LogicDigitEnum, FlippedOfDoNotCareYieldsDoNotCare) {
    EXPECT_EQ(logic_digit_enum::do_not_care,
     flipped(logic_digit_enum::do_not_care));
}
