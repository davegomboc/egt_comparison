#include "paths_test.hpp"


#include <optional>

#include "representation/figurine_enum.hpp"
#include "representation/material_signature_type.hpp"


using representation::figurine_colour_enum;
using representation::material_signature_type;


TEST(Paths, UncompressedChesspressoPathSuffixKPVsk) {
    std::optional<material_signature_type> KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt);
    std::filesystem::path const path_suffix(
     chesspresso_path_suffix(*KP_vs_k_opt, std::nullopt, true, false));
    EXPECT_EQ("3/KP_vs_k.plb", path_suffix.string());
}

TEST(Paths, XZCompressedChesspressoPathSuffixKRVsk) {
    std::optional<material_signature_type> KR_vs_k_opt{
     material_signature_type::create_opt("KR_vs_k")};
    ASSERT_TRUE(KR_vs_k_opt);
    std::filesystem::path const path_suffix(
     chesspresso_path_suffix(*KR_vs_k_opt, std::nullopt, true, true));
    EXPECT_EQ("3/KR_vs_k.plb.xz", path_suffix.string());
}

TEST(Paths, XZCompressedEspressoTextPathSuffixKPPVskppWithoutDontCare) {
    std::optional<material_signature_type> KPP_vs_kpp_opt{
     material_signature_type::create_opt("KPP_vs_kpp")};
    ASSERT_TRUE(KPP_vs_kpp_opt);
    std::filesystem::path const path_suffix(
     chesspresso_path_suffix(*KPP_vs_kpp_opt, "fr", false, true));
    EXPECT_EQ("6/KPP_vs_kpp.fr.pla.xz", path_suffix.string());
}

TEST(Paths, UncompressedEspressoTextPathSuffixKPPVskppWithDontCare) {
    std::optional<material_signature_type> KPP_vs_kpp_opt{
     material_signature_type::create_opt("KPP_vs_kpp")};
    ASSERT_TRUE(KPP_vs_kpp_opt);
    std::filesystem::path const path_suffix(
     chesspresso_path_suffix(*KPP_vs_kpp_opt, "fdr", false, false));
    EXPECT_EQ("6/KPP_vs_kpp.fdr.pla", path_suffix.string());
}

TEST(Paths, UncompressedDecisionDagPathSuffixKVsk) {
    std::optional<material_signature_type> K_vs_k_opt{
     material_signature_type::create_opt("K_vs_k")};
    ASSERT_TRUE(K_vs_k_opt);
    std::filesystem::path const path_suffix(
     decision_dag_path_suffix(*K_vs_k_opt, false));
    EXPECT_EQ("2/K_vs_k.dtr", path_suffix.string());
}

TEST(Paths, UncompressedDecisionDagPathSuffixKPVsk) {
    std::optional<material_signature_type> KP_vs_k_opt{
     material_signature_type::create_opt("KP_vs_k")};
    ASSERT_TRUE(KP_vs_k_opt);
    std::filesystem::path const path_suffix(
     decision_dag_path_suffix(*KP_vs_k_opt, false));
    EXPECT_EQ("3/KP_vs_k.dtr", path_suffix.string());
}
