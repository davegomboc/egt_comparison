#if !defined(EGT_COMPARISON_PROBE_TIMING_HPP)
#define EGT_COMPARISON_PROBE_TIMING_HPP


#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "epd/extended_position_description_type.hpp"
#include "representation/board_8x8_type.hpp"
#include "representation/canonicalization_enum.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/material_side_type.hpp"
#include "representation/material_signature_type.hpp"
#include "memory_information_snapshotter_type.hpp"
#include "primitive_bit_operations.hpp"
#include "probe_report_type.hpp"
#include "probing_summary_statistics_type.hpp"


std::vector<representation::egt_position_type>
determine_egt_positions(
    std::vector<epd::extended_position_description_type> const & epds);


// Probe reports are to be interpreted as being from White's perspective.
template <typename endgame_tables_type>
probe_report_type egt_probe(
    endgame_tables_type const & egts,
    representation::egt_position_type const & egt_position
) {
    representation::material_signature_type material_signature(
     egt_position.as_material_signature());
    material_signature.canonicalize();
    auto const * table_ptr{egts.get(material_signature)};
    if (not table_ptr) {
        std::cerr << "WW: " << material_signature << " table not found, so "
         "the game-theoretic value of " << egt_position << " is unknown.\n";
        assert(table_ptr);
        return unknown;
    }
    return table_ptr->probe_after_canonicalizing(egt_position);
}


// Returns false iff the probe reports for all positions were as expected.
template <typename endgame_tables_type>
bool vet_probe_reports(
    endgame_tables_type const & egts,
    std::vector<epd::extended_position_description_type> const & epds,
    std::vector<representation::egt_position_type> const & positions,
    std::vector<probe_report_type> const & expected_probe_reports,
    std::vector<probe_report_type> & probe_reports,
    std::vector<std::chrono::steady_clock::duration> & probe_durations,
    memory_information_snapshotter_type * snapshotter_ptr
) {
    bool result{};
    std::size_t const position_count{positions.size()};
    assert(position_count == expected_probe_reports.size());
    assert(position_count == probe_reports.size());
    assert(position_count == probe_durations.size());
    std::size_t const position_count_between_snapshots{
     (position_count + 7) / 8};
    if (snapshotter_ptr) {
        snapshotter_ptr->take_snapshot();
    }
    std::size_t position_count_until_next_snapshot{
     position_count_between_snapshots};
    for (std::size_t i{}; i != position_count; ++i) {
        auto const probe_start_time{std::chrono::steady_clock::now()};
        probe_reports[i] = egt_probe(egts, positions[i]);
        auto const probe_stop_time{std::chrono::steady_clock::now()};
        assert(probe_start_time <= probe_stop_time);
        probe_durations[i] = probe_stop_time - probe_start_time;
        bool print_details{};
        if (print_details) {
            std::cerr << "II:  original EPD = " << epds[i] << '\n';
        }
        if (probe_reports[i] == expected_probe_reports[i]) {
            if (print_details) {
                std::cerr << "\nII: Probe correctly returned "
                 << probe_reports[i].as_description() << " for position #" << i
                 << '\n';
            }
        } else {
            std::cerr << "\nEE: Probe returned "
             << probe_reports[i].as_description() << " instead of "
             << expected_probe_reports[i].as_description() << " for position #"
             << i;
            if (not print_details) {
                // The EPD record was only printed above if print_details was enabled,
                // but we always want to provide it when a probing error occurs.
                std::cerr << ": " << epds[i];
            }
            std::cerr << '\n';
            result = true;
        }
        if (print_details) {
            std::cerr << "II:  egt_position = " << positions[i] << '\n';
            representation::board_8x8_type const board{
             positions[i].placements()};
            std::cerr << board.as_renderable_string(false);
            auto const canonicalized{positions[i].canonical_position(
             representation::canonicalization_enum::all_symmetries)};
            std::cerr << "II: canonicalized = " << canonicalized << '\n';
            representation::board_8x8_type const canonicalized_board{
             canonicalized.placements()};
            std::cerr << canonicalized_board.as_renderable_string(false);
            std::cerr << "II:  logic digits = "
             << canonicalized.as_logic_digits() << '\n';
        }
        if (snapshotter_ptr) {
            --position_count_until_next_snapshot;
            if (0 == position_count_until_next_snapshot) {
                snapshotter_ptr->take_snapshot();
                position_count_until_next_snapshot
                 = position_count_between_snapshots;
            }
        }
        if ((i >= (1 << 24)) && has_single_bit(i)) {
            std::cerr << "TT: Vetted " << i << " positions.\n";
        }
    }
    if (snapshotter_ptr) {
        snapshotter_ptr->take_snapshot();
    }
    std::cerr << "TT: Completed vetting of " << position_count
     << " positions.\n";
    return result;
}


