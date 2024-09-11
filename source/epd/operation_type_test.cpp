#include "epd/operation_type_test.hpp"


using namespace epd;


TEST(OperationType, EmptyString) {
    std::optional<operation_type> operation_opt{operation_type::create("")};
    ASSERT_FALSE(operation_opt);
}

TEST(OperationType, FullMoveNumberOne) {
    std::string const operation("fmvn 1;");
    std::optional<operation_type> operation_opt{
     operation_type::create(operation)};
    ASSERT_TRUE(operation_opt);
    ASSERT_EQ(operation, operation_opt->as_epd());
}


TEST(OperationType, GameTheoreticValueBlessedLoss) {
    std::string const operation("GTV b;");
    std::optional<operation_type> operation_opt{
     operation_type::create(operation)};
    ASSERT_TRUE(operation_opt);
    ASSERT_EQ(operation, operation_opt->as_epd());
}


TEST(OperationType, GameTheoreticValueCursedWin) {
    std::string const operation("GTV c;");
    std::optional<operation_type> operation_opt{
     operation_type::create(operation)};
    ASSERT_TRUE(operation_opt);
    ASSERT_EQ(operation, operation_opt->as_epd());
}


TEST(OperationType, GameTheoreticValueDraw) {
    std::string const operation("GTV d;");
    std::optional<operation_type> operation_opt{
     operation_type::create(operation)};
    ASSERT_TRUE(operation_opt);
    ASSERT_EQ(operation, operation_opt->as_epd());
}


TEST(OperationType, GameTheoreticValueLoss) {
    std::string const operation("GTV l;");
    std::optional<operation_type> operation_opt{
     operation_type::create(operation)};
    ASSERT_TRUE(operation_opt);
    ASSERT_EQ(operation, operation_opt->as_epd());
}


TEST(OperationType, GameTheoreticValueWin) {
    std::string const operation("GTV w;");
    std::optional<operation_type> operation_opt{
     operation_type::create(operation)};
    ASSERT_TRUE(operation_opt);
    ASSERT_EQ(operation, operation_opt->as_epd());
}


TEST(OperationType, HalfMoveCountZero) {
    std::string const operation("hmvc 0;");
    std::optional<operation_type> operation_opt{
     operation_type::create(operation)};
    ASSERT_TRUE(operation_opt);
    ASSERT_EQ(operation, operation_opt->as_epd());
}
