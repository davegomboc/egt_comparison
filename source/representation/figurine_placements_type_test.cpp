#include "representation/figurine_placements_type_test.hpp"


#include <sstream>

#include "epd/board_contents_type.hpp"

#include "representation/figurine_placement_type.hpp"


using namespace representation;


TEST(FigurinePlacementsType, SymmetricalPlacementsForka1Kh8) {
    figurine_placements_type placements{ka1, Kh8};
    auto symmetries{placements.symmetrical_placements()};
    std::sort(symmetries.begin(), symmetries.end());
    symmetries.erase(std::unique(symmetries.begin(), symmetries.end()),
     symmetries.end());
    std::vector<figurine_placements_type> expected{
        figurine_placements_type{Ka1, kh8},
        figurine_placements_type{Kh1, ka8},
        figurine_placements_type{ka1, Kh8},
        figurine_placements_type{kh1, Ka8},
    };
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(symmetries, expected);
}


TEST(FigurinePlacementsType, SymmetricalPlacementsForKb6kg4) {
    figurine_placements_type Kb6_kg4{Kb6, kg4};
    auto symmetries{Kb6_kg4.symmetrical_placements()};
    std::sort(symmetries.begin(), symmetries.end());
    std::vector<figurine_placements_type> expected{
        figurine_placements_type{Kb6, kg4},
        figurine_placements_type{Kg6, kb4},
        figurine_placements_type{Kb3, kg5},
        figurine_placements_type{Kg3, kb5},
        figurine_placements_type{Kc7, ke2},
        figurine_placements_type{Kf7, kd2},
        figurine_placements_type{Kc2, ke7},
        figurine_placements_type{Kf2, kd7},
        figurine_placements_type{kb6, Kg4},
        figurine_placements_type{kg6, Kb4},
        figurine_placements_type{kb3, Kg5},
        figurine_placements_type{kg3, Kb5},
        figurine_placements_type{kc7, Ke2},
        figurine_placements_type{kf7, Kd2},
        figurine_placements_type{kc2, Ke7},
        figurine_placements_type{kf2, Kd7},
    };
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(symmetries, expected);
}


TEST(FigurinePlacementsType, SymmetricalPlacementsForKf1kc8) {
    figurine_placements_type placements{Kf1, kc8};
    auto symmetries{placements.symmetrical_placements()};
    std::sort(symmetries.begin(), symmetries.end());
    symmetries.erase(std::unique(symmetries.begin(), symmetries.end()),
     symmetries.end());
    std::vector<figurine_placements_type> expected{
        figurine_placements_type{Kf1, kc8},
        figurine_placements_type{Kc1, kf8},
        figurine_placements_type{kf1, Kc8},
        figurine_placements_type{kc1, Kf8},
        figurine_placements_type{Kh3, ka6},
        figurine_placements_type{Ka3, kh6},
        figurine_placements_type{Kh6, ka3},
        figurine_placements_type{Ka6, kh3},
    };
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(symmetries, expected);
}


TEST(FigurinePlacementsType, SymmetricalPlacementsForKf1Pg2kc8) {
    figurine_placements_type placements{Kf1, Pg2, kc8};
    std::vector<figurine_placements_type> symmetries{
     placements.symmetrical_placements()};
    std::sort(symmetries.begin(), symmetries.end());
    std::vector<figurine_placements_type> expected{
        figurine_placements_type{Kf1, Pg2, kc8},
        figurine_placements_type{Kc1, Pb2, kf8},
        figurine_placements_type{kc8, pb7, Kf1},
        figurine_placements_type{kf8, pg7, Kc1},
    };
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(symmetries, expected);
}


TEST(FigurinePlacementsType, SymmetricalPlacementsForReti1921Study) {
    figurine_placements_type reti1921{Kh8, ka6, Pc6, ph5};
    auto symmetries{reti1921.symmetrical_placements()};
    std::sort(symmetries.begin(), symmetries.end());
    std::vector<figurine_placements_type> expected{
        figurine_placements_type{Kh8, ka6, Pc6, ph5},
        figurine_placements_type{Ka8, kh6, Pf6, pa5},
        figurine_placements_type{kh1, Ka3, pc3, Ph4},
        figurine_placements_type{ka1, Kh3, pf3, Pa4},
    };
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(symmetries, expected);
}


