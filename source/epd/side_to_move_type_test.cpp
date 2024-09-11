#include "epd/side_to_move_type_test.hpp"


using namespace epd;


TEST(SideToMoveType, ParseEmptyString) {
    std::string const side_to_move{""};
    std::optional<side_to_move_type> const side_to_move_opt{
     side_to_move_type::create(side_to_move)};
    ASSERT_FALSE(side_to_move_opt);
}


TEST(SideToMoveType, ParseInvalidString) {
    std::string const side_to_move{"B"};
    std::optional<side_to_move_type> const side_to_move_opt{
     side_to_move_type::create(side_to_move)};
    ASSERT_FALSE(side_to_move_opt);
}


TEST(SideToMoveType, ParseBlackToMove) {
    std::string const side_to_move{"b"};
    std::optional<side_to_move_type> const side_to_move_opt{
     side_to_move_type::create(side_to_move)};
    ASSERT_TRUE(side_to_move_opt);
    ASSERT_TRUE(side_to_move_opt->black_is_to_move());
    ASSERT_FALSE(side_to_move_opt->white_is_to_move());
    ASSERT_EQ('b', side_to_move_opt->as_char());
}


TEST(SideToMoveType, ParseWhiteToMove) {
    std::string const side_to_move{"w"};
    std::optional<side_to_move_type> const side_to_move_opt{
     side_to_move_type::create(side_to_move)};
    ASSERT_TRUE(side_to_move_opt);
    ASSERT_FALSE(side_to_move_opt->black_is_to_move());
    ASSERT_TRUE(side_to_move_opt->white_is_to_move());
    ASSERT_EQ('w', side_to_move_opt->as_char());
}
