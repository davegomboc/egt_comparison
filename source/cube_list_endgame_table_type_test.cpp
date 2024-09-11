#include "cube_list_endgame_table_type_test.hpp"

#include <filesystem>
#include <memory>
#include <optional>

#include "representation/material_signature_type.hpp"

#include "cube_list_endgame_table_parameterized_testing_type.hpp"
#include "paths.hpp"


using namespace representation;


TEST(CubeListEndgameTableType, ProbeKVsk) {
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(K_vs_k_opt->as_string(), "", false));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<cube_list_endgame_table_type> K_vs_k_egt_ptr{
     cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(K_vs_k_egt_ptr);
}


TEST(CubeListEndgameTableType, ProbeKBVskD01I) {
    std::optional<material_signature_type> KB_vs_k_opt{
     material_signature_type::create_opt("KB_vs_k")};
    ASSERT_TRUE(KB_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KB_vs_k_opt->as_string(), "D01I", false));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<cube_list_endgame_table_type> KB_vs_k_egt_ptr{
     cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KB_vs_k_egt_ptr);
}


TEST(CubeListEndgameTableType, ProbeKNVskD01I) {
    std::optional<material_signature_type> KN_vs_k_opt{
     material_signature_type::create_opt("KN_vs_k")};
    ASSERT_TRUE(KN_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KN_vs_k_opt->as_string(), "D01I", false));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<cube_list_endgame_table_type> KN_vs_k_egt_ptr{
     cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KN_vs_k_egt_ptr);
}


TEST(CubeListEndgameTableType, ProbeKPVskD01I) {
    std::optional<material_signature_type> KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KP_vs_k_opt->as_string(), "D01I", false));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<cube_list_endgame_table_type> KP_vs_k_egt_ptr{
     cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KP_vs_k_egt_ptr);
}


TEST(CubeListEndgameTableType, ProbeKRVskD01I) {
    std::optional<material_signature_type> KR_vs_k_opt{
     material_signature_type::create_opt("KR_vs_k")};
    ASSERT_TRUE(KR_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KR_vs_k_opt->as_string(), "D01I", false));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<cube_list_endgame_table_type> KR_vs_k_egt_ptr{
     cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KR_vs_k_egt_ptr);
}


TEST(CubeListEndgameTableType, ProbeKQVskD01I) {
    std::optional<material_signature_type> KQ_vs_k_opt{
     material_signature_type::create_opt("KQ_vs_k")};
    ASSERT_TRUE(KQ_vs_k_opt);
    std::optional<std::filesystem::path> const path_opt(
     cube_list_path_opt(KQ_vs_k_opt->as_string(), "D01I", false));
    ASSERT_TRUE(path_opt);
    std::unique_ptr<cube_list_endgame_table_type> KQ_vs_k_egt_ptr{
     cube_list_endgame_table_type::create_ptr(*path_opt)};
    ASSERT_TRUE(KQ_vs_k_egt_ptr);
}


INSTANTIATE_TEST_SUITE_P(CubeListEndgameTableType,
 cube_list_endgame_table_parameterized_testing_type,
 testing::ValuesIn(legal_wcdbl_test_positions()));
