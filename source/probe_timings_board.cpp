#include "probe_timings_board.hpp"


#include <array>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <system_error>
#include <vector>

#include "epd/extended_position_description_type.hpp"
#include "unxz.hpp"

#include "generic_probe_timing.hpp"

#include "representation/material_signature_type.hpp"

#include "probe_timing.hpp"

#include "egt_format_type.hpp"


using representation::material_signature_type;


void determine_and_emit_probe_timing_board_entries(
    std::filesystem::path const input_path,
    std::string const machine,
    egt_format_type const * const egt_format_ptr,
    std::size_t const epd_line_count
) {
    std::error_code error_code;
    if (not std::filesystem::exists(input_path, error_code)) {
        std::cerr << "EE: " << input_path << " does not exist.\n";
        return;
    }
    std::ifstream compressed_data_in(input_path);
    std::optional<std::stringstream> data_stream_opt{
     decompressed_stream_opt(compressed_data_in)};
    if (not data_stream_opt) {
        std::cerr << "EE: Unable to decompress data from " << input_path
         << " into stringstream.\n";
        return;
    }

    std::vector<bool> has_been_read_in;
    std::vector<std::chrono::steady_clock::duration> probe_durations;
    std::vector<epd::extended_position_description_type> epds;
    read_probe_timings(*data_stream_opt, has_been_read_in, probe_durations,
     epds, epd_line_count);

    std::vector<material_signature_type> const
     canonicalized_material_signatures{canonical_material_signatures(epds)};
    std::unordered_map<material_signature_type, probing_summary_statistics_type>
     probe_timing_summary{summarize_probe_timings_by_material_signature(
     has_been_read_in, probe_durations, canonicalized_material_signatures)};

    std::vector<material_signature_type> ordered_signatures;
    for (auto const & [key, value]: probe_timing_summary) {
        ordered_signatures.push_back(key);
    }
    std::sort(ordered_signatures.begin(), ordered_signatures.end());

    int chars_per_float{std::numeric_limits<long double>::max_digits10};
    std::cout << std::scientific << std::setprecision(chars_per_float);
    for (material_signature_type const & signature: ordered_signatures) {
        probing_summary_statistics_type const & stats{
         probe_timing_summary[signature]};
        std::cout << std::setw(8) << machine
         << " " << std::setw(5) << egt_format_ptr->as_string()
         << " " << std::setw(12) << signature
         << " " << std::setw(chars_per_float) << stats.count()
         << " " << std::setw(chars_per_float) << stats.sum_of_durations()
         << " " << std::setw(chars_per_float) << stats.sum_of_squared_durations()
         << " " << std::setw(chars_per_float) << *stats.duration_mean_opt()
         << " " << std::setw(chars_per_float)
         << *stats.duration_sample_standard_deviation_opt() << std::endl;
    }
}
