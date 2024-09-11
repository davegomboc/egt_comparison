#include "probe_timing_main.hpp"


#include <chrono>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "epd/extended_position_description_type.hpp"
#include "memory_information_snapshotter_type.hpp"

#include "argument_parsing.hpp"
#include "generic_probe_timing.hpp"
#include "probing.hpp"
#include "syzygy_probe_timing.hpp"

#include "egt_format_type.hpp"
#include "endgame_tables_type.hpp"
#include "probe_timing.hpp"


using representation::egt_position_type;
using representation::material_signature_type;
using std::chrono::steady_clock;


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name
     << " <egt_format> <material_signature> [version_specifier]\n"
     "    or " << executable_name
     << " <egt_format> <figurine_count> [version_specifier]\n"
     "       where egt_format is one of bin, clst, dtr, icl, mtbdd, wpr;\n"
     "       where 2 <= figurine_count <= 5.\n"
     "       N.B.  Only clst, icl, mtbdd, and wpr support version "
     "specification.\n";
    std::exit(EXIT_FAILURE);
}


std::variant<unsigned int, representation::material_signature_type>
interpret_command_line_argument(char const * const argv2) {
    std::optional<material_signature_type> material_signature_opt{
     material_signature_type::create_opt(argv2)};
    if (material_signature_opt) {
        return *material_signature_opt;
    }
    return static_cast<unsigned int>(std::atoi(argv2));
}


/*
Provide stdin with the contents of an .epd file such as:
8/4R3/2K3N1/8/1k6/8/8/8 b - - hmvc 0; fmvn 1; GTV l;
8/3K4/q1r5/8/8/4k3/8/8 w - - hmvc 0; fmvn 1; GTV l;
6K1/8/8/8/1P4k1/8/7b/8 w - - hmvc 0; fmvn 1; GTV d;
8/8/2k5/8/4K3/6p1/8/5r2 b - - hmvc 0; fmvn 1; GTV w;
8/k7/8/8/2K5/4q1n1/8/8 b - - hmvc 0; fmvn 1; GTV w;
*/
int main(int argc, char ** argv) {
    memory_information_snapshotter_type snapshotter;

    if ((3 != argc) and (4 != argc)) {
        print_usage(argv[0]);
    }
    bool const version_specified{4 == argc};

    std::unique_ptr<egt_format_type> egt_format_ptr{
     egt_format_type::create_ptr(argv[1])};
    if (nullptr == egt_format_ptr) {
        print_usage(argv[0]);
    }
    egt_format_type & egt_format{*egt_format_ptr};
    bool const probing_syzygy_wcdbl{"rtbw" == egt_format.extension()};

    std::vector<epd::extended_position_description_type> epds;
    std::optional<std::string> bad_input_line_opt{
     ingest_lines_of_epd_strings(std::cin, epds)};
    if (bad_input_line_opt) {
        std::cerr << "EE: EPD test data ingestion failed at line |\n"
         << *bad_input_line_opt << "|.\n";
        return EXIT_FAILURE;
    }
    snapshotter.take_snapshot();

    std::vector<probe_report_type> expected_probe_reports;
    std::optional<epd::extended_position_description_type> bad_probe_report_epd_opt{
     determine_expected_probe_reports(epds, expected_probe_reports,
     not probing_syzygy_wcdbl)};
    if (bad_probe_report_epd_opt) {
        std::cerr << "EE: EPD record is missing information to create "
         "probe_report_type: |" << bad_probe_report_epd_opt->as_epd() << "|.\n";
        return EXIT_FAILURE;
    }
    snapshotter.take_snapshot();
    std::cerr << "II: EPD records have been loaded.\n";

    std::optional<endgame_tables_type> egts_opt;
    if (probing_syzygy_wcdbl) {
        unsigned int const figurine_count_available{probing_init()};
        if (0 == figurine_count_available) {
            std::cerr << "EE: Unable to initialize Syzygy probing code.\n";
            return EXIT_FAILURE;
        }
    } else {
        std::variant<unsigned int, material_signature_type> argv2(
         interpret_command_line_argument(argv[2]));
        if (std::holds_alternative<unsigned int>(argv2)) {
            unsigned int const figurine_count{std::get<unsigned int>(argv2)};
            if ((figurine_count < 2) or (5 < figurine_count)) {
                print_usage(argv[0]);
            }
            if (version_specified) {
                egts_opt = endgame_tables_type(figurine_count, egt_format,
                 argv[3], true);
            } else {
                egts_opt = endgame_tables_type(figurine_count, egt_format, true);
            }
        } else {
            material_signature_type const material_signature(
             std::get<material_signature_type>(argv2));
            egts_opt = endgame_tables_type(2, egt_format, true);
            if (version_specified) {
                egts_opt->load_or_exit(egt_format, material_signature,
                 argv[3]);
            } else {
                egts_opt->load_or_exit(egt_format, material_signature);
            }
        }
    }
    snapshotter.take_snapshot();
    std::cerr << "II: Endgame tables have been loaded.\n";

    std::vector<steady_clock::duration> probe_durations;
    bool mismatch_found;
    if (probing_syzygy_wcdbl) {
        std::vector<tb_position_type> positions;
        std::optional<epd::extended_position_description_type>
         bad_tb_position_epd_opt{determine_tb_positions(epds, positions)};
        if (bad_tb_position_epd_opt) {
            std::cerr << "EE: EPD record is missing information to create "
             "tb_position_type: |" << bad_tb_position_epd_opt->as_epd() << "|."
             << std::endl;
            return EXIT_FAILURE;
        }
        snapshotter.take_snapshot();
        std::cerr << "II: Converted EPD records to tb_position_type.\n";
        mismatch_found = {vet_and_time_syzygy_probes(positions, epds,
         expected_probe_reports, probe_durations, &snapshotter)};
    } else {
        std::vector<egt_position_type> positions{determine_egt_positions(epds)};
        snapshotter.take_snapshot();
        std::cerr << "II: Converted EPD records to egt_position_type.\n";
        mismatch_found = {vet_and_time_probes(*egts_opt, epds, positions,
         expected_probe_reports, probe_durations, &snapshotter)};
    }
    if (mismatch_found) {
        std::cerr << "EE: Probe report verification failed.\n";
        return EXIT_FAILURE;
    }
    snapshotter.take_snapshot();
    std::cerr << "II: Probe timing has completed.\n";

    emit_probe_timings(std::cout, probe_durations, epds);

    snapshotter.parse_all_snapshots();
    snapshotter.emit_tab_separated_header(std::cerr);
    snapshotter.emit_tab_separated_values(std::cerr);
    std::cerr << std::flush;
}
