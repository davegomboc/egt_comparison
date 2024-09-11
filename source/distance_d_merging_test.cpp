#include "distance_d_merging_test.hpp"


#include "cube_list.hpp"
#include "indexed_cube_list.hpp"
#include "load_and_save.hpp"
#include "verify_equivalence.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


TEST(DistanceDMerging, KQVskD01) {
    auto KQ_vs_k_opt{load_cube_list_opt<32, 8>("KQ_vs_k", "")};
    ASSERT_NE(std::nullopt, KQ_vs_k_opt);
    cube_list_type const KQ_vs_k{*KQ_vs_k_opt};
    distance_d_merge(*KQ_vs_k_opt, 1, no_reference_set(), 1);
    bool const D01_equivalence_problem_found{verify_equivalence(*KQ_vs_k_opt, KQ_vs_k)};
    EXPECT_FALSE(D01_equivalence_problem_found);
}


TEST(DistanceDMerging, KQVskD02) {
    auto KQ_vs_k_opt{load_cube_list_opt<32, 8>("KQ_vs_k", "")};
    ASSERT_NE(std::nullopt, KQ_vs_k_opt);
    cube_list_type const KQ_vs_k{*KQ_vs_k_opt};
    distance_d_merge(*KQ_vs_k_opt, 1, no_reference_set(), 1);
    bool const D01_equivalence_problem_found{verify_equivalence(*KQ_vs_k_opt, KQ_vs_k)};
    ASSERT_FALSE(D01_equivalence_problem_found);
    cube_list_type KQ_vs_k_D01(*KQ_vs_k_opt);
    distance_d_merge(*KQ_vs_k_opt, 2, KQ_vs_k_D01, 1);
    bool const D02_equivalence_problem_found{verify_equivalence(*KQ_vs_k_opt, KQ_vs_k_D01)};
    EXPECT_FALSE(D02_equivalence_problem_found);
}
