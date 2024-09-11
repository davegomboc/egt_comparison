#include "probe_report_counters_type_test.hpp"


TEST(ProbeReportCountersType, DumpCounts) {
    epd::side_to_move_type const white_to_move(false);
    epd::side_to_move_type const black_to_move(true);

    probe_report_counters_type counters;
    for (unsigned int i{}; i != probe_report_enum_limit; ++i) {
        for (unsigned int j{}; j != i; ++j) {
            counters.value(white_to_move, false, probe_report_type(i)) += 9;
            counters.value(white_to_move, true, probe_report_type(i)) += 1;
        }
        for (unsigned int j{i}; j != 9; ++j) {
            counters.value(black_to_move, false, probe_report_type(i)) += 9;
            counters.value(black_to_move, true, probe_report_type(i)) += 1;
        }
    }
    counters.dump(std::cerr);
}
