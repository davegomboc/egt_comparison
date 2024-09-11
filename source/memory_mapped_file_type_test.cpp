#include "memory_mapped_file_type_test.hpp"


#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <optional>
#include <string>


constexpr std::size_t test_data_size = 512;


std::string create_temporary_file() {
    std::filesystem::path path{std::filesystem::temp_directory_path()};
    path /= "memory_mapped_file_type_test_XXXXXX";
    std::string path_as_string{path.string()};
    int file_descriptor{mkstemp(path_as_string.data())};
    if (-1 == file_descriptor) {
        assert(false);
        std::exit(EXIT_FAILURE);
    }

    std::array<std::byte, test_data_size> test_data;
    for (std::size_t i{}; i != test_data_size; ++i) {
        test_data[i] = static_cast<std::byte>(i);
    }
    ssize_t bytes_written{
     write(file_descriptor, test_data.data(), test_data_size)};
    if (test_data_size != bytes_written) {
        close(file_descriptor);
        assert(false);
        std::exit(EXIT_FAILURE);
    }
    int fsync_result{fsync(file_descriptor)};
    if (0 != fsync_result) {
        close(file_descriptor);
        assert(false);
        std::exit(EXIT_FAILURE);
    }
    close(file_descriptor);
    return path_as_string;
}


TEST(MemoryMappedFileType, CreateMapAndReadTemporaryFile) {
    std::string temp_file_name{create_temporary_file()};

    std::optional<memory_mapped_file_type> mapped_file_opt{
     memory_mapped_file_type::map_file_into_memory(temp_file_name,
     O_LARGEFILE, O_RDONLY, PROT_READ, MAP_SHARED_VALIDATE)};
    ASSERT_NE(std::nullopt, mapped_file_opt);

    for (std::size_t i{}; i != test_data_size; ++i) {
        EXPECT_EQ(static_cast<std::byte>(i), (*mapped_file_opt)[i]);
    }

    std::error_code error_code;
    std::filesystem::remove(temp_file_name, error_code);
}