template <typename endgame_tables_type>
void time_probes(
    endgame_tables_type const & egts,
    std::vector<representation::egt_position_type> const & positions,
    std::vector<probe_report_type> & probe_reports,
    std::vector<std::chrono::steady_clock::duration> & probe_durations,
    memory_information_snapshotter_type * snapshotter_ptr
) {
    std::size_t const position_count{positions.size()};
    assert(position_count == probe_reports.size());
    assert(position_count == probe_durations.size());
    std::size_t const position_count_between_snapshots{
     (position_count + 7) / 8};
    if (snapshotter_ptr) {
        snapshotter_ptr->take_snapshot();
    }
    std::size_t position_count_until_next_snapshot{
     position_count_between_snapshots};
    for (std::size_t i{}; i != position_count; ++i) {
        probe_reports[i] = unknown;
        auto const probe_start_time{std::chrono::steady_clock::now()};
        probe_reports[i] = egt_probe(egts, positions[i]);
        auto const probe_stop_time{std::chrono::steady_clock::now()};
        assert(probe_start_time <= probe_stop_time);
        probe_durations[i] = probe_stop_time - probe_start_time;
        if (snapshotter_ptr) {
            --position_count_until_next_snapshot;
            if (0 == position_count_until_next_snapshot) {
                snapshotter_ptr->take_snapshot();
                position_count_until_next_snapshot
                 = position_count_between_snapshots;
            }
        }
    }
    if (snapshotter_ptr) {
        snapshotter_ptr->take_snapshot();
    }
}


// Returns false iff timings are properly captured.
template <typename endgame_tables_type>
bool vet_and_time_probes(
    endgame_tables_type const & egts,
    std::vector<epd::extended_position_description_type> const & epds,
    std::vector<representation::egt_position_type> const & positions,
    std::vector<probe_report_type> const & expected_probe_reports,
    std::vector<std::chrono::steady_clock::duration> & probe_durations,
    memory_information_snapshotter_type * snapshotter_ptr
) {
    std::size_t const position_count{positions.size()};
    assert(position_count == expected_probe_reports.size());
    probe_durations.clear();
    probe_durations.resize(position_count);

    // Check the probe reports while warming things up.
    auto const vetting_start_time{std::chrono::steady_clock::now()};
    std::vector<probe_report_type> probe_reports(position_count, unknown);
    bool const mismatch_found{vet_probe_reports(egts, epds, positions,
     expected_probe_reports, probe_reports, probe_durations, snapshotter_ptr)};
    if (mismatch_found) {
        probe_durations.clear();
        return true;
    }
    auto const vetting_stop_time{std::chrono::steady_clock::now()};
    assert(vetting_start_time <= vetting_stop_time);
    auto const vetting_duration{std::chrono::duration_cast<
     std::chrono::nanoseconds>(vetting_stop_time - vetting_start_time)};
    std::cerr << "II: vetting duration = " << vetting_duration.count()
     << " ns.\n";

    // Capture the actual probe timings.
    auto const timing_start_time{std::chrono::steady_clock::now()};
    time_probes(egts, positions, probe_reports, probe_durations,
     snapshotter_ptr);
    auto const timing_stop_time{std::chrono::steady_clock::now()};
    auto const timing_duration{std::chrono::duration_cast<
     std::chrono::nanoseconds>(timing_stop_time - timing_start_time)};
    std::cerr << "II: timing duration = " << timing_duration.count()
     << " ns.\n";

    return false;
}


std::vector<representation::material_signature_type>
canonical_material_signatures(
    std::vector<epd::extended_position_description_type> const & epds);


std::unordered_map<
    representation::material_signature_type,
    probing_summary_statistics_type
> summarize_probe_timings_by_material_signature(
    std::vector<bool> const & has_been_read_in,
    std::vector<std::chrono::steady_clock::duration> const & probe_durations,
    std::vector<representation::material_signature_type> const & canonical_material_signatures);


#endif // EGT_COMPARISON_PROBE_TIMING_HPP