TEST(FigurinePlacementsType, SymmetricalPlacementsForNeustadtl1890Study) {
    figurine_placements_type neustadtl1890{Kg2, kd1, pe5, pg5, Pf3};
    auto symmetries{neustadtl1890.symmetrical_placements()};
    std::sort(symmetries.begin(), symmetries.end());
    std::vector<figurine_placements_type> expected{
        figurine_placements_type{pe5, pg5, kd1, Pf3, Kg2},
        figurine_placements_type{Pe4, Pg4, Kd8, pf6, kg7},
        figurine_placements_type{pd5, pb5, ke1, Pc3, Kb2},
        figurine_placements_type{Pd4, Pb4, Ke8, pc6, kb7},
    };
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(symmetries, expected);
}


TEST(FigurinePlacementsType, AgreementOnCanonicalPlacementForKf6kc4) {
    figurine_placements_type Kf6_kc4{Kf6, kc4};
    auto symmetries{Kf6_kc4.symmetrical_placements()};
    auto const canonical{Kf6_kc4.canonical_placement()};
    std::for_each(symmetries.cbegin(), symmetries.cend(),
     [&](figurine_placements_type const & placements) {
        EXPECT_EQ(canonical, placements.canonical_placement());
    });
}


TEST(FigurinePlacementsType, AgreementOnCanonicalPlacementForkc6Re4Rd5Kf3) {
    figurine_placements_type kc6_Re4_Rd5_Kf3{kc6, Re4, Rd5, Kf3};
    auto symmetries{kc6_Re4_Rd5_Kf3.symmetrical_placements()};
    auto const canonical{kc6_Re4_Rd5_Kf3.canonical_placement()};
    std::for_each(symmetries.cbegin(), symmetries.cend(),
     [&](figurine_placements_type const & placements) {
         EXPECT_EQ(canonical, placements.canonical_placement());
    });
    figurine_placements_type kc6_Rd5_Re4_Kf3{kc6, Rd5, Re4, Kf3};
    EXPECT_EQ(canonical, kc6_Rd5_Re4_Kf3.canonical_placement());
}


TEST(FigurinePlacementsType, LogicDigitsForKa8kd7) {
    figurine_placements_type Ka8_kd7{Ka8, kd7};
    logic_digits_type const expected{
     off, off, off, off, off, off,  // K @ a 8
     off, off,  on, off,  on,  on}; // k @ d 7
    logic_digits_type const logic_digits{Ka8_kd7.as_logic_digits()};
    EXPECT_EQ(expected, logic_digits);
}


TEST(FigurinePlacementsType, LogicDigitsForKa8Qb8kd7) {
    figurine_placements_type Ka8_Qb8_kd7{Ka8, Qb8, kd7};
    logic_digits_type const expected{
     off, off, off, off, off, off,  // K @ a 8
     off, off, off, off, off,  on,  // Q @ b 8
     off, off,  on, off,  on,  on}; // k @ d 7
    logic_digits_type const logic_digits{Ka8_Qb8_kd7.as_logic_digits()};
    EXPECT_EQ(expected, logic_digits);
}


TEST(FigurinePlacementsType, AppendToPositionId) {
    std::size_t position_id{1};
    figurine_placements_type placements{Kh3, kb6};
    placements.append(position_id);
    EXPECT_EQ(0b1'101'111'010'001, position_id);
}


TEST(FigurinePlacementsType, PositionIdIsNoLongerRemovedFrom) {
    std::optional<material_signature_type> Kk_opt{
     material_signature_type::create_opt("Kk")};
    ASSERT_TRUE(Kk_opt);
    figurine_placements_type expected_placements{Kh3, kb6};
    std::size_t position_id{0b1'101'111'010'001};
    figurine_placements_type placements(*Kk_opt, position_id);
    EXPECT_EQ(expected_placements, placements);
    EXPECT_EQ(0b1'101'111'010'001, position_id);
}


