#include "decision_dag_endgame_table_type_test.hpp"


#include <filesystem>
#include <memory>
#include <optional>

#include "representation/material_signature_type.hpp"

#include "decision_dag_endgame_table_parameterized_testing_type.hpp"
#include "paths.hpp"


using namespace representation;


TEST(DecisionDagEndgameTableType, ProbeKVsk) {
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt);
    std::filesystem::path const path(decision_dag_path(*K_vs_k_opt, false));
    std::unique_ptr<decision_dag_endgame_table_type> K_vs_k_egt_ptr{
      decision_dag_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(K_vs_k_egt_ptr);
}


TEST(DecisionDagEndgameTableType, ProbeKBVsk) {
    std::optional<material_signature_type> KB_vs_k_opt{
     material_signature_type::create_opt("KB_vs_k")};
    ASSERT_TRUE(KB_vs_k_opt);
    std::filesystem::path const path{decision_dag_path(*KB_vs_k_opt, false)};
    std::unique_ptr<decision_dag_endgame_table_type> KB_vs_k_egt_ptr{
     decision_dag_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KB_vs_k_egt_ptr);
}


TEST(DecisionDagEndgameTableType, ProbeKNVsk) {
    std::optional<material_signature_type> KN_vs_k_opt{
     material_signature_type::create_opt("KN_vs_k")};
    ASSERT_TRUE(KN_vs_k_opt);
    std::filesystem::path const path{decision_dag_path(*KN_vs_k_opt, false)};
    std::unique_ptr<decision_dag_endgame_table_type> KN_vs_k_egt_ptr{
     decision_dag_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KN_vs_k_egt_ptr);
}


TEST(DecisionDagEndgameTableType, ProbeKPVsk) {
    std::optional<material_signature_type> KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt);
    std::filesystem::path const path{decision_dag_path(*KP_vs_k_opt, false)};
    std::unique_ptr<decision_dag_endgame_table_type> KP_vs_k_egt_ptr{
     decision_dag_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KP_vs_k_egt_ptr);
}


TEST(DecisionDagEndgameTableType, ProbeKQVsk) {
    std::optional<material_signature_type> KQ_vs_k_opt{
     material_signature_type::create_opt("KQ_vs_k")};
    ASSERT_TRUE(KQ_vs_k_opt);
    std::filesystem::path const path{decision_dag_path(*KQ_vs_k_opt, false)};
    std::unique_ptr<decision_dag_endgame_table_type> KQ_vs_k_egt_ptr{
     decision_dag_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KQ_vs_k_egt_ptr);
}


TEST(DecisionDagEndgameTableType, ProbeKRVsk) {
    std::optional<material_signature_type> KR_vs_k_opt{
     material_signature_type::create_opt("KR_vs_k")};
    ASSERT_TRUE(KR_vs_k_opt);
    std::filesystem::path const path{decision_dag_path(*KR_vs_k_opt, false)};
    std::unique_ptr<decision_dag_endgame_table_type> KR_vs_k_egt_ptr{
     decision_dag_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KR_vs_k_egt_ptr);
}


INSTANTIATE_TEST_SUITE_P(DecisionDagEndgameTableType,
 decision_dag_endgame_table_parameterized_testing_type,
 testing::ValuesIn(legal_wcdbl_test_positions()));
