#include "representation/egt_position_type_test.hpp"


#include <sstream>

#include "representation/coordinate_type.hpp"
#include "representation/figurine_colour_type.hpp"
#include "representation/figurine_placement_type.hpp"
#include "representation/material_signature_type.hpp"


using namespace representation;


TEST(EgtPositionType, InitializedSameFromDifferentIdForms) {
    auto const material_signature_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(material_signature_opt.has_value());

    std::size_t const bit_pair_count{13};
    std::size_t const limit{1 << bit_pair_count};
    for (std::size_t position_id{}; position_id != limit; ++position_id) {
        std::vector<bool> id_as_bool_vector{as_bool_vector_id(bit_pair_count,
         position_id)};
        std::size_t consumable_id{position_id};
        egt_position_type from_position_id(*material_signature_opt,
         consumable_id);
        egt_position_type from_vector_id(*material_signature_opt,
         id_as_bool_vector);
        ASSERT_EQ(from_position_id, from_vector_id);
    }
}


TEST(EgtPositionType, BoolVectorIdRoundTrip) {
    std::size_t const bit_pair_count{13};
    std::size_t const limit{1 << bit_pair_count};
    for (std::size_t position_id{}; position_id != limit; ++position_id) {
        EXPECT_EQ(position_id,
         as_position_id(as_bool_vector_id(bit_pair_count, position_id)));
    }
}


TEST(EgtPositionType, UnsignedCharVectorIdRoundTrip) {
    std::size_t const bit_pair_count{13};
    std::size_t const limit{1 << bit_pair_count};
    for (std::size_t position_id{}; position_id != limit; ++position_id) {
        EXPECT_EQ(position_id,
         as_position_id(as_mtbdd_uchar_vector_id(bit_pair_count, position_id)));
    }
}


TEST(EgtPositionType, OperatorLessThanWhenFigurinePlacementsIdentical) {
    egt_position_type btm(black, {Kd8, ke1}, no_ep);
    egt_position_type wtm(white, {Kd8, ke1}, no_ep);
    EXPECT_LT(wtm, btm);
}


TEST(EgtPositionType, FlipFilesOfKa5Pb3kc6pf7btm) {
    egt_position_type expected(black, {Kh5, Pg3, kf6, pc7}, no_ep);
    egt_position_type position(black, {Ka5, Pb3, kc6, pf7}, no_ep);
    position.flip_files();
    position.restore_ordering_invariant();
    EXPECT_EQ(expected, position);
}


TEST(EgtPositionType, InvertKa5Pb3kc6pf7btm) {
    egt_position_type expected(white, {ka4, pb6, Kc3, Pf2}, no_ep);
    egt_position_type position(black, {Ka5, Pb3, kc6, pf7}, no_ep);
    position.invert();
    position.restore_ordering_invariant();
    EXPECT_EQ(expected, position);
}


TEST(EgtPositionType, SymmetricalPositionsForKd8ke3btm) {
    egt_position_type position(black, {Kd8, ke3}, no_ep);

    std::vector<egt_position_type> expected_symmetries{
     egt_position_type(black, {Kd8, ke3}, no_ep),
     egt_position_type(white, {kd1, Ke6}, no_ep),
     egt_position_type(black, {Ke8, kd3}, no_ep),
     egt_position_type(white, {ke1, Kd6}, no_ep),
     egt_position_type(black, {Kd1, ke6}, no_ep),
     egt_position_type(white, {kd8, Ke3}, no_ep),
     egt_position_type(black, {Ke1, kd6}, no_ep),
     egt_position_type(white, {ke8, Kd3}, no_ep),
     egt_position_type(black, {Ka5, kf4}, no_ep),
     egt_position_type(white, {kh5, Kc4}, no_ep),
     egt_position_type(black, {Ka4, kf5}, no_ep),
     egt_position_type(white, {kh4, Kc5}, no_ep),
     egt_position_type(black, {Kh5, kc4}, no_ep),
     egt_position_type(white, {ka5, Kf4}, no_ep),
     egt_position_type(black, {Kh4, kc5}, no_ep),
     egt_position_type(white, {ka4, Kf5}, no_ep)};
    std::sort(expected_symmetries.begin(), expected_symmetries.end());

    auto symmetries{position.symmetrical_positions()};
    std::sort(symmetries.begin(), symmetries.end());
    symmetries.erase(std::unique(symmetries.begin(), symmetries.end()),
     symmetries.end());
    EXPECT_EQ(symmetries, expected_symmetries);
}


