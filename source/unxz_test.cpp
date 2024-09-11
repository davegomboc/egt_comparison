#include "unxz_test.hpp"

#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <strstream>

#include "representation/figurines_type.hpp"
#include "representation/material_signature_type.hpp"
#include "default_init_type.hpp"
#include "environment_variable_access.hpp"
#include "initialize_with.hpp"
#include "paths.hpp"


std::array<char, 1> constexpr empty_buffer{};

std::array<char, 4> constexpr junk_buffer{'j', 'u', 'n', 'k'};

// Generated via xz -9 -e on an empty file.
std::array<char, 0x20> constexpr well_formed_compressed_zero_byte_buffer {
    initialize_with<char>(
     0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x04,
     0xe6, 0xd6, 0xb4, 0x46, 0x00, 0x00, 0x00, 0x00,
     0x1c, 0xdf, 0x44, 0x21, 0x1f, 0xb6, 0xf3, 0x7d,
     0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x59, 0x5a
    )
};

// Generated via xz -9 -e on a file containing a single space character (and no
// trailing newline).
std::array<char, 0x3c> constexpr
well_formed_compressed_single_space_buffer {
    initialize_with<char>(
     0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x04,
     0xe6, 0xd6, 0xb4, 0x46, 0x02, 0x00, 0x21, 0x01,
     0x1c, 0x00, 0x00, 0x00, 0x10, 0xcf, 0x58, 0xcc,
     0x01, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
     0xcb, 0xf3, 0x01, 0xc6, 0x03, 0xe3, 0x9a, 0xe4,
     0x00, 0x01, 0x19, 0x01, 0xa5, 0x2c, 0x81, 0xcc,
     0x1f, 0xb6, 0xf3, 0x7d, 0x01, 0x00, 0x00, 0x00,
     0x00, 0x04, 0x59, 0x5a
    )
};

// Generated via xz -9 -e on a file containing Hello, world! and a trailing
// newline on GNU/Linux.
std::array<char, 0x48> constexpr
well_formed_compressed_hello_world_buffer {
    initialize_with<char>(
     0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x04,
     0xe6, 0xd6, 0xb4, 0x46, 0x02, 0x00, 0x21, 0x01,
     0x1c, 0x00, 0x00, 0x00, 0x10, 0xcf, 0x58, 0xcc,
     0x01, 0x00, 0x0d, 0x48, 0x65, 0x6c, 0x6c, 0x6f,
     0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
     0x0a, 0x00, 0x00, 0x00, 0xc3, 0xad, 0x94, 0xb3,
     0x17, 0xf6, 0x0c, 0xca, 0x00, 0x01, 0x26, 0x0e,
     0x08, 0x1b, 0xe0, 0x04, 0x1f, 0xb6, 0xf3, 0x7d,
     0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x59, 0x5a
    )
};


TEST(Unxz, SizeOfDecompressedZeroBytes) {
    std::optional<std::size_t> decompressed_size_opt{
     size_of_uncompressed_data_opt(empty_buffer.cbegin(), empty_buffer.cend())};
    ASSERT_EQ(std::nullopt, decompressed_size_opt);
}

TEST(Unxz, DecompressZeroBytes) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    ASSERT_NE(nullptr, xz_stream_ptr);
    std::strstreambuf buffer_with_ostensibly_xz_compressed_bytes(
     empty_buffer.data(), empty_buffer.size());
    std::istream data_in(&buffer_with_ostensibly_xz_compressed_bytes);
    std::ostringstream data_out;
    bool decompression_failed{decompress(*xz_stream_ptr, data_in, data_out)};
    lzma_end(xz_stream_ptr.get());
    ASSERT_TRUE(decompression_failed);
}

TEST(Unxz, VectorWithDecompressedDataOptOfZeroBytes) {
    auto const data_opt{vector_with_uncompressed_data_opt(
     empty_buffer.cbegin(), empty_buffer.cend())};
    EXPECT_EQ(std::nullopt, data_opt);
}

TEST(Unxz, SizeOfDecompressedJunkBytes) {
    std::optional<std::size_t> decompressed_size_opt{
     size_of_uncompressed_data_opt(junk_buffer.cbegin(), junk_buffer.cend())};
    ASSERT_EQ(std::nullopt, decompressed_size_opt);
}

