#include "epd/en_passant_target_type_test.hpp"


using namespace epd;


TEST(EnPassantTargetType, ParseEmptyString) {
    std::string const en_passant_target{""};
    auto const en_passant_target_opt{
     en_passant_target_type::create(en_passant_target)};
    ASSERT_FALSE(en_passant_target_opt);
}


TEST(EnPassantTargetType, ParseInvalidContent) {
    std::string const en_passant_target{"?"};
    auto const en_passant_target_opt{
     en_passant_target_type::create(en_passant_target)};
    ASSERT_FALSE(en_passant_target_opt);
}


TEST(EnPassantTargetType, ParseInvalidEnPassantSquare) {
    std::string const en_passant_target{"d8"};
    auto const en_passant_target_opt{
     en_passant_target_type::create(en_passant_target)};
    ASSERT_FALSE(en_passant_target_opt);
}


TEST(EnPassantTargetType, ParseNoEnPassantSquare) {
    std::string const en_passant_target{"-"};
    auto const en_passant_target_opt{
     en_passant_target_type::create(en_passant_target)};
    ASSERT_TRUE(en_passant_target_opt);
    ASSERT_FALSE(en_passant_target_opt->specified());
    ASSERT_EQ(0, en_passant_target_opt->as_uint8_t());
    ASSERT_EQ('a', en_passant_target_opt->file_as_char());
    ASSERT_EQ('8', en_passant_target_opt->rank_as_char());
    ASSERT_EQ(en_passant_target, en_passant_target_opt->as_epd_fen());
}


TEST(EnPassantTargetType, ParseSingularEnPassantSquare) {
    std::string const en_passant_target{"a8"};
    auto const en_passant_target_opt{
     en_passant_target_type::create(en_passant_target)};
    ASSERT_TRUE(en_passant_target_opt);
    ASSERT_FALSE(en_passant_target_opt->specified());
    ASSERT_EQ(0, en_passant_target_opt->as_uint8_t());
    ASSERT_EQ('a', en_passant_target_opt->file_as_char());
    ASSERT_EQ('8', en_passant_target_opt->rank_as_char());
    ASSERT_EQ("-", en_passant_target_opt->as_epd_fen());
}


TEST(EnPassantTargetType, ParseBlackJustMovedPawnTwoSquares) {
    std::string const en_passant_target{"h6"};
    auto const en_passant_target_opt{
     en_passant_target_type::create(en_passant_target)};
    ASSERT_TRUE(en_passant_target_opt);
    ASSERT_TRUE(en_passant_target_opt->specified());
    ASSERT_EQ(23, en_passant_target_opt->as_uint8_t());
    ASSERT_EQ('h', en_passant_target_opt->file_as_char());
    ASSERT_EQ('6', en_passant_target_opt->rank_as_char());
    ASSERT_EQ(en_passant_target, en_passant_target_opt->as_epd_fen());
}


TEST(EnPassantTargetType, ParseWhiteJustMovedPawnTwoSquares) {
    std::string const en_passant_target{"a3"};
    auto const en_passant_target_opt{
     en_passant_target_type::create(en_passant_target)};
    ASSERT_TRUE(en_passant_target_opt);
    ASSERT_TRUE(en_passant_target_opt->specified());
    ASSERT_EQ(40, en_passant_target_opt->as_uint8_t());
    ASSERT_EQ('a', en_passant_target_opt->file_as_char());
    ASSERT_EQ('3', en_passant_target_opt->rank_as_char());
    ASSERT_EQ(en_passant_target, en_passant_target_opt->as_epd_fen());
}
