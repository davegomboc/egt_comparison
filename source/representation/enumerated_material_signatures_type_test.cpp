#include "enumerated_material_signatures_type_test.hpp"

#include <algorithm>
#include <array>
#include <vector>


using namespace representation;


TEST(EnumeratedMaterialSignaturesType, TwoFigurine) {
    figurines_type const Kk{white_king, black_king};
    auto const material_signature_opt{material_signature_type::create_opt(Kk)};
    ASSERT_TRUE(material_signature_opt);

    constexpr unsigned int highest_figurine_count{2};
    enumerated_material_signatures_type const matsigs(highest_figurine_count);
    EXPECT_EQ(1, matsigs.material_signature_count(highest_figurine_count));
    EXPECT_EQ(*material_signature_opt, matsigs.material_signature(
     highest_figurine_count, 0));
}


TEST(EnumeratedMaterialSignaturesType, ThreeFigurine) {
    figurines_type const KQk{white_king, white_queen, black_king};
    figurines_type const KRk{white_king, white_rook, black_king};
    figurines_type const KBk{white_king, white_bishop, black_king};
    figurines_type const KNk{white_king, white_knight, black_king};
    figurines_type const KPk{white_king, white_pawn, black_king};
    constexpr unsigned int expected_material_signature_count{5};
    std::array<material_signature_type, expected_material_signature_count>
     expected{
     *material_signature_type::create_opt(KQk),
     *material_signature_type::create_opt(KRk),
     *material_signature_type::create_opt(KBk),
     *material_signature_type::create_opt(KNk),
     *material_signature_type::create_opt(KPk),
    };

    constexpr unsigned int highest_figurine_count{3};
    enumerated_material_signatures_type const matsigs(highest_figurine_count);
    EXPECT_EQ(expected_material_signature_count,
     matsigs.material_signature_count(highest_figurine_count));
    for (unsigned int i{}; i != expected_material_signature_count; ++i) {
        EXPECT_EQ(expected[i], matsigs.material_signature(
         highest_figurine_count, i));
    }
}


