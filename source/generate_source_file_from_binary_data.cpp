#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cstdlib>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <optional>

#include "memory_mapped_file_type.hpp"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Specify exactly one argument: the file to read."
         << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::filesystem::path const file{argv[1]};
    std::uintmax_t const byte_count{std::filesystem::file_size(file)};

    std::optional<memory_mapped_file_type> mapped_file_opt{
     memory_mapped_file_type::map_file_into_memory(
     file, 0, O_RDONLY, PROT_READ, MAP_PRIVATE)};
    if (std::nullopt == mapped_file_opt) {
        std::exit(EXIT_FAILURE);
    }

    std::cout << "#include <array>\n#include <cstddef>\n\n"
     "constexpr std::array<std::byte, " << byte_count << "> data = {\n"
     << std::hex << std::setfill('0') << std::internal;
    for (std::uintmax_t nth_byte{}; nth_byte != byte_count; ++nth_byte) {
        std::byte const value{(*mapped_file_opt)[nth_byte]};
        unsigned int const uchar_value{std::to_integer<unsigned char>(value)};
        if (0 == (nth_byte & 7)) {
            if (0 != nth_byte) {
                std::cout << "\n";
            }
            std::cout << "   ";
        }
        std::cout << " 0x" << std::setw(2) << uchar_value << ",";
    }
    std::cout << "\n};" << std::endl;
}
