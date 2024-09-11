#include "bdd_endgame_table_type_test.hpp"


#include "probe_report_type.hpp"

#include "representation/egt_position_type.hpp"

#include "bdd_endgame_table_parameterized_testing_type.hpp"
#include "paths.hpp"


using namespace representation;


TEST(BddEndgameTableType, ProbeKVsk) {
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt);
    std::filesystem::path const path{bdd_path(*K_vs_k_opt, "none", false)};
    std::unique_ptr<bdd_endgame_table_type> const K_vs_k_egt_ptr{
     bdd_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(K_vs_k_egt_ptr);
}


TEST(BddEndgameTableType, ProbeKBVsk) {
    std::optional<material_signature_type> KB_vs_k_opt{
     material_signature_type::create_opt("KB_vs_k")};
    ASSERT_TRUE(KB_vs_k_opt);
    std::filesystem::path const path{bdd_path(*KB_vs_k_opt, "restr", false)};
    std::unique_ptr<bdd_endgame_table_type> const KB_vs_k_egt_ptr{
     bdd_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KB_vs_k_egt_ptr);
}


TEST(BddEndgameTableType, ProbeKNVsk) {
    std::optional<material_signature_type> KN_vs_k_opt{
     material_signature_type::create_opt("KN_vs_k")};
    ASSERT_TRUE(KN_vs_k_opt);
    std::filesystem::path const path{bdd_path(*KN_vs_k_opt, "osm", false)};
    std::unique_ptr<bdd_endgame_table_type> const KN_vs_k_egt_ptr{
     bdd_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KN_vs_k_egt_ptr);
}


TEST(BddEndgameTableType, ProbeKPVsk) {
    std::optional<material_signature_type> KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt);
    std::filesystem::path const path{bdd_path(*KP_vs_k_opt, "tsm", false)};
    std::unique_ptr<bdd_endgame_table_type> const KP_vs_k_egt_ptr{
     bdd_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KP_vs_k_egt_ptr);
}


TEST(BddEndgameTableType, ProbeKQVsk) {
    std::optional<material_signature_type> KQ_vs_k_opt{
     material_signature_type::create_opt("KQ_vs_k")};
    ASSERT_TRUE(KQ_vs_k_opt);
    std::filesystem::path const path{bdd_path(*KQ_vs_k_opt, "tsm", false)};
    std::unique_ptr<bdd_endgame_table_type> const KQ_vs_k_egt_ptr{
     bdd_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KQ_vs_k_egt_ptr);
}


TEST(BddEndgameTableType, ProbeKRVsk) {
    std::optional<material_signature_type> KR_vs_k_opt{
     material_signature_type::create_opt("KR_vs_k")};
    ASSERT_TRUE(KR_vs_k_opt);
    std::filesystem::path const path{bdd_path(*KR_vs_k_opt, "tsm", false)};
    std::unique_ptr<bdd_endgame_table_type> const KR_vs_k_egt_ptr{
     bdd_endgame_table_type::create_ptr(path)};
    ASSERT_TRUE(KR_vs_k_egt_ptr);
}


INSTANTIATE_TEST_SUITE_P(BddEndgameTableType,
 bdd_endgame_table_parameterized_testing_type,
 testing::ValuesIn(legal_wcdbl_test_positions()));
