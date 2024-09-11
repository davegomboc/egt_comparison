#include "syzygy_probe_timing.hpp"


#include <cassert>
#include <iostream>

#include "primitive_bit_operations.hpp"


std::optional<epd::extended_position_description_type> determine_tb_positions(
    std::vector<epd::extended_position_description_type> const & epds,
    std::vector<tb_position_type> & tb_positions
) {
    static std::optional<epd::opcode_type> const half_move_count_opcode_opt(
     epd::opcode_type::create("hmvc"));
    assert(half_move_count_opcode_opt);
    static std::optional<epd::opcode_type> const full_move_number_opcode_opt(
     epd::opcode_type::create("fmvn"));
    assert(full_move_number_opcode_opt);
    tb_positions.clear();
    for (epd::extended_position_description_type const & epd: epds) {
        std::optional<tb_position_type> tb_position_opt(tb_position_type::create(epd));
        if (not tb_position_opt) {
            return epd;
        }
        tb_positions.push_back(std::move(*tb_position_opt));
    }
    return std::nullopt;
}


bool vet_syzygy_probe_reports(
    std::vector<tb_position_type> const & positions,
    std::vector<epd::extended_position_description_type> const & epds,
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
        probe_reports[i] = egt_probe(positions[i]);
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
            std::cerr << "EE: Probe returned "
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


void time_syzygy_probes(
    std::vector<tb_position_type> const & positions,
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
        probe_reports[i] = egt_probe(positions[i]);
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


bool vet_and_time_syzygy_probes(
    std::vector<tb_position_type> const & positions,
    std::vector<epd::extended_position_description_type> const & epds,
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
    bool const mismatch_found{vet_syzygy_probe_reports(positions, epds,
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
    time_syzygy_probes(positions, probe_reports, probe_durations,
     snapshotter_ptr);
    auto const timing_stop_time{std::chrono::steady_clock::now()};
    auto const timing_duration{std::chrono::duration_cast<
     std::chrono::nanoseconds>(timing_stop_time - timing_start_time)};
    std::cerr << "II: timing duration = " << timing_duration.count()
     << " ns.\n";

    return false;
}
