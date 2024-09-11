#include "load_and_save_test.hpp"


#include <cstdint>
#include <optional>

using cube_list_type = cube_list_tt<32, 8>;


TEST(LoadAndSave, PathForKVskDtreeObtained) {
    auto const K_vs_k_ddag_path_opt(decision_dag_path_opt("K_vs_k"));
    EXPECT_TRUE(K_vs_k_ddag_path_opt);

    // (Just for convenience.)
    if (K_vs_k_ddag_path_opt) {
        std::cout << *K_vs_k_ddag_path_opt << std::endl;
    }
}


TEST(LoadAndSave, InterpretOutputBitsOfChesspressoFormatKQVsk) {
    std::optional<std::filesystem::path> KQ_vs_k_bin_path_opt{
     chesspresso_path_opt("KQ_vs_k")};
    ASSERT_TRUE(KQ_vs_k_bin_path_opt);
    std::ifstream data_in(open_file_read(*KQ_vs_k_bin_path_opt));
    cube_list_type const F_cubes{load_chesspresso_F_plb<32, 8>(data_in)};
    // bit 2 == wwin, bit 1 == wdrw, bit 0 = wlos.
    // canonicalized KQ_vs_k always has white to move and with extra queen,
    // so wlos should never be set.
    for (cube_list_type::cube_type const & cube: F_cubes) {
        EXPECT_EQ(0, 0b11111001 & cube.outputs());
    }
}


TEST(LoadAndSave, LoadCubeListKVsk) {
    std::optional<cube_list_type> const cube_list_opt{
     load_cube_list_opt<32, 8>("K_vs_k", "")};
    ASSERT_TRUE(cube_list_opt);
}


TEST(LoadAndSave, LoadCubeListKVskD01I) {
    std::optional<cube_list_type> const cube_list_opt{
     load_cube_list_opt<32, 8>("K_vs_k", "D01I")};
    ASSERT_TRUE(cube_list_opt);
}