TEST(Unxz, DecompressJunkBytes) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    ASSERT_NE(nullptr, xz_stream_ptr);
    std::strstreambuf buffer_with_ostensibly_xz_compressed_bytes(
     junk_buffer.data(), junk_buffer.size());
    std::istream data_in(&buffer_with_ostensibly_xz_compressed_bytes);
    std::ostringstream data_out;
    bool decompression_failed{decompress(*xz_stream_ptr, data_in, data_out)};
    lzma_end(xz_stream_ptr.get());
    EXPECT_TRUE(decompression_failed);
}

TEST(Unxz, VectorWithDecompressedDataOptOfJunkBytes) {
    auto const data_opt{vector_with_uncompressed_data_opt(
     junk_buffer.cbegin(), junk_buffer.cend())};
    EXPECT_EQ(std::nullopt, data_opt);
}

TEST(Unxz, SizeOfDecompressedEmptyData) {
    std::optional<std::size_t> decompressed_size_opt{
     size_of_uncompressed_data_opt(
     well_formed_compressed_zero_byte_buffer.cbegin(),
     well_formed_compressed_zero_byte_buffer.cend())};
    ASSERT_TRUE(decompressed_size_opt);
    EXPECT_EQ(std::string("").size(), *decompressed_size_opt);
}

TEST(Unxz, DecompressCompressedEmptyData) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    ASSERT_NE(nullptr, xz_stream_ptr);
    std::strstreambuf buffer_with_ostensibly_xz_compressed_bytes(
     well_formed_compressed_zero_byte_buffer.data(),
     well_formed_compressed_zero_byte_buffer.size());
    std::istream data_in(&buffer_with_ostensibly_xz_compressed_bytes);
    std::ostringstream data_out;
    bool decompression_failed{decompress(*xz_stream_ptr, data_in, data_out)};
    lzma_end(xz_stream_ptr.get());
    ASSERT_FALSE(decompression_failed);
    EXPECT_TRUE(data_out.str().empty());
}

TEST(Unxz, VectorWithDecompressedCompressedEmptyData) {
    auto const data_opt{vector_with_uncompressed_data_opt(
     well_formed_compressed_zero_byte_buffer.cbegin(),
     well_formed_compressed_zero_byte_buffer.cend())};
    ASSERT_TRUE(data_opt.has_value());
    EXPECT_TRUE(data_opt->empty());
}

TEST(Unxz, SizeOfDecompressedSingleSpace) {
    std::optional<std::size_t> decompressed_size_opt{
     size_of_uncompressed_data_opt(
     well_formed_compressed_single_space_buffer.cbegin(),
     well_formed_compressed_single_space_buffer.cend())};
    ASSERT_TRUE(decompressed_size_opt);
    EXPECT_EQ(std::string(" ").size(), *decompressed_size_opt);
}

TEST(Unxz, DecompressCompressedSingleSpace) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    ASSERT_NE(nullptr, xz_stream_ptr);
    std::strstreambuf buffer_with_ostensibly_xz_compressed_bytes(
     well_formed_compressed_single_space_buffer.data(),
     well_formed_compressed_single_space_buffer.size());
    std::istream data_in(&buffer_with_ostensibly_xz_compressed_bytes);
    std::ostringstream data_out;
    bool decompression_failed{decompress(*xz_stream_ptr, data_in, data_out)};
    lzma_end(xz_stream_ptr.get());
    ASSERT_FALSE(decompression_failed);
    EXPECT_EQ(" ", data_out.str());
}

TEST(Unxz, VectorWithDecompressedCompressedSingleSpace) {
    auto const data_opt{vector_with_uncompressed_data_opt(
     well_formed_compressed_single_space_buffer.cbegin(),
     well_formed_compressed_single_space_buffer.cend())};
    ASSERT_TRUE(data_opt.has_value());
    std::vector<char> const expected{' '};
    EXPECT_EQ(expected.size(), data_opt->size());
    EXPECT_EQ(' ', (*data_opt)[0]);
}

TEST(Unxz, SizeOfDecompressedHelloWorld) {
    std::optional<std::size_t> decompressed_size_opt{
     size_of_uncompressed_data_opt(
     well_formed_compressed_hello_world_buffer.cbegin(),
     well_formed_compressed_hello_world_buffer.cend())};
    ASSERT_TRUE(decompressed_size_opt);
    EXPECT_EQ(std::string("Hello, world!\n").size(), *decompressed_size_opt);
}

