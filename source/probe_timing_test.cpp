#include "probe_timing_test.hpp"


#include <chrono>
#include <fstream>

#include "unxz.hpp"

#include "generic_probe_timing.hpp"

#include "representation/egt_position_type.hpp"
#include "egt_format_type.hpp"
#include "endgame_tables_type.hpp"
#include "probe_timing.hpp"


using namespace representation;


TEST(ProbeTimingTest, ProbeRawChesspressoKh5Qc7kh7btm) {
    chesspresso_binary_egt_format_type const egt_format;
    endgame_tables_type egts(3, egt_format, true);
    probe_report_type const wpr_expected{win};
    egt_position_type const egt_position(black, {Kh5, Qc5, kh7}, no_ep);
    probe_report_type const wpr_probe_report{egt_probe(egts, egt_position)};
    EXPECT_EQ(wpr_expected, wpr_probe_report);
}


TEST(ProbeTimingTest, ProbeRawChesspressoKh5Qc7kh7wtm) {
    chesspresso_binary_egt_format_type const egt_format;
    endgame_tables_type egts(3, egt_format, true);
    probe_report_type const wpr_expected{win};
    egt_position_type const egt_position(white, {Kh5, Qc5, kh7}, no_ep);
    probe_report_type const wpr_probe_report{egt_probe(egts, egt_position)};
    EXPECT_EQ(wpr_expected, wpr_probe_report);
}


TEST(ProbeTimingTest, ProbeRawChesspressokh5qc7Kh7btm) {
    chesspresso_binary_egt_format_type const egt_format;
    endgame_tables_type egts(3, egt_format, true);
    probe_report_type const wpr_expected{loss};
    egt_position_type const egt_position(black, {kh5, qc5, Kh7}, no_ep);
    probe_report_type const wpr_probe_report{egt_probe(egts, egt_position)};
    EXPECT_EQ(wpr_expected, wpr_probe_report);
}


TEST(ProbeTimingTest, ProbeRawChesspressokh5qc7Kh7wtm) {
    chesspresso_binary_egt_format_type const egt_format;
    endgame_tables_type egts(3, egt_format, true);
    probe_report_type const wpr_expected{loss};
    egt_position_type const egt_position(white, {kh5, qc5, Kh7}, no_ep);
    probe_report_type const wpr_probe_report{egt_probe(egts, egt_position)};
    EXPECT_EQ(wpr_expected, wpr_probe_report);
}
