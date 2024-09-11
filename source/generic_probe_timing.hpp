#if !defined(EGT_COMPARISON_GENERIC_PROBE_TIMING_HPP)
#define EGT_COMPARISON_GENERIC_PROBE_TIMING_HPP


#include <chrono>
#include <iosfwd>
#include <optional>
#include <string>
#include <vector>

#include "epd/extended_position_description_type.hpp"

#include "probe_report_type.hpp"


// Returns std::nullopt on success, or the problematic line that was
// not parseable as EPD if an error is detected.
std::optional<std::string> ingest_lines_of_epd_strings(
    std::istream & in,
    std::vector<epd::extended_position_description_type> & epds);


// Returns std::nullopt on success, otherwise, the EPD record that did
// not contain valid game-theoretic outcome information.
std::optional<epd::extended_position_description_type> determine_expected_probe_reports(
    std::vector<epd::extended_position_description_type> const & epds,
    std::vector<probe_report_type> & expected_probe_reports,
    bool convert_expected_probe_reports_to_white_player_perspective);


void emit_probe_timings(
    std::ostream & out,
    std::vector<std::chrono::steady_clock::duration> const & probe_durations,
    std::vector<epd::extended_position_description_type> const & epds);


void read_probe_timings(
    std::istream & in,
    std::vector<bool> & has_been_read_in,
    std::vector<std::chrono::steady_clock::duration> & probe_durations,
    std::vector<epd::extended_position_description_type> & epds,
    std::size_t expected_required_size);


#endif // EGT_COMPARISON_GENERIC_PROBE_TIMING_HPP