TEST(Unxz, DecompressCompressedHelloWorld) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    ASSERT_NE(nullptr, xz_stream_ptr);
    std::strstreambuf buffer_with_ostensibly_xz_compressed_bytes(
     well_formed_compressed_hello_world_buffer.data(),
     well_formed_compressed_hello_world_buffer.size());
    std::istream data_in(&buffer_with_ostensibly_xz_compressed_bytes);
    std::ostringstream data_out;
    bool decompression_failed{decompress(*xz_stream_ptr, data_in, data_out)};
    lzma_end(xz_stream_ptr.get());
    ASSERT_FALSE(decompression_failed);
    EXPECT_EQ("Hello, world!\n", data_out.str());
}

TEST(Unxz, VectorWithDecompressedCompressedHelloWorld) {
    auto const data_opt{vector_with_uncompressed_data_opt(
     well_formed_compressed_hello_world_buffer.cbegin(),
     well_formed_compressed_hello_world_buffer.cend())};
    ASSERT_TRUE(data_opt.has_value());
    std::vector<char, default_init_type<char>> const expected{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    EXPECT_EQ(expected.size(), data_opt->size());
    EXPECT_EQ(expected, *data_opt);
}

TEST(Unxz, DecompressXzCompressedChesspressoKQVskBin) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    ASSERT_NE(nullptr, xz_stream_ptr);
    auto KQk_opt{representation::material_signature_type::create_opt(
     representation::figurines_type({representation::white_queen,
     representation::white_king, representation::black_king}))};
    ASSERT_TRUE(KQk_opt);
    std::error_code error_code;
    std::filesystem::path plb_path{chesspresso_path(std::nullopt, *KQk_opt,
     std::nullopt, true, false)};
    ASSERT_TRUE(std::filesystem::exists(plb_path, error_code));
    std::filesystem::path plb_xz_path{chesspresso_path(std::nullopt, *KQk_opt,
     std::nullopt, true, true)};
    ASSERT_TRUE(std::filesystem::exists(plb_xz_path, error_code));

    std::ifstream uncompressed_data_in(plb_path);
    std::stringstream uncompressed_data_stream;
    uncompressed_data_stream << uncompressed_data_in.rdbuf();
    std::string const uncompressed_data{uncompressed_data_stream.str()};

    std::ifstream compressed_data_in(plb_xz_path);
    std::stringstream decompressed_data_stream;
    bool decompression_failed{decompress(*xz_stream_ptr, compressed_data_in,
     decompressed_data_stream)};
    lzma_end(xz_stream_ptr.get());
    ASSERT_FALSE(decompression_failed);
    std::string const decompressed_data{decompressed_data_stream.str()};

    EXPECT_EQ(uncompressed_data, decompressed_data);
}

TEST(Unxz, DecompressedStreamOptJunk) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    ASSERT_NE(nullptr, xz_stream_ptr);
    std::strstreambuf buffer_with_ostensibly_xz_compressed_bytes(
     junk_buffer.data(), junk_buffer.size());
    std::istream data_in(&buffer_with_ostensibly_xz_compressed_bytes);
    std::stringstream decompressed_data;
    bool decompression_failed{decompress(*xz_stream_ptr, data_in,
     decompressed_data)};
    lzma_end(xz_stream_ptr.get());
    EXPECT_TRUE(decompression_failed);
}

TEST(Unxz, DecompressedStreamOptHelloWorld) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    ASSERT_NE(nullptr, xz_stream_ptr);
    std::strstreambuf buffer_with_ostensibly_xz_compressed_bytes(
     well_formed_compressed_hello_world_buffer.data(),
     well_formed_compressed_hello_world_buffer.size());
    std::istream data_in(&buffer_with_ostensibly_xz_compressed_bytes);
    std::stringstream decompressed_data;
    bool decompression_failed{decompress(*xz_stream_ptr, data_in,
     decompressed_data)};
    lzma_end(xz_stream_ptr.get());
    ASSERT_FALSE(decompression_failed);
    ASSERT_EQ("Hello, world!\n", decompressed_data.str());
    std::string first_line;
    std::getline(decompressed_data, first_line);
    EXPECT_EQ("Hello, world!", first_line);
}
