#include "probe_report_nybble_endgame_table_type_test.hpp"


#include <optional>

#include "representation/canonicalization_enum.hpp"

#include "fen_position_wcdbl_verification_type.hpp"
#include "probe_report_nybble_endgame_table_parameterized_testing_type.hpp"


using namespace representation;


TEST(ProbeReportNybbleEndgameTableType, MapAllSymmetriesKVskIntoMemory) {
    std::size_t const expected_byte_count{1ull << (2 * 6)};
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> K_vs_k_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, *K_vs_k_opt)};
    EXPECT_NE(nullptr, K_vs_k_egt_ptr);
    EXPECT_EQ(expected_byte_count, K_vs_k_egt_ptr->size());
}


TEST(ProbeReportNybbleEndgameTableType, MapSingleSymmetryKVskIntoMemory) {
    std::size_t const expected_byte_count{1ull << (2 * 6)};
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> K_vs_k_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::single_symmetry, *K_vs_k_opt)};
    EXPECT_NE(nullptr, K_vs_k_egt_ptr);
    EXPECT_EQ(expected_byte_count, K_vs_k_egt_ptr->size());
}


TEST(ProbeReportNybbleEndgameTableType, MapAllSymmetriesKPVskIntoMemory) {
    std::size_t const expected_byte_count{1ull << (3 * 6)};
    std::optional<material_signature_type> KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> KP_vs_k_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, *KP_vs_k_opt)};
    EXPECT_NE(nullptr, KP_vs_k_egt_ptr);
    EXPECT_EQ(expected_byte_count, KP_vs_k_egt_ptr->size());
}


TEST(ProbeReportNybbleEndgameTableType, MapSingleSymmetryKPVskIntoMemory) {
    std::size_t const expected_byte_count{1ull << (3 * 6)};
    std::optional<material_signature_type> KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> KP_vs_k_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::single_symmetry, *KP_vs_k_opt)};
    EXPECT_NE(nullptr, KP_vs_k_egt_ptr);
    EXPECT_EQ(expected_byte_count, KP_vs_k_egt_ptr->size());
}


TEST(ProbeReportNybbleEndgameTableType, MapAllSymmetriesKPVskpIntoMemory) {
    std::size_t const expected_byte_count{1ull << (4 * 6)};
    std::optional<material_signature_type> KP_vs_kp_opt{
     material_signature_type::create_opt("KP_vs_kp")};
    ASSERT_TRUE(KP_vs_kp_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> KP_vs_kp_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, *KP_vs_kp_opt)};
    EXPECT_NE(nullptr, KP_vs_kp_egt_ptr);
    EXPECT_EQ(expected_byte_count, KP_vs_kp_egt_ptr->size());
}


TEST(ProbeReportNybbleEndgameTableType, MapSingleSymmetryKPVskpIntoMemory) {
    std::size_t const expected_byte_count{1ull << (4 * 6)};
    std::optional<material_signature_type> KP_vs_kp_opt{
     material_signature_type::create_opt("KP_vs_kp")};
    ASSERT_TRUE(KP_vs_kp_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> KP_vs_kp_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::single_symmetry, *KP_vs_kp_opt)};
    EXPECT_NE(nullptr, KP_vs_kp_egt_ptr);
    EXPECT_EQ(expected_byte_count, KP_vs_kp_egt_ptr->size());
}


TEST(ProbeReportNybbleEndgameTableType, MapAllSymmetriesKPPVskpIntoMemory) {
    std::size_t const expected_byte_count{1ull << (5 * 6)};
    std::optional<material_signature_type> KPP_vs_kp_opt{
     material_signature_type::create_opt("KPP_vs_kp")};
    ASSERT_TRUE(KPP_vs_kp_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> KPP_vs_kp_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, *KPP_vs_kp_opt)};
    EXPECT_NE(nullptr, KPP_vs_kp_egt_ptr);
    EXPECT_EQ(expected_byte_count, KPP_vs_kp_egt_ptr->size());
}


TEST(ProbeReportNybbleEndgameTableType, MapSingleSymmetryKPPVskpIntoMemory) {
    std::size_t const expected_byte_count{1ull << (5 * 6)};
    std::optional<material_signature_type> KPP_vs_kp_opt{
     material_signature_type::create_opt("KPP_vs_kp")};
    ASSERT_TRUE(KPP_vs_kp_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> KPP_vs_kp_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::single_symmetry, *KPP_vs_kp_opt)};
    EXPECT_NE(nullptr, KPP_vs_kp_egt_ptr);
    EXPECT_EQ(expected_byte_count, KPP_vs_kp_egt_ptr->size());
}


