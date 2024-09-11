#include "representation/figurine_placement_type_test.hpp"


#include <sstream>


using namespace representation;


TEST(FigurinePlacementType, FlipA8H1ForKa1) {
    figurine_placement_type placement{Ka1};
    placement.flip_a8h1();
    EXPECT_EQ(placement, Kh8);
}


TEST(FigurinePlacementType, FlipColourForKa1) {
    figurine_placement_type placement{Ka1};
    placement.flip_colour();
    EXPECT_EQ(placement, ka1);
}


TEST(FigurinePlacementType, FlipFileForKa1) {
    figurine_placement_type placement{Ka1};
    placement.flip_file();
    EXPECT_EQ(placement, Kh1);
}


TEST(FigurinePlacementType, FlipRankForKa1) {
    figurine_placement_type placement{Ka1};
    placement.flip_rank();
    EXPECT_EQ(placement, Ka8);
}


TEST(FigurinePlacementType, FlipA8H1ForKf5) {
    figurine_placement_type placement{Kf5};
    placement.flip_a8h1();
    EXPECT_EQ(placement, Kd3);
}


TEST(FigurinePlacementType, FlipColourForKf5) {
    figurine_placement_type placement{Kf5};
    placement.flip_colour();
    EXPECT_EQ(placement, kf5);
}


TEST(FigurinePlacementType, FlipFileForKf5) {
    figurine_placement_type placement{Kf5};
    placement.flip_file();
    EXPECT_EQ(placement, Kc5);
}


TEST(FigurinePlacementType, FlipRankForKf5) {
    figurine_placement_type placement{Kf5};
    placement.flip_rank();
    EXPECT_EQ(placement, Kf4);
}


TEST(FigurinePlacementType, FlipA8H1Forkg2) {
    figurine_placement_type placement{kg2};
    placement.flip_a8h1();
    EXPECT_EQ(placement, kg2);
}


TEST(FigurinePlacementType, FlipColourForkg2) {
    figurine_placement_type placement{kg2};
    placement.flip_colour();
    EXPECT_EQ(placement, Kg2);
}


TEST(FigurinePlacementType, FlipFileForkg2) {
    figurine_placement_type placement{kg2};
    placement.flip_file();
    EXPECT_EQ(placement, kb2);
}


TEST(FigurinePlacementType, FlipRankForkg2) {
    figurine_placement_type placement{kg2};
    placement.flip_rank();
    EXPECT_EQ(placement, kg7);
}


TEST(FigurinePlacementType, LessThan) {
    EXPECT_LT(Ka8, Kb8);
    EXPECT_LT(Kb8, Ka7);
    EXPECT_LT(Ka7, Ka6);
    EXPECT_LT(Ka1, Kh1);
    EXPECT_LT(Kh1, Qa8);
    EXPECT_LT(bf5, nd8);
    EXPECT_LT(ng4, pa8);
    EXPECT_LT(ph8, ph1);
}


TEST(FigurinePlacementType, AppendToBoolVector) {
    std::vector<bool> expected_bits{true, true, false, true, true, true, true};
    std::vector<bool> bits{true};
    h3.append(bits);
    EXPECT_TRUE(std::equal(expected_bits.cbegin(), expected_bits.cend(),
     bits.cbegin(), bits.cend()));
}


TEST(FigurinePlacementType, AppendToUnsignedCharVector) {
    std::vector<unsigned char> expected_bits_as_uchars{
     false, true, false, true, true, true, true};
    std::vector<unsigned char> bits_as_uchars{false};
    h3.append(bits_as_uchars);
    EXPECT_TRUE(std::equal(expected_bits_as_uchars.cbegin(),
     expected_bits_as_uchars.cend(), bits_as_uchars.cbegin(),
     bits_as_uchars.cend()));
}


TEST(FigurinePlacementType, InitializeFromIdAsBoolVector) {
    std::vector<bool> bits{true, false, true, true, true, true};
    figurine_placement_type const placement(white_king, bits);
    EXPECT_TRUE(bits.empty());
    EXPECT_EQ(figurine_placement_type(white_king, h3), placement);
}


TEST(FigurinePlacementType, InitializeFromIdAsUnsignedCharVector) {
    std::vector<unsigned char> bits_as_uchars{
     true, false, true, true, true, true};
    figurine_placement_type const placement(white_king, bits_as_uchars);
    EXPECT_TRUE(bits_as_uchars.empty());
    EXPECT_EQ(figurine_placement_type(white_king, h3), placement);
}


TEST(FigurinePlacementType, IdentifyEnPassantTargetsFromIllegalPawnPlacements) {
    EXPECT_EQ(a3, Pa1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(b3, Pb1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(c3, Pc1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(d3, Pd1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(e3, Pe1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(f3, Pf1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(g3, Pg1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(h3, Ph1.identify_en_passant_target_from_illegal_pawn_placement());

    EXPECT_EQ(no_ep, Pa8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, Pb8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, Pc8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, Pd8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, Pe8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, Pf8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, Pg8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, Ph8.identify_en_passant_target_from_illegal_pawn_placement());

    EXPECT_EQ(a6, pa8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(b6, pb8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(c6, pc8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(d6, pd8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(e6, pe8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(f6, pf8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(g6, pg8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(h6, ph8.identify_en_passant_target_from_illegal_pawn_placement());

    EXPECT_EQ(no_ep, pa1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, pb1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, pc1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, pd1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, pe1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, pf1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, pg1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, ph1.identify_en_passant_target_from_illegal_pawn_placement());

    EXPECT_EQ(no_ep, Na1.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, na8.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, Pa2.identify_en_passant_target_from_illegal_pawn_placement());
    EXPECT_EQ(no_ep, pa7.identify_en_passant_target_from_illegal_pawn_placement());
}
