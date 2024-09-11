#include "indexed_cube_list_endgame_table_type_test.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <optional>

#include "representation/cover_enum.hpp"
#include "representation/figurine_colour_type.hpp"
#include "representation/material_signature_type.hpp"

#include "paths.hpp"


using namespace representation;


TEST(IndexedCubeListEndgameTableType, ProbeKVsk) {
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(K_vs_k_opt->as_string(), "", true));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<indexed_cube_list_endgame_table_type> K_vs_k_egt_ptr{
     indexed_cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(K_vs_k_egt_ptr);

    egt_position_type const Ke4kd6wtm(white, {Ke4, kd6}, no_ep);
    EXPECT_EQ(draw, K_vs_k_egt_ptr->probe_after_canonicalizing(Ke4kd6wtm));
    egt_position_type const Kg3kf5btm(black, {Kg3, kf5}, no_ep);
    EXPECT_EQ(draw, K_vs_k_egt_ptr->probe_after_canonicalizing(Kg3kf5btm));
}


TEST(IndexedCubeListEndgameTableType, ProbeKBVskD01I) {
    std::optional<material_signature_type> KB_vs_k_opt{
     material_signature_type::create_opt("KB_vs_k")};
    ASSERT_TRUE(KB_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KB_vs_k_opt->as_string(), "D01I", true));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<indexed_cube_list_endgame_table_type> KB_vs_k_egt_ptr{
     indexed_cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KB_vs_k_egt_ptr);

    egt_position_type const wtm(white, {Bc6, Ke6, kg6}, no_ep);
    EXPECT_EQ(draw, KB_vs_k_egt_ptr->probe_after_canonicalizing(wtm));
}


TEST(IndexedCubeListEndgameTableType, ProbeKNVskD01I) {
    std::optional<material_signature_type> KN_vs_k_opt{
     material_signature_type::create_opt("KN_vs_k")};
    ASSERT_TRUE(KN_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KN_vs_k_opt->as_string(), "D01I", true));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<indexed_cube_list_endgame_table_type> KN_vs_k_egt_ptr{
     indexed_cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KN_vs_k_egt_ptr);

    egt_position_type const btm(black, {nd4, ke3, Ke5}, no_ep);
    EXPECT_EQ(draw, KN_vs_k_egt_ptr->probe_after_canonicalizing(btm));
}


TEST(IndexedCubeListEndgameTableType, ProbeKPVskD01I) {
    std::optional<material_signature_type> KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KP_vs_k_opt->as_string(), "D01I", true));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<indexed_cube_list_endgame_table_type> KP_vs_k_egt_ptr{
     indexed_cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KP_vs_k_egt_ptr);

    egt_position_type btm_draw_by_stalemate(black, {Ka6, Pa7, ka8}, no_ep);
    EXPECT_EQ(draw, KP_vs_k_egt_ptr->probe_after_canonicalizing(
     btm_draw_by_stalemate));
    egt_position_type wtm_wwin(white, {Ke6, Pe7, ke8}, no_ep);
    EXPECT_EQ(win, KP_vs_k_egt_ptr->probe_after_canonicalizing(wtm_wwin));
    egt_position_type btm_wlos(black, {Ka6, kc6, ph7}, no_ep);
    EXPECT_EQ(loss, KP_vs_k_egt_ptr->probe_after_canonicalizing(btm_wlos));
    egt_position_type const Kb1Pb5ka3wtm(white, {Kb1, ka3, Pb5}, no_ep);
    EXPECT_EQ(win, KP_vs_k_egt_ptr->probe_after_canonicalizing(Kb1Pb5ka3wtm));
}


