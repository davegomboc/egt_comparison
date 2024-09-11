#include "epd/operations_type_test.hpp"


using namespace epd;


TEST(OperationsType, EmptyString) {
    std::string const operations("");
    std::optional<operations_type> operations_opt{
     operations_type::create(operations)};
    ASSERT_TRUE(operations_opt);
    std::optional<std::string> fen_opt{operations_opt->as_fen_opt()};

    ASSERT_TRUE(operations_opt->empty());
    ASSERT_EQ(0, operations_opt->size());
    ASSERT_EQ(operations, operations_opt->as_epd());
    ASSERT_FALSE(operations_opt->as_fen_opt());
}


TEST(OperationsType, TypicalSampledPositionOperations) {
    std::string const operations("GTV d; fmvn 1; hmvc 0;");
    std::optional<operations_type> operations_opt{
     operations_type::create(operations)};
    ASSERT_TRUE(operations_opt);
    std::optional<std::string> fen_opt{operations_opt->as_fen_opt()};

    ASSERT_FALSE(operations_opt->empty());
    ASSERT_EQ(3, operations_opt->size());
    ASSERT_EQ(operations, operations_opt->as_epd());
    ASSERT_TRUE(operations_opt->as_fen_opt());
}


TEST(OperationsType, OperationsSortedWhenGeneratingEpdFragment) {
    std::optional<operations_type> operations_opt{
     operations_type::create("hmvc 27; fmvn 53; GTV d;")};
    ASSERT_TRUE(operations_opt);
    std::optional<std::string> fen_opt{operations_opt->as_fen_opt()};

    ASSERT_FALSE(operations_opt->empty());
    ASSERT_EQ(3, operations_opt->size());
    ASSERT_EQ("GTV d; fmvn 53; hmvc 27;", operations_opt->as_epd());
    ASSERT_TRUE(operations_opt->as_fen_opt());
    ASSERT_EQ("27 53", *fen_opt);
}


TEST(OperationsType, CreateFromFenWithEmptyString) {
    std::string_view const hmvc_and_fmvn_operands("");
    std::optional<operations_type> operations_opt{
     operations_type::create_from_fen(hmvc_and_fmvn_operands)};
    ASSERT_FALSE(operations_opt);
}

TEST(OperationsType, CreateFromFenWithSingleArgument) {
    std::string_view const hmvc_and_fmvn_operands("88");
    std::optional<operations_type> operations_opt{
     operations_type::create_from_fen(hmvc_and_fmvn_operands)};
    ASSERT_FALSE(operations_opt);
}

TEST(OperationsType, CreateFromFenWithTwoArguments) {
    std::string_view const hmvc_and_fmvn_operands("0 1");
    std::optional<operations_type> operations_opt{
     operations_type::create_from_fen(hmvc_and_fmvn_operands)};
    ASSERT_TRUE(operations_opt);
    ASSERT_EQ("fmvn 1; hmvc 0;", operations_opt->as_epd());
    std::optional<std::string> fen_opt{operations_opt->as_fen_opt()};
    ASSERT_TRUE(fen_opt);
    ASSERT_EQ(hmvc_and_fmvn_operands, *fen_opt);
}

TEST(OperationsType, CreateFromFenWithThreeArguments) {
    std::string_view const hmvc_and_fmvn_operands("0 1 2");
    std::optional<operations_type> operations_opt{
     operations_type::create_from_fen(hmvc_and_fmvn_operands)};
    ASSERT_FALSE(operations_opt);
}
