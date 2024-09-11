#include "generic_probe_timing.hpp"


#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string_view>

#include "epd/opcode_type.hpp"
#include "epd/operation_type.hpp"
#include "primitive_bit_operations.hpp"
#include "primitive_text_operations.hpp"

#include "probing_summary_statistics_type.hpp"


std::optional<std::string> ingest_lines_of_epd_strings(
    std::istream & in,
    std::vector<epd::extended_position_description_type> & epds
) {
    std::size_t line_count{};
    epds.clear();
    std::string current_epd_line;
    while (std::getline(in, current_epd_line, '\n')) {
        std::optional<epd::extended_position_description_type> epd_opt{
         epd::extended_position_description_type::create(current_epd_line)};
        if (not epd_opt) {
            std::cerr << "TT: Unparseable EPD line found: \"" << current_epd_line << "\".\n";
            return current_epd_line;
        }
        epds.push_back(std::move(*epd_opt));
        ++line_count;
        if ((line_count >= (1 << 24)) && has_single_bit(line_count)) {
            std::cerr << "TT: Parsed " << line_count << " EPD lines.\n";
        }
    }
    std::cerr << "TT: Completed parsing of " << line_count << " EPD lines.\n";
    return std::nullopt;
}


std::optional<epd::extended_position_description_type>
determine_expected_probe_reports(
    std::vector<epd::extended_position_description_type> const & epds,
    std::vector<probe_report_type> & expected_probe_reports,
    bool const convert_expected_probe_reports_to_white_player_perspective
) {
    static std::optional<epd::opcode_type> const game_theoretic_value_opcode_opt(
     epd::opcode_type::create("GTV"));
    assert(game_theoretic_value_opcode_opt);
    expected_probe_reports.clear();
    for (epd::extended_position_description_type const & epd: epds) {
        std::optional<epd::operation_type> game_theoretic_value_operation_opt{
         epd.operations().find_by_opcode_opt(*game_theoretic_value_opcode_opt)};
        if (not game_theoretic_value_operation_opt) {
            return epd;
        }
        std::optional<probe_report_type> probe_report_opt{
         probe_report_type::create(*game_theoretic_value_operation_opt)};
        if (not probe_report_opt) {
            return epd;
        }
        if (convert_expected_probe_reports_to_white_player_perspective) {
            if (epd.side_to_move().black_is_to_move()) {
                probe_report_opt->flip();
            }
        }
        expected_probe_reports.push_back(std::move(*probe_report_opt));
    }
    return std::nullopt;
}


void emit_probe_timings(
    std::ostream & out,
    std::vector<std::chrono::steady_clock::duration> const & probe_durations,
    std::vector<epd::extended_position_description_type> const & epds
) {
    std::size_t const position_count{epds.size()};
    assert(position_count == probe_durations.size());
    if (0 == position_count) {
        return;
    }

    probing_summary_statistics_type summary_stats;
    for (std::size_t i{}; i != position_count; ++i) {
        std::chrono::duration<long double, std::nano> duration_in_ns{
         probe_durations[i]};
        summary_stats.include(duration_in_ns);
    }
    summary_stats.emit(out, false);

    out << "duration (ns)   position #   extended position description\n";
    for (std::size_t i{}; i != position_count; ++i) {
        auto const duration{std::chrono::duration_cast<
         std::chrono::nanoseconds>(probe_durations[i])};
        out << std::setw(13) << duration.count() << "   " << std::setw(10) << i
         << "   " << epds[i] << '\n';
    }
}


void read_probe_timings(
    std::istream & in,
    std::vector<bool> & has_been_read_in,
    std::vector<std::chrono::steady_clock::duration> & probe_durations,
    std::vector<epd::extended_position_description_type> & epds,
    std::size_t const expected_required_size
) {
    assert(has_been_read_in.size() == probe_durations.size());
    assert(has_been_read_in.size() == epds.size());

    has_been_read_in.reserve(expected_required_size);
    probe_durations.reserve(expected_required_size);
    epds.reserve(expected_required_size);

    std::string current_line;
    while (std::getline(in, current_line, '\n')) {
        if (starts_with(current_line, "duration_count = ")
         or starts_with(current_line, "sum_of_durations = ")
         or starts_with(current_line, "sum_of_squared_durations = ")
         or starts_with(current_line, "duration_mean = ")
         or starts_with(current_line, "duration_sample_standard_deviation = ")
         or starts_with(current_line, "duration (ns)")) {
            continue;
        }
        std::istringstream line_in(current_line);
        std::size_t duration_ns;
        std::size_t nth_position;
        line_in >> duration_ns >> nth_position;
        std::chrono::nanoseconds duration(duration_ns);
        std::string const remainder(std::istreambuf_iterator<char>(line_in), {});
        std::string_view epd(remainder);
        trim_whitespace_prefix(epd);
        std::optional<epd::extended_position_description_type> epd_opt{
         epd::extended_position_description_type::create(epd)};
        if (has_been_read_in.size() <= nth_position) {
            if (has_been_read_in.capacity() <= nth_position) {
                std::size_t const new_capacity{
                 std::max(has_been_read_in.capacity() + 1024,
                 has_been_read_in.capacity() * 3 / 2)};
                 assert(nth_position < new_capacity);
                has_been_read_in.reserve(new_capacity);
                probe_durations.reserve(new_capacity);
                epds.reserve(new_capacity);
            }
            has_been_read_in.resize(nth_position + 1);
            probe_durations.resize(nth_position + 1);
            epds.resize(nth_position + 1);
        }
        if (has_been_read_in[nth_position]) {
            std::cerr << "EE: position # " << nth_position
             << " was already read in.\n";
            std::exit(EXIT_FAILURE);
        }
        has_been_read_in[nth_position] = true;
        probe_durations[nth_position] = std::move(duration);
        epds[nth_position] = std::move(*epd_opt);
    }

    has_been_read_in.shrink_to_fit();
    probe_durations.shrink_to_fit();
    epds.shrink_to_fit();
}
