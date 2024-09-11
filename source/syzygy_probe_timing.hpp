#if !defined(EGT_COMPARISON_SYZYGY_PROBE_TIMING_HPP)
#define EGT_COMPARISON_SYZYGY_PROBE_TIMING_HPP


#include <chrono>
#include <iosfwd>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "epd/extended_position_description_type.hpp"
#include "memory_information_snapshotter_type.hpp"

#include "probe_timing_epd_parsing.hpp"
#include "probe_report_type.hpp"
#include "probing.hpp"
#include "tb_position_type.hpp"


// Returns std::nullopt on success, otherwise, the EPD record that did not
// contain sufficient information to fully set up tb_position_type.
std::optional<epd::extended_position_description_type> determine_tb_positions(
    std::vector<epd::extended_position_description_type> const & epds,
    std::vector<tb_position_type> & tb_positions);


// Returns false iff the probe reports for all positions were as expected.
bool vet_syzygy_probe_reports(
    std::vector<tb_position_type> const & positions,
    std::vector<epd::extended_position_description_type> const & epds,
    std::vector<probe_report_type> const & expected_probe_reports,
    std::vector<probe_report_type> & probe_reports,
    std::vector<std::chrono::steady_clock::duration> & probe_durations);


void time_syzygy_probes(
    std::vector<tb_position_type> const & positions,
    std::vector<probe_report_type> & probe_reports,
    std::vector<std::chrono::steady_clock::duration> & probe_durations);


// Returns false iff timings are properly captured.
bool vet_and_time_syzygy_probes(
    std::vector<tb_position_type> const & positions,
    std::vector<epd::extended_position_description_type> const & epds,
    std::vector<probe_report_type> const & expected_probe_reports,
    std::vector<std::chrono::steady_clock::duration> & probe_durations,
    memory_information_snapshotter_type * snapshotter_ptr);


#endif // EGT_COMPARISON_SYZYGY_PROBE_TIMING_HPP