TEST(ProbeReportNybbleEndgameTableType, AllSymmetriesEveryKVskProbeResultIsDrawn) {
    std::size_t const expected_byte_count{1ull << (2 * 6)};
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> K_vs_k_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, *K_vs_k_opt)};
    ASSERT_NE(nullptr, K_vs_k_egt_ptr);
    ASSERT_EQ(expected_byte_count, K_vs_k_egt_ptr->size());
    for (std::size_t i{}; i != 2 * expected_byte_count; ++i) {
        std::size_t consumable_position_id{i};
        egt_position_type const position_to_probe(*K_vs_k_opt,
         consumable_position_id);
        probe_report_type const probe_report(
         K_vs_k_egt_ptr->probe_after_canonicalizing(position_to_probe));
        ASSERT_TRUE((draw == probe_report) or (invalid == probe_report)
         or (illegal == probe_report));
    }
}


TEST(ProbeReportNybbleEndgameTableType, SingleSymmetryEveryKVskProbeResultIsDrawn) {
    std::size_t const expected_byte_count{1ull << (2 * 6)};
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> K_vs_k_egt_ptr{
     probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::single_symmetry, *K_vs_k_opt)};
    ASSERT_NE(nullptr, K_vs_k_egt_ptr);
    ASSERT_EQ(expected_byte_count, K_vs_k_egt_ptr->size());
    for (std::size_t i{}; i != 2 * expected_byte_count; ++i) {
        std::size_t consumable_position_id{i};
        egt_position_type const position_to_probe(*K_vs_k_opt,
         consumable_position_id);
        probe_report_type const probe_report(
         K_vs_k_egt_ptr->probe_after_canonicalizing(position_to_probe));
        ASSERT_TRUE((draw == probe_report) or (invalid == probe_report)
         or (illegal == probe_report));
    }
}


/* These tests are too slow to run routinely. */
TEST(ProbeReportNybbleEndgameTableType, AllSymmetriesEveryKBVskProbeResultIsDrawn) {
    std::size_t const expected_byte_count{1ull << (3 * 6)};
    std::optional<material_signature_type> KB_vs_k_opt{
     material_signature_type::create_opt("KB_vs_k")};
    ASSERT_TRUE(KB_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> const
     KB_vs_k_egt_ptr{probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, *KB_vs_k_opt)};
    ASSERT_NE(nullptr, KB_vs_k_egt_ptr);
    ASSERT_EQ(expected_byte_count, KB_vs_k_egt_ptr->size());
    for (std::size_t i{}; i != 2 * expected_byte_count; ++i) {
        std::size_t consumable_position_id{i};
        egt_position_type const position_to_probe(*KB_vs_k_opt,
         consumable_position_id);
        probe_report_type const probe_report(
         KB_vs_k_egt_ptr->probe_after_canonicalizing(position_to_probe));
        ASSERT_TRUE((draw == probe_report) or (invalid == probe_report)
         or (illegal == probe_report));
    }
}


TEST(ProbeReportNybbleEndgameTableType, SingleSymmetryEveryKBVskProbeResultIsDrawn) {
    std::size_t const expected_byte_count{1ull << (3 * 6)};
    std::optional<material_signature_type> KB_vs_k_opt{
     material_signature_type::create_opt("KB_vs_k")};
    ASSERT_TRUE(KB_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> const
     KB_vs_k_egt_ptr{probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::single_symmetry, *KB_vs_k_opt)};
    ASSERT_NE(nullptr, KB_vs_k_egt_ptr);
    ASSERT_EQ(expected_byte_count, KB_vs_k_egt_ptr->size());
    for (std::size_t i{}; i != 2 * expected_byte_count; ++i) {
        std::size_t consumable_position_id{i};
        egt_position_type const position_to_probe(*KB_vs_k_opt,
         consumable_position_id);
        probe_report_type const probe_report(
         KB_vs_k_egt_ptr->probe_after_canonicalizing(position_to_probe));
        ASSERT_TRUE((draw == probe_report) or (invalid == probe_report)
         or (illegal == probe_report));
    }
}


