#include "memory_mapped_file_type.hpp"


#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>


memory_mapped_file_type::memory_mapped_file_type(
    std::size_t size,
    void * address
) : the_memory_map_size(size),
    the_memory_mapped_address(static_cast<std::byte *>(address))
{
    int madvise_result{madvise(address, size,
     MADV_COLD | MADV_DONTDUMP | MADV_DONTNEED | MADV_RANDOM)};
    if (-1 == madvise_result) {
        std::cerr << "WW: Errno " << std::strerror(errno)
         << " encountered when calling madvise().\n";
        errno = 0;
    }
}


std::optional<memory_mapped_file_type>
memory_mapped_file_type::map_file_into_memory(
    std::filesystem::path file,
    int open_flags,
    mode_t open_mode,
    int mmap_protection,
    int mmap_flags
) {
    std::string file_as_string{file.string()};
    int const file_descriptor{
     open(file_as_string.data(), open_flags, open_mode)};
    if (-1 == file_descriptor) {
        std::cerr << "EE: Call to open(" << file_as_string << ", " << open_flags
         << ", " << open_mode << ") failed: " << std::strerror(errno) << ".\n";
        errno = 0;
        return std::nullopt;
    }

    std::error_code error_code;
    std::uintmax_t const mapped_file_size{
     std::filesystem::file_size(file, error_code)};
    if (static_cast<std::uintmax_t>(-1) == mapped_file_size) {
        std::cerr << "EE: Could not determine file size: "
         << std::strerror(errno) << "." << std::endl;
        errno = 0;
        return std::nullopt;
    }

    void * const mapped_memory_address{mmap(nullptr, mapped_file_size,
     mmap_protection, mmap_flags, file_descriptor, 0)};
    if (MAP_FAILED == mapped_memory_address) {
        std::cerr << "EE: Attempt to memory map " << file
         << " failed: " << std::strerror(errno) << "." << std::endl;
        errno = 0;
    }
    int const close_result{close(file_descriptor)};
    if (-1 == close_result) {
        std::cerr << "EE: Call to close(" << file_descriptor
         << ") failed: " << std::strerror(errno) << "." << std::endl;
        errno = 0;
    }
    if (MAP_FAILED == mapped_memory_address) {
        return std::nullopt;
    }
    return memory_mapped_file_type{mapped_file_size, mapped_memory_address};
}


void memory_mapped_file_type::reset() {
    if (nullptr != the_memory_mapped_address) {
        int const munmap_result{
         munmap(the_memory_mapped_address, the_memory_map_size)};
        if (0 != munmap_result) {
            std::cerr << "EE: Errno " << errno
             << " encountered when calling munmap." << std::endl;
            errno = 0;
        }
        the_memory_mapped_address = nullptr;
        the_memory_map_size = 0;
    }
}


memory_mapped_file_type & memory_mapped_file_type::operator=(
    memory_mapped_file_type && other
) {
    reset();
    the_memory_mapped_address = other.the_memory_mapped_address;
    the_memory_map_size = other.the_memory_map_size;
    other.the_memory_mapped_address = nullptr;
    other.the_memory_map_size = 0;
    return *this;
}


memory_mapped_file_type::memory_mapped_file_type(
    memory_mapped_file_type && other
) : the_memory_map_size(other.the_memory_map_size),
    the_memory_mapped_address(other.the_memory_mapped_address)
{
    other.the_memory_mapped_address = nullptr;
    other.the_memory_map_size = 0;
}
