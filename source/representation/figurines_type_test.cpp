#include "representation/figurines_type_test.hpp"


#include "epd/board_contents_type.hpp"


using namespace representation;


TEST(FigurinesType, ConstructFromEmptyBoardContentsType) {
    epd::board_contents_type const empty_board;
    figurines_type const figurines(empty_board);
    EXPECT_TRUE(figurines.empty());
    EXPECT_FALSE(figurines.valid_material_balance());
}


TEST(FigurinesType, ConstructFromStartingPositonBoardContentsType) {
    std::optional<epd::board_contents_type> const initial_board_opt{
     epd::board_contents_type::create(
     "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR")};
    ASSERT_TRUE(initial_board_opt);
    figurines_type const figurines(*initial_board_opt);
    EXPECT_EQ(32, figurines.size());
    EXPECT_TRUE(figurines.valid_material_balance());
}


TEST(FigurinesType, Create) {
    figurines_type const expected{white_king, white_queen, white_pawn,
     black_king, black_rook, black_rook};
    std::optional<figurines_type> const figurines_opt{
     figurines_type::create_opt("KQP_vs_krr")};
    ASSERT_TRUE(figurines_opt);
    EXPECT_EQ(expected, *figurines_opt);
}


TEST(FigurinesType, LessThan) {
    EXPECT_LT(figurines_type::create_opt("Q"), figurines_type::create_opt("R"));
    EXPECT_LT(figurines_type::create_opt("b"), figurines_type::create_opt("n"));
    EXPECT_LT(figurines_type::create_opt("N"), figurines_type::create_opt("q"));
    EXPECT_LT(figurines_type::create_opt("P"), figurines_type::create_opt("PK"));
    EXPECT_LT(figurines_type::create_opt("QN"), figurines_type::create_opt("NQ"));
}
