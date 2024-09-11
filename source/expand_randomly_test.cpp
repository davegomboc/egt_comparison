#include "expand_randomly_test.hpp"


#include "expand_randomly_parameterized_testing_type.hpp"


INSTANTIATE_TEST_SUITE_P(ExpandRandomly,
 expand_randomly_parameterized_testing_type, testing::Values(
    expand_randomly_bundle_type{"K_vs_k",  "D01I", 0},
    expand_randomly_bundle_type{"K_vs_k",  "D01I", 123456789},
    expand_randomly_bundle_type{"K_vs_k",  "D01I", 987654321},
    expand_randomly_bundle_type{"KB_vs_k", "D01I", 0},
    expand_randomly_bundle_type{"KB_vs_k", "D01I", 123456789},
    expand_randomly_bundle_type{"KB_vs_k", "D01I", 987654321},
    expand_randomly_bundle_type{"KN_vs_k", "D01I", 0},
    expand_randomly_bundle_type{"KN_vs_k", "D01I", 123456789},
    expand_randomly_bundle_type{"KN_vs_k", "D01I", 987654321},
    expand_randomly_bundle_type{"KP_vs_k", "D01I", 0},
    expand_randomly_bundle_type{"KP_vs_k", "D01I", 123456789},
    expand_randomly_bundle_type{"KP_vs_k", "D01I", 987654321},
    expand_randomly_bundle_type{"KQ_vs_k", "D01I", 0},
    expand_randomly_bundle_type{"KQ_vs_k", "D01I", 123456789},
    expand_randomly_bundle_type{"KQ_vs_k", "D01I", 987654321},
    expand_randomly_bundle_type{"KR_vs_k", "D01I", 0},
    expand_randomly_bundle_type{"KR_vs_k", "D01I", 123456789},
    expand_randomly_bundle_type{"KR_vs_k", "D01I", 987654321}));