TEST(EgtPositionType, SymmetricalPositionsForKa5Pb3kc6pf7btm) {
    egt_position_type position(black, {Ka5, Pb3, kc6, pf7}, no_ep);

    std::vector<egt_position_type> expected_symmetries{
     egt_position_type(black, {Ka5, Pb3, kc6, pf7}, no_ep),
     egt_position_type(black, {Kh5, Pg3, kf6, pc7}, no_ep),
     egt_position_type(white, {ka4, pb6, Kc3, Pf2}, no_ep),
     egt_position_type(white, {kh4, pg6, Kf3, Pc2}, no_ep)};
    std::sort(expected_symmetries.begin(), expected_symmetries.end());

    auto symmetries{position.symmetrical_positions()};
    std::sort(symmetries.begin(), symmetries.end());
    symmetries.erase(std::unique(symmetries.begin(), symmetries.end()),
     symmetries.end());
    EXPECT_EQ(symmetries, expected_symmetries);
}


TEST(EgtPositionType, SymmetricalPositionsForKh1Pc3kb2pd7wtm) {
    egt_position_type const position(white, {Kh1, Pc3, kb2, pd7}, no_ep);
    std::vector<egt_position_type> expected_symmetries{
     egt_position_type(white, {Kh1, Pc3, kb2, pd7}, no_ep),
     egt_position_type(black, {kh8, pc6, Kb7, Pd2}, no_ep),
     egt_position_type(white, {Ka1, Pf3, kg2, pe7}, no_ep),
     egt_position_type(black, {ka8, pf6, Kg7, Pe2}, no_ep)};
    std::sort(expected_symmetries.begin(), expected_symmetries.end());

    auto symmetries{position.symmetrical_positions()};
    std::sort(symmetries.begin(), symmetries.end());
    symmetries.erase(std::unique(symmetries.begin(), symmetries.end()),
     symmetries.end());
    EXPECT_EQ(symmetries, expected_symmetries);
}


TEST(EgtPositionType, Ka8Qc8kd8btmIsCanonical) {
    egt_position_type position(black, {Ka8, Qc8, kd8}, no_ep);

    std::vector<egt_position_type> expected_symmetries{
     egt_position_type(black, {Ka8, Qc8, kd8}, no_ep),
     egt_position_type(black, {Kh8, Qf8, ke8}, no_ep),
     egt_position_type(black, {Kh1, Qf1, ke1}, no_ep),
     egt_position_type(black, {Ka1, Qc1, kd1}, no_ep),
     egt_position_type(black, {Ka8, Qa6, ka5}, no_ep),
     egt_position_type(black, {Ka1, Qa3, ka4}, no_ep),
     egt_position_type(black, {Kh1, Qh3, kh4}, no_ep),
     egt_position_type(black, {Kh8, Qh6, kh5}, no_ep),
     egt_position_type(white, {ka8, qc8, Kd8}, no_ep),
     egt_position_type(white, {kh8, qf8, Ke8}, no_ep),
     egt_position_type(white, {kh1, qf1, Ke1}, no_ep),
     egt_position_type(white, {ka1, qc1, Kd1}, no_ep),
     egt_position_type(white, {ka8, qa6, Ka5}, no_ep),
     egt_position_type(white, {ka1, qa3, Ka4}, no_ep),
     egt_position_type(white, {kh1, qh3, Kh4}, no_ep),
     egt_position_type(white, {kh8, qh6, Kh5}, no_ep),
    };
    std::sort(expected_symmetries.begin(), expected_symmetries.end());

    auto symmetries{position.symmetrical_positions()};
    std::sort(symmetries.begin(), symmetries.end());
    symmetries.erase(std::unique(symmetries.begin(), symmetries.end()),
     symmetries.end());
    ASSERT_EQ(symmetries, expected_symmetries);

    egt_position_type canonical_position{position.canonical_position(
     canonicalization_enum::all_symmetries)};
    EXPECT_EQ(position, canonical_position);
}


TEST(EgtPositionType, CanonicalizeUsingSingleSymmetryYieldsInverse) {
    egt_position_type const position(black, {Ke5, ke3, nd4}, no_ep);
    egt_position_type const expected(white, {Ke6, Nd5, ke4}, no_ep);
    egt_position_type const canonical(position.canonical_position(
     canonicalization_enum::single_symmetry));
    EXPECT_EQ(canonical, expected);
}


TEST(EgtPositionType, CanonicalizeUsingSingleSymmetryYieldsOriginal) {
    egt_position_type const position(white, {Ke6, Nd5, ke4}, no_ep);
    egt_position_type const expected(position);
    egt_position_type const canonical(position.canonical_position(
     canonicalization_enum::single_symmetry));
    EXPECT_EQ(canonical, expected);
}


