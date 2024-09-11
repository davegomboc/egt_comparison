#include "position_sampling_type_test.hpp"


#include <map>


using namespace representation;


TEST(PositionSamplingType, DrawCoordinatesUniformly) {
    position_sampling_type sampler;
    std::map<coordinate_type, int> coordinate_draw_counts;
    for (int i{}; i != 6400; ++i) {
        coordinate_type coordinate{sampler.draw_coordinate_uniformly()};
        ++coordinate_draw_counts[coordinate];
    }

    // We expect that each of the 64 coordinates has been sampled between 60 and
    // 140 times, inclusive.
    EXPECT_EQ(64, coordinate_draw_counts.size());
    coordinate_type coordinate{a8};
    do {
        int times_drawn{coordinate_draw_counts[coordinate]};
        //std::cerr << "II: " << coordinate << " " << times_drawn << "\n";
        EXPECT_TRUE((60 <= times_drawn) and (times_drawn <= 140));
    } while (coordinate.next());
}


TEST(PositionSamplingType, DrawNonKingFigurineUniformly) {
    position_sampling_type sampler;
    std::map<figurine_type, int> figurine_draw_counts;
    for (int i{}; i != 1000; ++i) {
        figurine_type non_king_figurine{
         sampler.draw_non_king_figurine_uniformly()};
        ++figurine_draw_counts[non_king_figurine];
    }

    // We expect that each of the 10 eligible figurines has been sampled between
    // 60 and 140 times, inclusive, and that the kings were never sampled.
    EXPECT_EQ(10, figurine_draw_counts.size());
    figurines_type const desired_figurines{
     black_queen, black_rook, black_bishop, black_knight, black_pawn,
     white_queen, white_rook, white_bishop, white_knight, white_pawn};
    for (figurine_type figurine: desired_figurines) {
        int times_drawn{figurine_draw_counts[figurine]};
        //std::cerr << "II: " << figurine << " " << times_drawn << "\n";
        EXPECT_TRUE((60 <= times_drawn) and (times_drawn <= 140));
    }
    EXPECT_EQ(0, figurine_draw_counts[black_king]);
    EXPECT_EQ(0, figurine_draw_counts[white_king]);
}


TEST(PositionSamplingType, DrawColourToMoveUniformly) {
    position_sampling_type sampler;
    std::map<figurine_colour_type, std::size_t> colour_to_move_draw_counts;
    for (std::size_t i{}; i != 200; ++i) {
        figurine_colour_type const colour_to_move{
         sampler.draw_colour_to_move_uniformly()};
        ++colour_to_move_draw_counts[colour_to_move];
    }

    // We expect that each of the two colours has been sampled between 60 and
    // 140 times, inclusive.
    //std::cerr << "II: black = " << colour_to_move_draw_counts[black] << '\n';
    //std::cerr << "II: white = " << colour_to_move_draw_counts[white] << '\n';
    EXPECT_TRUE(60 <= colour_to_move_draw_counts[black]);
    EXPECT_TRUE(60 <= colour_to_move_draw_counts[white]);
    EXPECT_TRUE(colour_to_move_draw_counts[black] <= 140);
    EXPECT_TRUE(colour_to_move_draw_counts[white] <= 140);
}


TEST(PositionSamplingType, DrawEgtEntrancePositionUniformlyFromFigurineCount) {
    position_sampling_type sampler;
    for (unsigned int figurine_count{2}; figurine_count != 6;
     ++figurine_count) {
        //std::cerr << "II: figurine_count = " << figurine_count << '\n';
        for (int i{}; i != 20; ++i) {
            auto [probe_report, tb_position]{
             sampler.draw_egt_entrance_position_uniformly(figurine_count)};
            // TODO: Automate verifying that the figurine count of each
            // generated position is correct.
            //std::cerr << "II: " << tb_position.as_fen() << ' '
            // << probe_report.as_description() << '\n';
            EXPECT_NE(invalid, probe_report);
            EXPECT_NE(illegal, probe_report);
            EXPECT_NE(redundant, probe_report);
            EXPECT_NE(unknown, probe_report);
        }
    }
}
