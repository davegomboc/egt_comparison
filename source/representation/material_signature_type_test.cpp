#include "representation/material_signature_type_test.hpp"


#include "representation/figurine_type.hpp"
#include "representation/figurines_type.hpp"


using namespace representation;


TEST(MaterialSignatureType, KkIsNotLessThanKk) {
    auto Kk_opt{material_signature_type::create_opt(figurines_type(
     {white_king, black_king}))};
    ASSERT_TRUE(Kk_opt);
    EXPECT_GE(*Kk_opt, *Kk_opt);
}


TEST(MaterialSignatureType, KkLessThanKQk) {
    auto Kk_opt{material_signature_type::create_opt(figurines_type(
     {white_king, black_king}))};
    auto KQk_opt{material_signature_type::create_opt(figurines_type(
     {white_queen, white_king, black_king}))};
    ASSERT_TRUE(Kk_opt);
    ASSERT_TRUE(KQk_opt);
    EXPECT_LT(*Kk_opt, *KQk_opt);
}


TEST(MaterialSignatureType, KkLessThanKkq) {
    auto Kk_opt{material_signature_type::create_opt(figurines_type(
     {white_king, black_king}))};
    auto Kkq_opt{material_signature_type::create_opt(figurines_type(
     {black_queen, white_king, black_king}))};
    ASSERT_TRUE(Kk_opt);
    ASSERT_TRUE(Kkq_opt);
    EXPECT_LT(*Kk_opt, *Kkq_opt);
}


TEST(MaterialSignatureType, KQkLessThanKkq) {
    auto KQk_opt{material_signature_type::create_opt(figurines_type(
     {white_queen, white_king, black_king}))};
    auto Kkq_opt{material_signature_type::create_opt(figurines_type(
     {black_queen, white_king, black_king}))};
    ASSERT_TRUE(KQk_opt);
    ASSERT_TRUE(Kkq_opt);
    EXPECT_LT(*KQk_opt, *Kkq_opt);
}


TEST(MaterialSignatureType, KQkLessThanKRk) {
    auto KQk_opt{material_signature_type::create_opt(figurines_type(
     {white_king, black_king, white_queen}))};
    auto KRk_opt{material_signature_type::create_opt(figurines_type(
     {white_rook, white_king, black_king}))};
    ASSERT_TRUE(KQk_opt);
    ASSERT_TRUE(KRk_opt);
    EXPECT_LT(*KQk_opt, *KRk_opt);
}


TEST(MaterialSignatureType, KPkLessThanKkq) {
    auto KPk_opt{material_signature_type::create_opt(figurines_type(
     {white_king, black_king, white_pawn}))};
    auto Kkq_opt{material_signature_type::create_opt(figurines_type(
     {white_king, black_king, black_queen}))};
    ASSERT_TRUE(KPk_opt);
    ASSERT_TRUE(Kkq_opt);
    EXPECT_LT(*KPk_opt, *Kkq_opt);
}


TEST(MaterialSignatureType, KkqLessThanKkp) {
    auto Kkq_opt{material_signature_type::create_opt(figurines_type(
     {black_queen, black_king, white_king}))};
    auto Kkp_opt{material_signature_type::create_opt(figurines_type(
     {black_pawn, white_king, black_king}))};
    ASSERT_TRUE(Kkq_opt);
    ASSERT_TRUE(Kkp_opt);
    EXPECT_LT(*Kkq_opt, *Kkp_opt);
}


TEST(MaterialSignatureType, KkpLessThanKQQk) {
    auto Kkp_opt{material_signature_type::create_opt(figurines_type(
     {black_pawn, white_king, black_king}))};
    auto KQQk_opt{material_signature_type::create_opt(figurines_type(
     {white_queen, black_king, white_king, white_queen}))};
    ASSERT_TRUE(Kkp_opt);
    ASSERT_TRUE(KQQk_opt);
    EXPECT_LT(*Kkp_opt, *KQQk_opt);
}


