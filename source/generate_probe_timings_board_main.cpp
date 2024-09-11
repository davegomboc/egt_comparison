#include "generate_probe_timings_board_main.hpp"


#include <array>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include "egt_format_type.hpp"
#include "probe_timings_board.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cout << "usage: " << executable_name
     << " <probe_timings_root_directory>" << std::endl;
    std::exit(EXIT_FAILURE);
}


int main(int argc, char * argv[]) {
    std::array<std::string, 1> const machines{"hpcc"};
    std::array<std::string, 5> const formats{
     "dtr", "mtbdd", "rtbw", "tllm", "wpr"};
    if (2 != argc) {
        print_usage(argv[0]);
    }
    std::filesystem::path const probe_timings_root_directory{argv[1]};
    if (not std::filesystem::exists(probe_timings_root_directory)) {
        std::cerr << "EE: " << probe_timings_root_directory
         << " does not exist.\n";
        std::exit(EXIT_FAILURE);
    }
    for (std::string const & machine: machines) {
        std::filesystem::path machine_directory{probe_timings_root_directory};
        machine_directory /= machine;
        if (not std::filesystem::exists(machine_directory)) {
            std::cerr << "WW: " << machine_directory << " does not exist.\n";
            continue;
        }
        for (std::string const & format: formats) {
            std::unique_ptr<egt_format_type> egt_format_ptr{
             egt_format_type::create_ptr(format)};
            if (nullptr == egt_format_ptr) {
                std::cerr << "WW: " << format << " not recognized.\n";
                continue;
            }
            std::filesystem::path input_path{machine_directory};
            input_path /= egt_format_ptr->as_string();
            input_path /= "shuffled-345-figurine.stdout.xz";
            std::size_t const epd_line_count{UINT64_C(290'000'000)};
            determine_and_emit_probe_timing_board_entries(input_path, machine,
             egt_format_ptr.get(), epd_line_count);
        }
    }
}
