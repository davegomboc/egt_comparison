#include "representation/material_side_type_test.hpp"


using namespace representation;


TEST(MaterialSideType, KinglessIsNotAllowed) {
    EXPECT_FALSE(material_side_type::create_opt({queen, rook}));
}


TEST(MaterialSideType, TwoKingsAreNotAllowed) {
    EXPECT_FALSE(material_side_type::create_opt({king, king}));
}


TEST(MaterialSideType, LowerCaseAccepted) {
    std::optional<material_side_type> side_opt{
     material_side_type::create_opt("kpp")};
    EXPECT_TRUE(side_opt);
}

TEST(MaterialSideType, MixedCaseAccepted) {
    std::optional<material_side_type> side_opt{
     material_side_type::create_opt("KpP")};
    EXPECT_TRUE(side_opt);
}

TEST(MaterialSideType, UpperCaseAccepted) {
    std::optional<material_side_type> side_opt{
     material_side_type::create_opt("KPP")};
    EXPECT_TRUE(side_opt);
}

TEST(MaterialSideType, MorePromotedPiecesThanAbsentPawnsNotAllowed) {
    EXPECT_FALSE(material_side_type::create_opt("KQRRQQQQRRRRR"));
}


TEST(MaterialSideType, PromotedPiecesEqualToAbsentPawnsAllowed) {
    std::optional<material_side_type> side_opt{
     material_side_type::create_opt("KQRRQQQQRRRR")};
    EXPECT_TRUE(side_opt);
}


TEST(MaterialSideType, StreamInsertion) {
    std::optional<material_side_type> starting_side_opt{
     material_side_type::create_opt("pppppppprbnqkbnr")};
    EXPECT_TRUE(starting_side_opt);
    std::ostringstream out;
    out << *starting_side_opt;
    EXPECT_EQ(out.str(), "KQRRBBNNPPPPPPPP");
}


TEST(MaterialSideType, OneOfEachFigurineEndUpSortedAfterCreate) {
    std::optional<material_side_type> side_opt{
     material_side_type::create_opt("pqbrnk")};
    ASSERT_TRUE(side_opt);
    material_side_type const & side{*side_opt};
    EXPECT_EQ(6, side.size());
    EXPECT_EQ(king, side[0]);
    EXPECT_EQ(queen, side[1]);
    EXPECT_EQ(rook, side[2]);
    EXPECT_EQ(bishop, side[3]);
    EXPECT_EQ(knight, side[4]);
    EXPECT_EQ(pawn, side[5]);
}


TEST(MaterialSideType, MoreFigurinesIsLessThanFewerFigurines) {
    std::optional<material_side_type> kp_opt{
     material_side_type::create_opt({king, pawn})};
    std::optional<material_side_type> kbn_opt{
     material_side_type::create_opt({king, bishop, knight})};
    std::optional<material_side_type> kppp_opt{
     material_side_type::create_opt({king, pawn, pawn, pawn})};
    ASSERT_TRUE(kp_opt);
    ASSERT_TRUE(kbn_opt);
    ASSERT_TRUE(kppp_opt);
    EXPECT_LT(*kbn_opt, *kp_opt);
    EXPECT_LT(*kppp_opt, *kbn_opt);
}