TEST(IndexedCubeListEndgameTableType, ProbeKRVskD01I) {
    std::optional<material_signature_type> KR_vs_k_opt{
     material_signature_type::create_opt("KR_vs_k")};
    ASSERT_TRUE(KR_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KR_vs_k_opt->as_string(), "D01I", true));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<indexed_cube_list_endgame_table_type> KR_vs_k_egt_ptr{
     indexed_cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KR_vs_k_egt_ptr);

    egt_position_type wtm_wwin(white, {Ra1, Kh1, kh8}, no_ep);
    EXPECT_EQ(win, KR_vs_k_egt_ptr->probe_after_canonicalizing(wtm_wwin));
    egt_position_type wtm_wdrw(white, {Ka5, ke5, rb4}, no_ep);
    EXPECT_EQ(draw, KR_vs_k_egt_ptr->probe_after_canonicalizing(wtm_wdrw));
    egt_position_type wtm_wlos(white, {Kg2, ke5, rf6}, no_ep);
    EXPECT_EQ(loss, KR_vs_k_egt_ptr->probe_after_canonicalizing(wtm_wlos));
    egt_position_type btm_wwin(black, {Kc3, Rd4, ke5}, no_ep);
    EXPECT_EQ(win, KR_vs_k_egt_ptr->probe_after_canonicalizing(btm_wwin));
    egt_position_type btm_wdrw(black, {Kh4, Rd3, kc4}, no_ep);
    EXPECT_EQ(draw, KR_vs_k_egt_ptr->probe_after_canonicalizing(btm_wdrw));
    egt_position_type btm_wlos(black, {Kf6, kf4, rf3}, no_ep);
    EXPECT_EQ(loss, KR_vs_k_egt_ptr->probe_after_canonicalizing(btm_wlos));
}


TEST(IndexedCubeListEndgameTableType, ProbeKQVskD01I) {
    std::optional<material_signature_type> KQ_vs_k_opt{
     material_signature_type::create_opt("KQ_vs_k")};
    ASSERT_TRUE(KQ_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KQ_vs_k_opt->as_string(), "D01I", true));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<indexed_cube_list_endgame_table_type> KQ_vs_k_egt_ptr{
     indexed_cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KQ_vs_k_egt_ptr);

    egt_position_type wtm_wwin(white, {Qc6, Ke6, kg6}, no_ep);
    EXPECT_EQ(win, KQ_vs_k_egt_ptr->probe_after_canonicalizing(wtm_wwin));
    egt_position_type wtm_wdrw(white, {Ka1, qb2, kh8}, no_ep);
    EXPECT_EQ(draw, KQ_vs_k_egt_ptr->probe_after_canonicalizing(wtm_wdrw));
    egt_position_type wtm_wlos(white, {Ka1, qg7, kh8}, no_ep);
    EXPECT_EQ(loss, KQ_vs_k_egt_ptr->probe_after_canonicalizing(wtm_wlos));
    egt_position_type btm_wlos(black, {qc6, Ke7, kg6}, no_ep);
    EXPECT_EQ(loss, KQ_vs_k_egt_ptr->probe_after_canonicalizing(btm_wlos));
    egt_position_type btm_wdrw_1(black, {Ka8, Qg2, kh1}, no_ep);
    EXPECT_EQ(draw, KQ_vs_k_egt_ptr->probe_after_canonicalizing(btm_wdrw_1));
    egt_position_type btm_wdrw_2(black, {Qc6, Kf6, kd6}, no_ep);
    EXPECT_EQ(draw, KQ_vs_k_egt_ptr->probe_after_canonicalizing(btm_wdrw_2));
    egt_position_type btm_wwin(black, {Qc6, Ke6, kg6}, no_ep);
    EXPECT_EQ(win, KQ_vs_k_egt_ptr->probe_after_canonicalizing(btm_wwin));

    // Checking against unknown is safe for D00 or D01, but not in general.
    egt_position_type btm_bad(black, {qc6, Ke6, kg6}, no_ep);
    EXPECT_EQ(unknown, KQ_vs_k_egt_ptr->probe_after_canonicalizing(btm_bad));
}