TEST(ProbeReportNybbleEndgameTableType, AllSymmetriesEveryKNVskProbeResultIsDrawn) {
    std::size_t const expected_byte_count{1ull << (3 * 6)};
    std::optional<material_signature_type> const KN_vs_k_opt{
     material_signature_type::create_opt("KN_vs_k")};
    ASSERT_TRUE(KN_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> const
     KN_vs_k_egt_ptr{probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, *KN_vs_k_opt)};
    ASSERT_NE(nullptr, KN_vs_k_egt_ptr);
    ASSERT_EQ(expected_byte_count, KN_vs_k_egt_ptr->size());
    for (std::size_t i{}; i != 2 * expected_byte_count; ++i) {
        std::size_t consumable_position_id{i};
        egt_position_type const position_to_probe(*KN_vs_k_opt,
         consumable_position_id);
        probe_report_type const probe_report(
         KN_vs_k_egt_ptr->probe_after_canonicalizing(position_to_probe));
        ASSERT_TRUE((draw == probe_report) or (invalid == probe_report)
         or (illegal == probe_report));
    }
}


TEST(ProbeReportNybbleEndgameTableType, SingleSymmetryEveryKNVskProbeResultIsDrawn) {
    std::size_t const expected_byte_count{1ull << (3 * 6)};
    std::optional<material_signature_type> const KN_vs_k_opt{
     material_signature_type::create_opt("KN_vs_k")};
    ASSERT_TRUE(KN_vs_k_opt.has_value());
    std::unique_ptr<probe_report_nybble_endgame_table_type> const
     KN_vs_k_egt_ptr{probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::single_symmetry, *KN_vs_k_opt)};
    ASSERT_NE(nullptr, KN_vs_k_egt_ptr);
    ASSERT_EQ(expected_byte_count, KN_vs_k_egt_ptr->size());
    for (std::size_t i{}; i != 2 * expected_byte_count; ++i) {
        std::size_t consumable_position_id{i};
        egt_position_type const position_to_probe(*KN_vs_k_opt,
         consumable_position_id);
        probe_report_type const probe_report(
         KN_vs_k_egt_ptr->probe_after_canonicalizing(position_to_probe));
        ASSERT_TRUE((draw == probe_report) or (invalid == probe_report)
         or (illegal == probe_report));
    }
}
/**/


INSTANTIATE_TEST_SUITE_P(ProbeReportNybbleEndgameTableType,
 probe_report_nybble_endgame_table_parameterized_testing_type,
 testing::ValuesIn(wcdbl_test_positions()));


TEST(ProbeReportNybbleEndgameTableType, AllSymmetriesProbeInvalidPositionDescription) {
    std::optional<material_signature_type> const KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt);
    std::unique_ptr<probe_report_nybble_endgame_table_type> const
     KP_vs_k_egt_ptr{probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, *KP_vs_k_opt)};
    ASSERT_TRUE(KP_vs_k_egt_ptr);
    egt_position_type const overlapping_king_and_pawn(black, {Ke3, kd5, pe3},
     no_ep);
    EXPECT_EQ(invalid, KP_vs_k_egt_ptr->probe_after_canonicalizing(
     overlapping_king_and_pawn));
    egt_position_type const overlapping_kings(black, {Kf3, kf3, pe3}, no_ep);
    EXPECT_EQ(invalid, KP_vs_k_egt_ptr->probe_after_canonicalizing(
     overlapping_kings));
}


TEST(ProbeReportNybbleEndgameTableType, SingleSymmetryProbeInvalidPositionDescription) {
    std::optional<material_signature_type> const KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt);
    std::unique_ptr<probe_report_nybble_endgame_table_type> const
     KP_vs_k_egt_ptr{probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::single_symmetry, *KP_vs_k_opt)};
    ASSERT_TRUE(KP_vs_k_egt_ptr);
    egt_position_type const overlapping_king_and_pawn(black, {Ke3, kd5, pe3},
     no_ep);
    EXPECT_EQ(invalid, KP_vs_k_egt_ptr->probe_after_canonicalizing(
     overlapping_king_and_pawn));
    egt_position_type const overlapping_kings(black, {Kf3, kf3, pe3}, no_ep);
    EXPECT_EQ(invalid, KP_vs_k_egt_ptr->probe_after_canonicalizing(
     overlapping_kings));
}
