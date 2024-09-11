#include "chesspresso_probe_timing_epd_parsing_test.hpp"


#include <optional>
#include <string>
#include <vector>

#include "representation/egt_position_type.hpp"


using namespace representation;


TEST(ChesspressoProbeTimingEpdParsingTest, EgtPositionFromEpdSegments) {
    std::vector<std::string> const epd_segments{
     "8/7k/8/2Q4K/8/7N/8/8", "b", "-", "-", "0", "1", "l"};
    egt_position_type const expected(black, {Kh5, Qc5, Nh3, kh7}, no_ep);
    std::optional<egt_position_type> const egt_position_opt{
     position_opt_from<egt_position_type>(epd_segments)};
    ASSERT_TRUE(egt_position_opt);
    EXPECT_EQ(expected, *egt_position_opt);
}