TEST(MaterialSignatureType, KQQkpLessThanKQRk) {
    auto KQQk_opt{material_signature_type::create_opt(figurines_type(
     {white_queen, black_king, white_king, white_queen}))};
    auto KQRk_opt{material_signature_type::create_opt(figurines_type(
     {white_rook, black_king, white_king, white_queen}))};
    ASSERT_TRUE(KQQk_opt);
    ASSERT_TRUE(KQRk_opt);
    EXPECT_LT(*KQQk_opt, *KQRk_opt);
}


TEST(MaterialSignatureType, KQPkLessThanKRRk) {
    auto KQPk_opt{material_signature_type::create_opt(figurines_type(
     {white_pawn, black_king, white_king, white_queen}))};
    auto KRRk_opt{material_signature_type::create_opt(figurines_type(
     {white_rook, black_king, white_king, white_rook}))};
    ASSERT_TRUE(KQPk_opt);
    ASSERT_TRUE(KRRk_opt);
    EXPECT_LT(*KQPk_opt, *KRRk_opt);
}


TEST(MaterialSignatureType, KPPkLessThanKQkq) {
    auto KPPk_opt{material_signature_type::create_opt(figurines_type(
     {white_pawn, black_king, white_king, white_pawn}))};
    auto KQkq_opt{material_signature_type::create_opt(figurines_type(
     {black_queen, black_king, white_king, white_queen}))};
    ASSERT_TRUE(KPPk_opt);
    ASSERT_TRUE(KQkq_opt);
    EXPECT_LT(*KPPk_opt, *KQkq_opt);
}


TEST(MaterialSignatureType, KPkpLessThanKkqq) {
    auto KQkq_opt{material_signature_type::create_opt(figurines_type(
     {black_queen, black_king, white_king, white_queen}))};
    auto Kkqq_opt{material_signature_type::create_opt(figurines_type(
     {black_queen, black_king, white_king, black_queen}))};
    ASSERT_TRUE(KQkq_opt);
    ASSERT_TRUE(Kkqq_opt);
    EXPECT_LT(*KQkq_opt, *Kkqq_opt);
}


TEST(MaterialSignatureType, KkppLessThanKQQQk) {
    auto Kkpp_opt{material_signature_type::create_opt(figurines_type(
     {black_pawn, black_king, white_king, black_pawn}))};
    auto KQQQk_opt{material_signature_type::create_opt(figurines_type(
     {black_king, white_king, white_queen, white_queen, white_queen}))};
    ASSERT_TRUE(Kkpp_opt);
    ASSERT_TRUE(KQQQk_opt);
    EXPECT_LT(*Kkpp_opt, *KQQQk_opt);
}


TEST(MaterialSignatureType, KBBkqLessThanKQkbb) {
    auto KBBkq_opt{material_signature_type::create_opt(figurines_type(
     {white_king, white_bishop, white_bishop, black_king, black_queen}))};
    auto KQkbb_opt{material_signature_type::create_opt(figurines_type(
     {white_king, white_queen, black_king, black_bishop, black_bishop}))};
    ASSERT_TRUE(KBBkq_opt);
    ASSERT_TRUE(KQkbb_opt);
    EXPECT_LT(*KBBkq_opt, *KQkbb_opt);
}


TEST(MaterialSignatureType, FindAndCreateUsingPath) {
    figurines_type const expected_figurines{
     white_king, white_queen, white_pawn, black_king, black_rook, black_rook};
    auto const material_signature_opt{
     material_signature_type::find_and_create_opt(
     "/home/dave/EGTs/chesspresso/6/KQP_vs_krr.sorted.xz")};
    ASSERT_TRUE(material_signature_opt);
    EXPECT_EQ(material_signature_opt->as_figurines(), expected_figurines);
}


TEST(MaterialSignatureType, EqualMaterialKk) {
    figurines_type const Kk{white_king, black_king};
    auto const material_signature_opt{material_signature_type::create_opt(Kk)};
    ASSERT_TRUE(material_signature_opt);
    EXPECT_TRUE(material_signature_opt->equal());
}


