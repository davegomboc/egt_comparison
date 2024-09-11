#include "representation/board_8x8_type_test.hpp"


using namespace representation;


TEST(Board8x8Type, ConstructEmpty) {
    board_8x8_type const board;
    EXPECT_TRUE(board.empty());
}

TEST(Board8x8Type, ConstructEmptyViaBoardContents) {
    epd::board_contents_type const empty_board_contents;
    board_8x8_type const board(empty_board_contents);
    EXPECT_TRUE(board.empty());
}

TEST(Board8x8Type, ExtractFigurinePlacementsFromInitialPositionFen) {
    std::string const initial_position_fen{
     "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};
    figurine_placements_type const expected{
     ra8, nb8, bc8, qd8, ke8, bf8, ng8, rh8,
     pa7, pb7, pc7, pd7, pe7, pf7, pg7, ph7,
     Pa2, Pb2, Pc2, Pd2, Pe2, Pf2, Pg2, Ph2,
     Ra1, Nb1, Bc1, Qd1, Ke1, Bf1, Ng1, Rh1};
    std::optional<board_8x8_type> const board_opt{
     board_8x8_type::create_opt(initial_position_fen)};
    ASSERT_TRUE(board_opt);
    figurine_placements_type const placements{board_opt->as_placements()};
    EXPECT_EQ(expected, placements);
}
