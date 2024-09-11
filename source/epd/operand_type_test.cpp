#include "epd/operand_type_test.hpp"


using namespace epd;


TEST(OperandType, ChessClock) {
    std::string const operand("003:23:59:02");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, ChessMove) {
    std::string const operand("cxd5");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, Date) {
    std::string const operand("2023.05.12");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, EmptyString) {
    std::string const operand("");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, ExplicitlyPositiveInteger) {
    std::string const operand("+5");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, Identifier) {
    std::string const operand("identifier");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, MaximumRequiredInteger) {
    std::string const operand("2147483647");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, MinimumRequiredInteger) {
    std::string const operand("-2147483648");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, QuotedStringContainingNonprintingCharacter) {
    std::string const operand("\"\a\"");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_FALSE(operand_opt);
}


TEST(OperandType, QuotedStringContainingOnlyWhitespace) {
    std::string const operand("\"    \"");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, QuotedStringContainingWords) {
    std::string const operand("\"checkmate stalemate\"");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, TimeOfDay) {
    std::string const operand("00:47:30");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_TRUE(operand_opt);
    ASSERT_EQ(operand, operand_opt->as_epd());
}


TEST(OperandType, UnquotedStringContainingWhitespace) {
    std::string const operand("not allowed");
    std::optional<operand_type> operand_opt{operand_type::create(operand)};
    ASSERT_FALSE(operand_opt);
}
