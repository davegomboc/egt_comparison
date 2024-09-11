#include "irreduce_algorithm_test.hpp"


#include "cube_list.hpp"
#include "distance_d_merging.hpp"
#include "indexed_cube_list.hpp"
#include "load_and_save.hpp"
#include "verify_equivalence.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


TEST(IrreduceAlgorithm, KQVskD00AndD00I) {
    std::optional<cube_list_type> cubes_to_simplify_opt{load_cube_list_opt<32, 8>("KQ_vs_k", "")};
    ASSERT_NE(std::nullopt, cubes_to_simplify_opt);

    cube_list_type const KQ_vs_k_D00(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D00_icl{KQ_vs_k_D00};
    std::cerr << "II: D00 and D00_icl have been built.\n";

    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00_icl, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00_icl, KQ_vs_k_D00_icl));

    bool const problem_making_KQ_vs_K_D00I{
     irreduce(*cubes_to_simplify_opt, KQ_vs_k_D00_icl, true, false, true)};
    EXPECT_FALSE(problem_making_KQ_vs_K_D00I);

    cube_list_type const KQ_vs_k_D00I(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D00I_icl{KQ_vs_k_D00I};
    std::cerr << "II: D00I and D00I_icl have been built.\n";

    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00I, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00I_icl, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00I, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00I_icl, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00I, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00I_icl, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00I, KQ_vs_k_D00I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D00I_icl, KQ_vs_k_D00I_icl));
}


TEST(IrreduceAlgorithm, KQVskD01AndD01I) {
    std::optional<cube_list_type> cubes_to_simplify_opt{load_cube_list_opt<32, 8>("KQ_vs_k", "")};
    ASSERT_NE(std::nullopt, cubes_to_simplify_opt);

    cube_list_type const KQ_vs_k_D00(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D00_icl{KQ_vs_k_D00};

    irreduce(*cubes_to_simplify_opt, KQ_vs_k_D00_icl, true, false, true);

    cube_list_type const KQ_vs_k_D00I(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D00I_icl{KQ_vs_k_D00I};

    distance_d_merge(*cubes_to_simplify_opt, 1, no_reference_set(), 1);
    cube_list_type const KQ_vs_k_D01{*cubes_to_simplify_opt};
    indexed_cube_list_type const KQ_vs_k_D01_icl{KQ_vs_k_D01};
    std::cerr << "II: D01 and D01_icl have been built.\n";

    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01_icl, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01_icl, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01_icl, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01, KQ_vs_k_D00I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01_icl, KQ_vs_k_D00I_icl));

    bool const problem_making_KQ_vs_K_D01I{
     irreduce(*cubes_to_simplify_opt, KQ_vs_k_D00_icl, true, false, true)};
    EXPECT_FALSE(problem_making_KQ_vs_K_D01I);

    cube_list_type const KQ_vs_k_D01I(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D01I_icl{KQ_vs_k_D01I};
    std::cerr << "II: D01I and D01I_icl have been built.\n";

    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I_icl, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I_icl, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I_icl, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I, KQ_vs_k_D00I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I_icl, KQ_vs_k_D00I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I, KQ_vs_k_D01));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I_icl, KQ_vs_k_D01));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I, KQ_vs_k_D01_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I_icl, KQ_vs_k_D01_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I, KQ_vs_k_D01I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I_icl, KQ_vs_k_D01I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I, KQ_vs_k_D01I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D01I_icl, KQ_vs_k_D01I_icl));
}


TEST(IrreduceAlgorithm, KQVskD02AndD02I) {
    std::optional<cube_list_type> cubes_to_simplify_opt{load_cube_list_opt<32, 8>("KQ_vs_k", "")};
    ASSERT_NE(std::nullopt, cubes_to_simplify_opt);

    cube_list_type const KQ_vs_k_D00(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D00_icl{KQ_vs_k_D00};

    irreduce(*cubes_to_simplify_opt, KQ_vs_k_D00_icl, true, false, true);

    cube_list_type const KQ_vs_k_D00I(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D00I_icl{KQ_vs_k_D00I};

    distance_d_merge(*cubes_to_simplify_opt, 1, no_reference_set(), 1);
    cube_list_type const KQ_vs_k_D01{*cubes_to_simplify_opt};
    indexed_cube_list_type const KQ_vs_k_D01_icl{KQ_vs_k_D01};

    irreduce(*cubes_to_simplify_opt, KQ_vs_k_D00_icl, true, false, true);

    cube_list_type const KQ_vs_k_D01I(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D01I_icl{KQ_vs_k_D01I};

    distance_d_merge(*cubes_to_simplify_opt, 2, KQ_vs_k_D01I, 1);
    cube_list_type const KQ_vs_k_D02{*cubes_to_simplify_opt};
    indexed_cube_list_type const KQ_vs_k_D02_icl{KQ_vs_k_D02};
    std::cerr << "II: D02 and D02_icl have been built.\n";

    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02_icl, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02_icl, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02_icl, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02, KQ_vs_k_D00I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02_icl, KQ_vs_k_D00I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02, KQ_vs_k_D01));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02_icl, KQ_vs_k_D01));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02, KQ_vs_k_D01_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02_icl, KQ_vs_k_D01_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02, KQ_vs_k_D01I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02_icl, KQ_vs_k_D01I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02, KQ_vs_k_D01I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02_icl, KQ_vs_k_D01I_icl));

    bool const problem_making_KQ_vs_K_D02I{
     irreduce(*cubes_to_simplify_opt, KQ_vs_k_D00_icl, true, false, true)};
    EXPECT_FALSE(problem_making_KQ_vs_K_D02I);

    cube_list_type const KQ_vs_k_D02I(*cubes_to_simplify_opt);
    indexed_cube_list_type const KQ_vs_k_D02I_icl{KQ_vs_k_D02I};
    std::cerr << "II: D02I and D02I_icl have been built.\n";

    // TODO: Determine whether any why the eight checks below should
    // be expected to fail when D01_icl is used as the reference set
    // when building KQ_vs_k_D02I and KQ_vs_k_D02I_icl.
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I_icl, KQ_vs_k_D00));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I_icl, KQ_vs_k_D00_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I_icl, KQ_vs_k_D00I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I, KQ_vs_k_D00I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I_icl, KQ_vs_k_D00I_icl));
    // TODO: Determine whether any why the eight checks above should
    // be expected to fail when D01_icl is used as the reference set
    // when building KQ_vs_k_D02I and KQ_vs_k_D02I_icl.
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I, KQ_vs_k_D01));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I_icl, KQ_vs_k_D01));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I, KQ_vs_k_D01_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I_icl, KQ_vs_k_D01_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I, KQ_vs_k_D01I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I_icl, KQ_vs_k_D01I));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I, KQ_vs_k_D01I_icl));
    EXPECT_FALSE(verify_equivalence(KQ_vs_k_D02I_icl, KQ_vs_k_D01I_icl));
}
