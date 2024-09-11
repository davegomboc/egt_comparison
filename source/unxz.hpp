#if !defined(EGT_COMPARISON_UNXZ_HPP)
#define EGT_COMPARISON_UNXZ_HPP


#include <filesystem>
#include <iosfwd>
#include <istream>
#include <memory>
#include <optional>
#include <sstream>
#include <vector>

#include <lzma.h>

#include "default_init_type.hpp"


// TODO: Use a streambuf that supports the xz compression format.

// nullptr is returned iff a failure occurred.
std::unique_ptr<lzma_stream> get_initialized_xz_stream();

// Returns false on success, true otherwise.
bool decompress(
    lzma_stream & xz_stream,
    std::istream & data_in,
    std::ostream & data_out);

std::optional<std::size_t> size_of_uncompressed_data_opt(
    char const * cbegin, char const * cend);

std::optional<std::size_t> size_of_uncompressed_data_opt(
    std::byte const * cbegin, std::byte const * cend);

std::optional<std::vector<char, default_init_type<char>>>
vector_with_uncompressed_data_opt(
    char const * cbegin, char const * cend);

std::optional<std::vector<char, default_init_type<char>>>
vector_with_uncompressed_data_opt(
    std::byte const * cbegin, std::byte const * cend);

std::optional<std::vector<char, default_init_type<char>>>
vector_with_uncompressed_data_opt(
    std::filesystem::path const & path);

std::optional<std::stringstream> decompressed_stream_opt(std::istream & in);


#endif // EGT_COMPARISON_UNXZ_HPP