TEST(EnumeratedMaterialSignaturesType, FourFigurine) {
    figurines_type const KQQk{
     white_king, white_queen, white_queen, black_king};
    figurines_type const KQRk{
     white_king, white_queen, white_rook, black_king};
    figurines_type const KQBk{
     white_king, white_queen, white_bishop, black_king};
    figurines_type const KQNk{
     white_king, white_queen, white_knight, black_king};
    figurines_type const KQPk{
     white_king, white_queen, white_pawn, black_king};
    figurines_type const KQkq{
     white_king, white_queen, black_king, black_queen};
    figurines_type const KQkr{
     white_king, white_queen, black_king, black_rook};
    figurines_type const KQkb{
     white_king, white_queen, black_king, black_bishop};
    figurines_type const KQkn{
     white_king, white_queen, black_king, black_knight};
    figurines_type const KQkp{
     white_king, white_queen, black_king, black_pawn};
    figurines_type const KRRk{
     white_king, white_rook, white_rook, black_king};
    figurines_type const KRBk{
     white_king, white_rook, white_bishop, black_king};
    figurines_type const KRNk{
     white_king, white_rook, white_knight, black_king};
    figurines_type const KRPk{
     white_king, white_rook, white_pawn, black_king};
    figurines_type const KRkr{
     white_king, white_rook, black_rook, black_king};
    figurines_type const KRkb{
     white_king, white_rook, black_bishop, black_king};
    figurines_type const KRkn{
     white_king, white_rook, black_knight, black_king};
    figurines_type const KRkp{
     white_king, white_rook, black_pawn, black_king};
    figurines_type const KBBk{
     white_king, black_king, white_bishop, white_bishop};
    figurines_type const KBNk{
     white_king, black_king, white_bishop, white_knight};
    figurines_type const KBPk{
     white_king, black_king, white_bishop, white_pawn};
    figurines_type const KBkb{
     white_king, black_king, white_bishop, black_bishop};
    figurines_type const KBkn{
     white_king, black_king, white_bishop, black_knight};
    figurines_type const KBkp{
     white_king, black_king, white_bishop, black_pawn};
    figurines_type const KNNk{
     black_king, white_knight, white_knight, white_king};
    figurines_type const KNPk{
     black_king, white_pawn, white_knight, white_king};
    figurines_type const KNkn{
     black_king, black_knight, white_knight, white_king};
    figurines_type const KNkp{
     black_king, black_pawn, white_knight, white_king};
    figurines_type const KPPk{
      black_king, white_king, white_pawn, white_pawn};
    figurines_type const KPkp{
      black_pawn, white_pawn, black_king, white_king};
    constexpr unsigned int expected_material_signature_count{30};
    std::array<material_signature_type, expected_material_signature_count>
     expected{
     *material_signature_type::create_opt(KQQk),
     *material_signature_type::create_opt(KQRk),
     *material_signature_type::create_opt(KQBk),
     *material_signature_type::create_opt(KQNk),
     *material_signature_type::create_opt(KQPk),
     *material_signature_type::create_opt(KRRk),
     *material_signature_type::create_opt(KRBk),
     *material_signature_type::create_opt(KRNk),
     *material_signature_type::create_opt(KRPk),
     *material_signature_type::create_opt(KBBk),
     *material_signature_type::create_opt(KBNk),
     *material_signature_type::create_opt(KBPk),
     *material_signature_type::create_opt(KNNk),
     *material_signature_type::create_opt(KNPk),
     *material_signature_type::create_opt(KPPk),
     *material_signature_type::create_opt(KQkq),
     *material_signature_type::create_opt(KQkr),
     *material_signature_type::create_opt(KQkb),
     *material_signature_type::create_opt(KQkn),
     *material_signature_type::create_opt(KQkp),
     *material_signature_type::create_opt(KRkr),
     *material_signature_type::create_opt(KRkb),
     *material_signature_type::create_opt(KRkn),
     *material_signature_type::create_opt(KRkp),
     *material_signature_type::create_opt(KBkb),
     *material_signature_type::create_opt(KBkn),
     *material_signature_type::create_opt(KBkp),
     *material_signature_type::create_opt(KNkn),
     *material_signature_type::create_opt(KNkp),
     *material_signature_type::create_opt(KPkp),
    };

    constexpr unsigned int highest_figurine_count{4};
    enumerated_material_signatures_type const matsigs(highest_figurine_count);
    EXPECT_EQ(expected_material_signature_count,
     matsigs.material_signature_count(highest_figurine_count));
    for (unsigned int i{}; i != expected_material_signature_count; ++i) {
        EXPECT_EQ(expected[i], matsigs.material_signature(
         highest_figurine_count, i));
    }
}


TEST(EnumeratedMaterialSignaturesType, MaterialSignaturesForThreeOrLess) {
    figurines_type const KQk{white_king, white_queen, black_king};
    figurines_type const KRk{white_king, white_rook, black_king};
    figurines_type const KBk{white_king, white_bishop, black_king};
    figurines_type const KNk{white_king, white_knight, black_king};
    figurines_type const KPk{white_king, white_pawn, black_king};
    figurines_type const Kk{white_king, black_king};
    std::vector<material_signature_type> expected_signatures{
     *material_signature_type::create_opt(KQk),
     *material_signature_type::create_opt(KRk),
     *material_signature_type::create_opt(KBk),
     *material_signature_type::create_opt(KNk),
     *material_signature_type::create_opt(KPk),
     *material_signature_type::create_opt(Kk),
    };
    std::sort(expected_signatures.begin(), expected_signatures.end());

    unsigned int constexpr highest_figurine_count{3};
    enumerated_material_signatures_type matsigs(highest_figurine_count);
    std::vector<material_signature_type> signatures{
     matsigs.material_signatures()};
    std::sort(signatures.begin(), signatures.end());

    EXPECT_EQ(expected_signatures, signatures);
}
