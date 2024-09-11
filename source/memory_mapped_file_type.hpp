#if !defined(EGT_COMPARISON_MEMORY_MAPPED_FILE_TYPE_HPP)
#define EGT_COMPARISON_MEMORY_MAPPED_FILE_TYPE_HPP


#include <sys/mman.h>

#include <cstddef>
#include <filesystem>
#include <optional>


// Assumes that
// a) the underlying file has a fixed size
// b) it is desired to map the entire file into memory
class memory_mapped_file_type {
public:
    static std::optional<memory_mapped_file_type> map_file_into_memory(
        std::filesystem::path path,
        int open_flags,
        mode_t open_mode,
        int mmap_protection,
        int mmap_flags);

    memory_mapped_file_type & operator=(memory_mapped_file_type const &) =
     delete;
    memory_mapped_file_type & operator=(memory_mapped_file_type &&);
    memory_mapped_file_type(memory_mapped_file_type const &) = delete;
    memory_mapped_file_type(memory_mapped_file_type &&);
    ~memory_mapped_file_type() { reset(); }
    memory_mapped_file_type()
     : the_memory_map_size{}, the_memory_mapped_address{nullptr} {}

    explicit operator bool() const { return data(); }
    bool empty() const { return 0 == size(); }
    std::size_t size() const { return the_memory_map_size; }

    std::byte const * data() const { return the_memory_mapped_address; }
    std::byte * data() { return the_memory_mapped_address; }

    std::byte const * cbegin() const { return the_memory_mapped_address; }
    std::byte const * begin() const { return the_memory_mapped_address; }
    std::byte * begin() { return the_memory_mapped_address; }

    std::byte const * cend() const {
        return the_memory_mapped_address + the_memory_map_size;
    }
    std::byte const * end() const {
        return the_memory_mapped_address + the_memory_map_size;
    }
    std::byte * end() {
        return the_memory_mapped_address + the_memory_map_size;
    }

    std::byte operator[](std::size_t index) const {
        return the_memory_mapped_address[index];
    }
    std::byte operator[](std::size_t index) {
        return the_memory_mapped_address[index];
    }

    void reset();

private:
    memory_mapped_file_type(std::size_t size, void * address);

    std::size_t the_memory_map_size{};
    std::byte * the_memory_mapped_address{};
};


#endif // EGT_COMPARISON_MEMORY_MAPPED_FILE_TYPE_HPP