TEST(MaterialSideType, OneAndTwoShapesEqualityIsAsExpected) {
    std::optional<material_side_type> k_opt{
     material_side_type::create_opt({king})};
    std::optional<material_side_type> kq_opt{
     material_side_type::create_opt({king, queen})};
    std::optional<material_side_type> kr_opt{
     material_side_type::create_opt({king, rook})};
    std::optional<material_side_type> kb_opt{
     material_side_type::create_opt({king, bishop})};
    std::optional<material_side_type> kn_opt{
     material_side_type::create_opt({king, knight})};
    std::optional<material_side_type> kp_opt{
     material_side_type::create_opt({king, pawn})};
    ASSERT_TRUE(k_opt);
    ASSERT_TRUE(kq_opt);
    ASSERT_TRUE(kr_opt);
    ASSERT_TRUE(kb_opt);
    ASSERT_TRUE(kn_opt);
    ASSERT_TRUE(kp_opt);
    material_side_type const & k{*k_opt};
    material_side_type const & kq{*kq_opt};
    material_side_type const & kr{*kr_opt};
    material_side_type const & kb{*kb_opt};
    material_side_type const & kn{*kn_opt};
    material_side_type const & kp{*kp_opt};
    EXPECT_EQ(kq, kq);
    EXPECT_NE(kq, kr);
    EXPECT_NE(kq, kb);
    EXPECT_NE(kq, kn);
    EXPECT_NE(kq, kp);
    EXPECT_NE(kq, k);
    EXPECT_EQ(kr, kr);
    EXPECT_NE(kr, kb);
    EXPECT_NE(kr, kn);
    EXPECT_NE(kr, kp);
    EXPECT_NE(kr, k);
    EXPECT_EQ(kb, kb);
    EXPECT_NE(kb, kn);
    EXPECT_NE(kb, kp);
    EXPECT_NE(kb, k);
    EXPECT_EQ(kn, kn);
    EXPECT_NE(kn, kp);
    EXPECT_NE(kn, k);
    EXPECT_EQ(kp, kp);
    EXPECT_NE(kp, k);
    EXPECT_EQ(k, k);
}


TEST(MaterialSideType, OneAndTwoShapesOrderingIsAsExpected) {
    std::optional<material_side_type> k_opt{
     material_side_type::create_opt({king})};
    std::optional<material_side_type> kq_opt{
     material_side_type::create_opt({king, queen})};
    std::optional<material_side_type> kr_opt{
     material_side_type::create_opt({king, rook})};
    std::optional<material_side_type> kb_opt{
     material_side_type::create_opt({king, bishop})};
    std::optional<material_side_type> kn_opt{
     material_side_type::create_opt({king, knight})};
    std::optional<material_side_type> kp_opt{
     material_side_type::create_opt({king, pawn})};
    ASSERT_TRUE(k_opt);
    ASSERT_TRUE(kq_opt);
    ASSERT_TRUE(kr_opt);
    ASSERT_TRUE(kb_opt);
    ASSERT_TRUE(kn_opt);
    ASSERT_TRUE(kp_opt);
    material_side_type const & k{*k_opt};
    material_side_type const & kq{*kq_opt};
    material_side_type const & kr{*kr_opt};
    material_side_type const & kb{*kb_opt};
    material_side_type const & kn{*kn_opt};
    material_side_type const & kp{*kp_opt};
    EXPECT_LE(kq, kq);
    EXPECT_GE(kq, kq);
    EXPECT_LT(kq, kr);
    EXPECT_GT(kr, kq);
    EXPECT_LT(kq, kb);
    EXPECT_GT(kb, kq);
    EXPECT_LT(kq, kn);
    EXPECT_GT(kn, kq);
    EXPECT_LT(kq, kp);
    EXPECT_GT(kp, kq);
    EXPECT_LT(kq, k);
    EXPECT_GT(k, kq);
    EXPECT_LE(kr, kr);
    EXPECT_GE(kr, kr);
    EXPECT_LT(kr, kb);
    EXPECT_GT(kb, kr);
    EXPECT_LT(kr, kn);
    EXPECT_GT(kn, kr);
    EXPECT_LT(kr, kp);
    EXPECT_GT(kp, kr);
    EXPECT_LT(kr, k);
    EXPECT_GT(k, kr);
    EXPECT_LE(kb, kb);
    EXPECT_GE(kb, kb);
    EXPECT_LT(kb, kn);
    EXPECT_GT(kn, kb);
    EXPECT_LT(kb, kp);
    EXPECT_GT(kp, kb);
    EXPECT_LT(kb, k);
    EXPECT_GT(k, kb);
    EXPECT_LE(kn, kn);
    EXPECT_GE(kn, kn);
    EXPECT_LT(kn, kp);
    EXPECT_GT(kp, kn);
    EXPECT_LT(kn, k);
    EXPECT_GT(k, kn);
    EXPECT_LE(kp, kp);
    EXPECT_GE(kp, kp);
    EXPECT_LT(kp, k);
    EXPECT_GT(k, kp);
    EXPECT_LE(k, k);
    EXPECT_GE(k, k);
}