TEST(MaterialSignatureType, EqualMaterialKRk) {
    figurines_type const KRk{white_king, white_rook, black_king};
    auto const material_signature_opt{material_signature_type::create_opt(KRk)};
    ASSERT_TRUE(material_signature_opt);
    EXPECT_FALSE(material_signature_opt->equal());
}


TEST(MaterialSignatureType, EqualMaterialKRkr) {
    figurines_type const KRkr{white_king, white_rook, black_king, black_rook};
    auto const material_signature_opt{
     material_signature_type::create_opt(KRkr)};
    ASSERT_TRUE(material_signature_opt);
    EXPECT_TRUE(material_signature_opt->equal());
}


TEST(MaterialSignatureType, EqualMaterialKRkn) {
    figurines_type const KRkn{white_king, white_rook, black_king, black_knight};
    auto const material_signature_opt{
     material_signature_type::create_opt(KRkn)};
    ASSERT_TRUE(material_signature_opt);
    EXPECT_FALSE(material_signature_opt->equal());
}


TEST(MaterialSignatureType, CubeSchemaKk) {
    figurines_type const Kk{white_king, black_king};
    auto const material_signature_opt{material_signature_type::create_opt(Kk)};
    ASSERT_TRUE(material_signature_opt);
    cube_schema_type const cube_schema{material_signature_opt->cube_schema()};
    EXPECT_EQ(cube_schema_type(1 + (2 * 6), 3), cube_schema);
}


TEST(MaterialSignatureType, CubeSchemaKQk) {
    figurines_type const KQk{white_king, white_queen, black_king};
    auto const material_signature_opt{material_signature_type::create_opt(KQk)};
    ASSERT_TRUE(material_signature_opt);
    cube_schema_type const cube_schema{material_signature_opt->cube_schema()};
    EXPECT_EQ(cube_schema_type(1 + (3 * 6), 3), cube_schema);
}


TEST(MaterialSignatureType, CubeSchemaKQkr) {
    figurines_type const KQkr{white_king, white_queen, black_king, black_rook};
    auto const material_signature_opt{
     material_signature_type::create_opt(KQkr)};
    ASSERT_TRUE(material_signature_opt);
    cube_schema_type const cube_schema{material_signature_opt->cube_schema()};
    EXPECT_EQ(cube_schema_type(1 + (4 * 6), 3), cube_schema);
}


TEST(MaterialSignatureType, CubeSchemaKBNkq) {
    figurines_type const KBNkq{
     white_king, white_bishop, white_knight, black_king, black_queen};
    auto const material_signature_opt{
     material_signature_type::create_opt(KBNkq)};
    ASSERT_TRUE(material_signature_opt);
    cube_schema_type const cube_schema{material_signature_opt->cube_schema()};
    EXPECT_EQ(cube_schema_type(1 + (5 * 6), 3), cube_schema);
}


TEST(MaterialSignatureType, CubeSchemaKQRkqp) {
    figurines_type const KQRkqp{
     white_king, white_queen, white_rook, black_king, black_queen, black_pawn};
    auto const material_signature_opt{
     material_signature_type::create_opt(KQRkqp)};
    ASSERT_TRUE(material_signature_opt);
    cube_schema_type const cube_schema{material_signature_opt->cube_schema()};
    EXPECT_EQ(cube_schema_type(1 + (6 * 6), 3), cube_schema);
}


TEST(MaterialSignatureType, AsStringKQkr) {
    figurines_type const KQkr{white_king, white_queen, black_king, black_rook};
    auto const material_signature_opt{
     material_signature_type::create_opt(KQkr)};
    ASSERT_TRUE(material_signature_opt);
    std::string const KQ_vs_kr{material_signature_opt->as_string()};
    EXPECT_EQ(KQ_vs_kr, "KQ_vs_kr");
}


TEST(MaterialSignatureType, AsSyzygyStringKQkr) {
    figurines_type const KQkr{white_king, white_queen, black_king, black_rook};
    auto const material_signature_opt{
     material_signature_type::create_opt(KQkr)};
    ASSERT_TRUE(material_signature_opt);
    std::string const KQvKR{material_signature_opt->as_syzygy_string()};
    EXPECT_EQ(KQvKR, "KQvKR");
}
