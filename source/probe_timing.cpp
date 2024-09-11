#include "probe_timing.hpp"


#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "execution_measurement.hpp"

#include "probe_report_type.hpp"
#include "probe_timing_epd_parsing.hpp"
#include "probing.hpp"

#include "representation/board_8x8_type.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/figurines_type.hpp"
#include "chesspresso_probe_timing_epd_parsing.hpp"


using representation::black;
using representation::board_8x8_type;
using representation::egt_position_type;
using representation::figurines_type;
using representation::material_signature_type;
using std::chrono::steady_clock;


std::vector<egt_position_type> determine_egt_positions(
    std::vector<epd::extended_position_description_type> const & epds
) {
    std::vector<egt_position_type> result;
    for (epd::extended_position_description_type const & epd: epds) {
        result.emplace_back(egt_position_type(epd));
    }
    return result;
}


std::pair<std::optional<egt_position_type>, std::optional<probe_report_type>>
parse_epd_test_data_line(std::string const & current_epd_line) {
    std::vector<std::string> const epd_segments{segment_epd(current_epd_line)};
    assert(epd_segment_limit == epd_segments.size());
    std::optional<egt_position_type> position_opt{
     position_opt_from<egt_position_type>(epd_segments)};
    std::optional<probe_report_type> wpr_probe_report_opt{
     probe_report_opt_from(epd_segments)};
    return std::make_pair(std::move(position_opt),
     std::move(wpr_probe_report_opt));
}


std::vector<material_signature_type> canonical_material_signatures(
    std::vector<epd::extended_position_description_type> const & epds
) {
    std::vector<material_signature_type> result;
    for (epd::extended_position_description_type const & epd: epds) {
        figurines_type const figurines(epd.board_contents());
        std::optional<material_signature_type> signature_opt{
         material_signature_type::create_opt(figurines)};
        assert(signature_opt);
        signature_opt->canonicalize();
        result.push_back(std::move(*signature_opt));
    }
    return result;
}


std::unordered_map<
    material_signature_type,
    probing_summary_statistics_type
> summarize_probe_timings_by_material_signature(
    std::vector<bool> const & has_been_read_in,
    std::vector<std::chrono::steady_clock::duration> const & probe_durations,
    std::vector<material_signature_type> const & canonicalized_material_signatures
) {
    assert(has_been_read_in.size() == probe_durations.size());
    assert(has_been_read_in.size() == canonicalized_material_signatures.size());

    std::unordered_map<representation::material_signature_type,
     probing_summary_statistics_type> result;

    for (std::size_t nth_probe{}; nth_probe != has_been_read_in.size();
     ++nth_probe) {
        if (not has_been_read_in[nth_probe]) {
            continue;
        }
        std::chrono::duration<long double, std::nano> probe_duration(
         probe_durations[nth_probe]);
        material_signature_type const signature(
         canonicalized_material_signatures[nth_probe]);
        assert(signature.canonical());
        auto iter(result.find(signature));
        if (iter == result.end()) {
            // No summary statistics object for this material signature exists
            // yet, so we make one.
            probing_summary_statistics_type probe_summary_stats;
            result[signature] = probe_summary_stats;
            iter = result.find(signature);
            assert(result.end() != iter);
        }
        result[signature].include(probe_duration);
    }
    return result;
}