TEST(FigurinePlacementsType, AppendToBoolVector) {
    // arbitrary but matching prior data at start of both bits and expected_bits
    std::vector<bool> bits{false, true};
    std::vector<bool> expected_bits{
     false,  true,
      true, false,  true,  true,  true,  true, // K @ h 3
     false,  true, false, false, false,  true, // K @ b 6
    };
    figurine_placements_type placements{Kh3, kb6};
    placements.append(bits);
    EXPECT_TRUE(std::equal(expected_bits.cbegin(), expected_bits.cend(),
     bits.cbegin(), bits.cend()));
}


TEST(FigurinePlacementsType, AppendToUnsignedCharVector) {
    // arbitrary but matching prior data at start of both bits and expected_bits
    std::vector<unsigned char> bits_as_uchars{false,  true, false,  true};
    std::vector<unsigned char> expected_bits_as_uchars{
     false,  true, false,  true,
      true, false,  true,  true,  true,  true, // K @ h 3
     false,  true, false, false, false,  true, // K @ b 6
    };
    figurine_placements_type placements{Kh3, kb6};
    placements.append(bits_as_uchars);
    EXPECT_TRUE(std::equal(expected_bits_as_uchars.cbegin(),
     expected_bits_as_uchars.cend(), bits_as_uchars.cbegin(),
     bits_as_uchars.cend()));
}


TEST(FigurinePlacementsType, InitializeFromIdAsBoolVector) {
    figurine_placements_type expected{Kh3, kb6};
    std::optional<material_signature_type> Kk_opt{
     material_signature_type::create_opt("Kk")};
    ASSERT_TRUE(Kk_opt);
    std::vector<bool> bits{
      true, false,  true,  true,  true,  true, // K @ h 3
     false,  true, false, false, false,  true, // k @ b 6
    };
    figurine_placements_type const placements(*Kk_opt, bits);
    EXPECT_TRUE(bits.empty());
    EXPECT_EQ(expected, placements);
}


TEST(FigurinePlacementsType, InitializeFromIdAsUnsignedCharVector) {
    figurine_placements_type expected{Kh3, kb6};
    std::optional<material_signature_type> Kk_opt{
     material_signature_type::create_opt("Kk")};
    ASSERT_TRUE(Kk_opt);
    std::vector<unsigned char> bits_as_uchars{
      true, false,  true,  true,  true,  true, // K @ h 3
     false,  true, false, false, false,  true, // k @ b 6
    };
    figurine_placements_type const placements(*Kk_opt, bits_as_uchars);
    EXPECT_TRUE(bits_as_uchars.empty());
    EXPECT_EQ(expected, placements);
}


TEST(FigurinePlacementsType, InitializeFromEmptyEpdBoardContents) {
    std::optional<epd::board_contents_type> const empty_board_contents_opt(
     epd::board_contents_type::create("8/8/8/8/8/8/8/8"));
    ASSERT_TRUE(empty_board_contents_opt.has_value());
    figurine_placements_type const expected{};
    figurine_placements_type const obtained(*empty_board_contents_opt);
    EXPECT_EQ(obtained, expected);
}


TEST(FigurinePlacementsType, InitializeFromInitialEpdBoardContents) {
    std::optional<epd::board_contents_type> const initial_board_contents_opt(
     epd::board_contents_type::create(
     "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));
    ASSERT_TRUE(initial_board_contents_opt.has_value());
    figurine_placements_type const expected{
     ra8, nb8, bc8, qd8, ke8, bf8, ng8, rh8,
     pa7, pb7, pc7, pd7, pe7, pf7, pg7, ph7,
     Pa2, Pb2, Pc2, Pd2, Pe2, Pf2, Pg2, Ph2,
     Ra1, Nb1, Bc1, Qd1, Ke1, Bf1, Ng1, Rh1};
    figurine_placements_type const obtained(*initial_board_contents_opt);
    EXPECT_EQ(obtained, expected);
}
