#include "epd/board_contents_type_test.hpp"


using namespace epd;


TEST(BoardContentsType, Parse) {
    std::string const board_contents{"8/8/2P5/2k5/4B3/8/6N1/3K4"};
    auto const board_contents_opt{board_contents_type::create(board_contents)};
    ASSERT_TRUE(board_contents_opt);
    ASSERT_EQ(board_contents, board_contents_opt->as_epd_fen());
}
