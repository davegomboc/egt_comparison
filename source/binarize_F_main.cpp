#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "unxz.hpp"
#include "const_memory_stream_buffer_view_type.hpp"


static std::string convert_espresso_input_row_to_PCN_string(
    std::string const & row
) {
    std::string result;
    for (std::size_t i{}; i != row.size(); ++i) {
        switch(row[i]) {
            case '-':
            case 'X':
            case '2':
                result += "11";
                break;
            case '1':
                result += "10";
                break;
            case '0':
                result += "01";
                break;
            case ' ':
                break;
            default:
                std::cerr << "FAIL." << std::endl << "row = |" << row << "|."
                 << std::endl << "offset = " << i << std::endl;
                std::exit(EXIT_FAILURE);
        }
    }
    return result;
}


static void process_stream(std::istream & in, std::ostream & out) {
    std::array<std::uint64_t, 4> binary_header{0, 0, 0, 0};
    std::uint64_t & per_cube_bit_count = binary_header[0];
    std::uint64_t & F_cube_count = binary_header[1];

    bool const debug{};

    for (std::string line; std::getline(in, line);) {
        if (line.empty()) continue;
        if ('.' == line[0]) {
            if (line.size() < 2) {
                std::cerr << "EE: Malformed . line detected.\n";
                std::exit(EXIT_FAILURE);
            }
            switch (line[1]) {
            case 'i':
                line = line.substr(2);
                per_cube_bit_count += 2 * static_cast<std::size_t>(std::atoi(
                 line.c_str()));
                break;
            case 'o':
                line = line.substr(2);
                per_cube_bit_count += static_cast<std::size_t>(std::atoi(
                 line.c_str()));
                break;
            case 'p':
                line = line.substr(2);
                F_cube_count = static_cast<std::size_t>(std::atoi(
                 line.c_str()));
                if (debug) {
                    std::cout << "per_cube_bit_count = " << per_cube_bit_count
                              << "\nF_cube_count = " << F_cube_count
                              << std::endl;
                }
                // Emit the binary file's header.
                assert(32 == sizeof(binary_header));
                out.write(reinterpret_cast<char *>(binary_header.data()), 32);
                break;
            default:
                break;
            }
            continue;
        }
        if (debug) {
            std::cout << " As read: |" << line << "|." << std::endl;
        }
        std::reverse(line.begin(), line.end());
        if (debug) {
            std::cout << "Reversed: |" << line << "|." << std::endl;
        }
        assert(' ' == line[3]);
        std::string outputs(line.substr(0, 3));
        if (debug) {
            std::cout << " Outputs: |" << outputs << "|." << std::endl;
        }
        line = line.substr(4);
        if (debug) {
            std::cout << "  Inputs: |" << line << "|." << std::endl;
        }
        std::string pcn{convert_espresso_input_row_to_PCN_string(line)};
        if (debug) {
            std::cout << "inputpcn: " << std::string(131 - per_cube_bit_count,
             ' ') << "|" << pcn << "|." << std::endl;
        }
        pcn = outputs + pcn;
        if (debug) {
            std::cout << " all pcn: " << std::string(128 - per_cube_bit_count,
             ' ') << "|" << pcn << "|." << std::endl;
        }
        assert(per_cube_bit_count == pcn.size());
        pcn = std::string(128 - per_cube_bit_count, '0') + pcn;
        if (debug) {
            std::cout << "full pcn: |" << pcn << "|.\n"
                      << "high pcn: |" << pcn.substr(0, 64) << "|.\n"
                      << " low pcn: " << std::string(64, ' ') << "|"
                      << pcn.substr(64, 64) << "|." << std::endl;
        }
        std::bitset<64> high_bits{pcn.substr(0, 64)};
        std::uint64_t high_value{high_bits.to_ullong()};
        if (debug) {
            std::cout << "high u64: |" << high_value << "|." << std::endl;
        }
        std::bitset<64> low_bits{pcn.substr(64, 64)};
        std::uint64_t low_value{low_bits.to_ullong()};
        if (debug) {
            std::cout << " low u64: |" << low_value << "|." << std::endl;
        }
        out.write(reinterpret_cast<char const *>(&low_value),
         sizeof(std::uint64_t));
        if (64 < per_cube_bit_count) {
            out.write(reinterpret_cast<char const *>(&high_value),
             sizeof(std::uint64_t));
        }
    }
    out << std::flush;
}


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cout << "usage: " << executable_name
              << " <F_only_input.pla.xz> {<F_only_input.pla.xz> ...}"
              << std::endl;
    std::exit(EXIT_FAILURE);
}

// N.B. Each input file specified needs to contain just F, not also R or D.
int main(int argc, char *argv[]) {
    if (1 == argc) {
        print_usage(argv[0]);
    }
    for (int nth_file{1}; nth_file != argc; ++nth_file) {
        std::filesystem::path const input_path{argv[nth_file]};
        std::filesystem::path output_path{input_path};
        if (".xz" != output_path.extension()) {
            print_usage(argv[0]);
        }
        output_path.replace_extension();
        if (".pla" != output_path.extension()) {
            print_usage(argv[0]);
        }
        output_path.replace_extension(".plb");
        auto const data_opt(vector_with_uncompressed_data_opt(input_path));
        if (std::nullopt == data_opt) {
            std::cerr << "EE: Decompression of " << input_path << " failed.\n";
            return EXIT_FAILURE;
        }
        const_memory_stream_buffer_view_type input_buffer(data_opt->data(),
         data_opt->data() + data_opt->size());
        std::istream in(&input_buffer);
        std::ofstream out{output_path, std::ios::binary | std::ios_base::out};
        process_stream(in, out);
    }
}
