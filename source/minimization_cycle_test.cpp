#include "minimization_cycle_test.hpp"


#include <optional>

#include "minimization_cycle_parameterized_testing_type.hpp"


INSTANTIATE_TEST_SUITE_P(MinimizationCycle,
 minimization_cycle_parameterized_testing_type, testing::Values(
    minimization_cycle_bundle_type{"K_vs_k", "",     true, true, false, std::nullopt},
    minimization_cycle_bundle_type{"K_vs_k", "D01I", true, true, false, std::nullopt},
    minimization_cycle_bundle_type{"K_vs_k", "",     true, true, false, 0},
    minimization_cycle_bundle_type{"K_vs_k", "D01I", true, true, false, 0},
    minimization_cycle_bundle_type{"K_vs_k", "",     true, true, false, 123456789},
    minimization_cycle_bundle_type{"K_vs_k", "D01I", true, true, false, 123456789},
    minimization_cycle_bundle_type{"K_vs_k", "",     true, true, false, 987654321},
    minimization_cycle_bundle_type{"K_vs_k", "D01I", true, true, false, 987654321}

/* These tests run too slowly on a home computer.
    minimization_cycle_bundle_type{"KQ_vs_k", "D01I", true, true, false, std::nullopt},
    minimization_cycle_bundle_type{"KR_vs_k", "D01I", true, true, false, std::nullopt},
    minimization_cycle_bundle_type{"KB_vs_k", "D01I", true, true, false, std::nullopt},
    minimization_cycle_bundle_type{"KN_vs_k", "D01I", true, true, false, std::nullopt},
    minimization_cycle_bundle_type{"KP_vs_k", "D01I", true, true, false, std::nullopt}
*/
));
