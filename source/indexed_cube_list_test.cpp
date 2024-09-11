#include "indexed_cube_list_test.hpp"


#include <filesystem>
#include <iomanip>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>

#include "cube_binary_inputs.hpp"
#include "logic_digit_enum.hpp"
#include "look_up_parameterized_testing_type.hpp"


INSTANTIATE_TEST_SUITE_P(IndexedCubeList, look_up_parameterized_testing_type,
 testing::Values(
    look_up_bundle_type{"K_vs_k",  "",     cube_binary_inputs_tt<32>(lde_dc, 13), {2}},
    look_up_bundle_type{"K_vs_k",  "D01I", cube_binary_inputs_tt<32>(lde_dc, 13), {2}},
    look_up_bundle_type{"KB_vs_k", "",     cube_binary_inputs_tt<32>(lde_dc, 19), {2}},
    look_up_bundle_type{"KB_vs_k", "D01I", cube_binary_inputs_tt<32>(lde_dc, 19), {2}},
    look_up_bundle_type{"KN_vs_k", "",     cube_binary_inputs_tt<32>(lde_dc, 19), {2}},
    look_up_bundle_type{"KN_vs_k", "D01I", cube_binary_inputs_tt<32>(lde_dc, 19), {2}},
    look_up_bundle_type{"KP_vs_k", "",     cube_binary_inputs_tt<32>(lde_dc, 19), {2, 4}},
    look_up_bundle_type{"KP_vs_k", "D01I", cube_binary_inputs_tt<32>(lde_dc, 19), {2, 4}},
    look_up_bundle_type{"KQ_vs_k", "",     cube_binary_inputs_tt<32>(lde_dc, 19), {2, 4}},
    look_up_bundle_type{"KQ_vs_k", "D01I", cube_binary_inputs_tt<32>(lde_dc, 19), {2, 4}},
    look_up_bundle_type{"KR_vs_k", "",     cube_binary_inputs_tt<32>(lde_dc, 19), {2, 4}},
    look_up_bundle_type{"KR_vs_k", "D01I", cube_binary_inputs_tt<32>(lde_dc, 19), {2, 4}}));