TEST(EgtPositionType, AsLogicDigitsForKa5Pb3kc6pf7btm) {
    logic_digits_type expected{
      on,           // black to move
     off,  on,  on, // rank 5
     off, off, off, // file a
      on, off,  on, // rank 3
     off, off,  on, // file b
     off,  on, off, // rank 6
     off,  on, off, // file c
     off, off,  on, // rank 7
      on, off,  on, // file f
    };
    egt_position_type position(black, {Ka5, Pb3, kc6, pf7}, no_ep);
    logic_digits_type position_as_logic_digits(position.as_logic_digits());
    EXPECT_EQ(expected, position_as_logic_digits);
}


TEST(EgtPositionType, InitializeFromIdAsBoolVector) {
    egt_position_type expected(black, {Kh3, kb6}, no_ep);
    std::optional<material_signature_type> Kk_opt{
     material_signature_type::create_opt("Kk")};
    ASSERT_TRUE(Kk_opt);
    std::vector<bool> bits{
      true,                                    // btm
      true, false,  true,  true,  true,  true, // K @ h 3
     false,  true, false, false, false,  true, // k @ b 6
    };
    egt_position_type const egt_position(*Kk_opt, bits);
    EXPECT_TRUE(0 == bits.size());
    EXPECT_EQ(expected, egt_position);
}


TEST(EgtPositionType, InitializeFromIdAsUnsignedShortVector) {
    egt_position_type expected(black, {Kh3, kb6}, no_ep);
    std::optional<material_signature_type> Kk_opt{
     material_signature_type::create_opt("Kk")};
    ASSERT_TRUE(Kk_opt);
    std::vector<unsigned short> bits_as_ushorts{
      true,                                    // btm
      true, false,  true,  true,  true,  true, // K @ h 3
     false,  true, false, false, false,  true, // k @ b 6
    };
    egt_position_type const egt_position(*Kk_opt, bits_as_ushorts);
    EXPECT_TRUE(0 == bits_as_ushorts.size());
    EXPECT_EQ(expected, egt_position);
}


TEST(EgtPositionType, RoundTripViaBoolVectorForKa5Bb3kc6nf7btm) {
    egt_position_type const position(black, {Ka5, Bb3, kc6, nf7}, no_ep);
    std::vector<bool> id_as_bool_vector{position.as_bool_vector_id()};
    std::optional<material_signature_type> const material_signature_opt{
     material_signature_type::create_opt(position.placements().figurines())};
    ASSERT_TRUE(material_signature_opt);
    egt_position_type const reconstructed_position(*material_signature_opt,
     id_as_bool_vector);
    EXPECT_EQ(position, reconstructed_position);
}


TEST(EgtPositionType, RoundTripViaPositionIdForKa5Bb3kc6nf7btm) {
    egt_position_type const position(black, {Ka5, Bb3, kc6, nf7}, no_ep);
    std::size_t const position_id{position.as_position_id()};
    std::optional<material_signature_type> const material_signature_opt{
     material_signature_type::create_opt(position.placements().figurines())};
    ASSERT_TRUE(material_signature_opt);
    egt_position_type const reconstructed_position(*material_signature_opt,
     position_id);
    EXPECT_EQ(position, reconstructed_position);
}


TEST(EgtPositionType, RoundTripViaBoolVectorForKa1Pd5kg7pc5epc6wtm) {
    egt_position_type const position(white, {Ka1, Pd5, kg7, pc5}, c6);
    std::vector<bool> id_as_bool_vector{position.as_bool_vector_id()};
    std::optional<material_signature_type> const material_signature_opt{
     material_signature_type::create_opt(position.placements().figurines())};
    ASSERT_TRUE(material_signature_opt);
    egt_position_type const reconstructed_position(*material_signature_opt,
     id_as_bool_vector);
    EXPECT_EQ(position, reconstructed_position);
}


TEST(EgtPositionType, RoundTripViaPositionIdForKa1Pd5kg7pc5epc6wtm) {
    egt_position_type const position(white, {Ka1, Pd5, kg7, pc5}, c6);
    std::size_t const position_id{position.as_position_id()};
    std::optional<material_signature_type> const material_signature_opt{
     material_signature_type::create_opt(position.placements().figurines())};
    ASSERT_TRUE(material_signature_opt);
    egt_position_type const reconstructed_position(*material_signature_opt,
     position_id);
    EXPECT_EQ(position, reconstructed_position);
}
