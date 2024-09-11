#include "unxz.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

#include <fcntl.h>

#include "const_memory_stream_buffer_view_type.hpp"
#include "memory_mapped_file_type.hpp"
#include "mutable_memory_stream_buffer_view_type.hpp"
#include "output_counting_only_stream_buffer_type.hpp"


// nullptr is returned iff a failure occurred.
std::unique_ptr<lzma_stream> get_initialized_xz_stream() {
    std::unique_ptr<lzma_stream> xz_stream{new lzma_stream(LZMA_STREAM_INIT)};
    lzma_ret const return_code{lzma_stream_decoder(xz_stream.get(),
     std::numeric_limits<std::uint64_t>::max(), 0)};
    if (LZMA_OK == return_code) {
        return xz_stream;
    }
    std::cerr << "DD: Failure in get_initialized_xz_stream(): return_code = "
     << static_cast<int>(return_code) << std::endl;
    return nullptr;
}


// Returns false on success, true otherwise.
bool decompress(
    lzma_stream & xz_stream,
    std::istream & data_in,
    std::ostream & data_out
) {
    constexpr std::size_t buffer_size{0x10000};
    std::array<std::byte, buffer_size> buffer_in;
    std::array<std::byte, buffer_size> buffer_out;
    char * const istream_buffer_in_data{
     reinterpret_cast<char *>(buffer_in.data())};
    char * const istream_buffer_out_data{
     reinterpret_cast<char *>(buffer_out.data())};
    std::uint8_t * const lzmalib_buffer_in_data{
     reinterpret_cast<std::uint8_t *>(buffer_in.data())};
    std::uint8_t * const lzmalib_buffer_out_data{
     reinterpret_cast<std::uint8_t *>(buffer_out.data())};

    xz_stream.next_in = nullptr;
    xz_stream.avail_in = 0;
    xz_stream.next_out = lzmalib_buffer_out_data;
    xz_stream.avail_out = buffer_size;
    lzma_action action(LZMA_RUN);

    while (true) {
        if ((0 == xz_stream.avail_in) and (not data_in.eof())) {
            xz_stream.next_in = lzmalib_buffer_in_data;
            data_in.read(istream_buffer_in_data, buffer_size);
            assert(0 <= data_in.gcount());
            using target_type = decltype(xz_stream.avail_in);
            assert(static_cast<std::size_t>(data_in.gcount())
             <= std::numeric_limits<target_type>::max());
            xz_stream.avail_in = static_cast<target_type>(data_in.gcount());
            if (data_in.bad()) {
                std::cerr << "EE: Error while reading file into buffer_in."
                 << std::endl;
                return true;
            }
            if (data_in.eof()) {
                action = LZMA_FINISH;
            }
        }
        lzma_ret const lzma_code_result{lzma_code(&xz_stream, action)};
        if ((0 == xz_stream.avail_out) or
         (LZMA_STREAM_END == lzma_code_result)) {
            std::size_t const write_size{buffer_size - xz_stream.avail_out};
            assert(write_size <= std::numeric_limits<std::streamsize>::max());
            data_out.write(istream_buffer_out_data,
             static_cast<std::streamsize>(write_size));
            if (data_out.bad()) {
                std::cerr << "DD: Error while writing decompressed data from "
                 "buffer_out to data_out." << std::endl;
                return true;
            }
            xz_stream.next_out = lzmalib_buffer_out_data;
            xz_stream.avail_out = buffer_size;
        }
        if (LZMA_OK == lzma_code_result) {
            continue;
        }
        if (LZMA_STREAM_END == lzma_code_result) {
            data_out << std::flush;
            return false;
        }
        std::ostringstream error_message;
        switch (lzma_code_result) {
        case LZMA_MEM_ERROR:
            error_message << "LZMA_MEM_ERROR";
            break;
        case LZMA_FORMAT_ERROR:
            error_message << "LZMA_FORMAT_ERROR";
            break;
        case LZMA_OPTIONS_ERROR:
            error_message << "LZMA_OPTIONS_ERROR";
            break;
        case LZMA_DATA_ERROR:
            error_message << "LZMA_DATA_ERROR";
            break;
        case LZMA_BUF_ERROR:
            error_message << "LZMA_BUF_ERROR";
            break;
        case LZMA_MEMLIMIT_ERROR:
            error_message << "LZMA_MEMLIMIT_ERROR";
            break;
        case LZMA_PROG_ERROR:
            error_message << "LZMA_PROG_ERROR";
            break;
        case LZMA_GET_CHECK:
            error_message << "LZMA_GET_CHECK";
            break;
        case LZMA_NO_CHECK:
            error_message << "LZMA_NO_CHECK";
            break;
        case LZMA_UNSUPPORTED_CHECK:
            error_message << "LZMA_UNSUPPORTED_CHECK";
            break;
        case LZMA_STREAM_END:
            error_message << "LZMA_STREAM_END";
            break;
        case LZMA_SEEK_NEEDED:
            error_message << "LZMA_SEEK_NEEDED";
            break;
        default:
            error_message << "An unknown LZMA error";
        }
        error_message << " occurred.\n";
        std::cerr << error_message.str();
        return true;
    }
}


// TODO: Review https://tukaani.org/xz/xz-file-format.txt.  We should
// be able to use the information stored within the XZ header to
// compute the needed information without actually performing a full
// decompression.
std::optional<std::size_t> size_of_uncompressed_data_opt(
    char const * const cbegin,
    char const * const cend
) {
    const_memory_stream_buffer_view_type xz_buffer_view(cbegin, cend);
    std::istream xz_compressed_input(&xz_buffer_view);
    output_counting_only_stream_buffer_type unxz_sizer;
    std::ostream output_sizing_stream(&unxz_sizer);

    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    if (nullptr == xz_stream_ptr) {
        return std::nullopt;
    }
    bool const decompression_result{
     decompress(*xz_stream_ptr, xz_compressed_input, output_sizing_stream)};
    return decompression_result ? std::nullopt
     : std::make_optional<size_t>(unxz_sizer.characters_written_count());
}

std::optional<std::size_t> size_of_uncompressed_data_opt(
    std::byte const * cbegin,
    std::byte const * cend
) {
    return size_of_uncompressed_data_opt(reinterpret_cast<char const *>(cbegin),
     reinterpret_cast<char const *>(cend));
}

std::optional<std::vector<char, default_init_type<char>>>
vector_with_uncompressed_data_opt(
    char const * const cbegin,
    char const * const cend
) {
    std::optional<std::size_t> uncompressed_size_opt{
     size_of_uncompressed_data_opt(cbegin, cend)};
    if (std::nullopt == uncompressed_size_opt) {
        return std::nullopt;
    }
    auto data_opt{
     std::make_optional<std::vector<char, default_init_type<char>>>()};
    data_opt->resize(*uncompressed_size_opt);
    const_memory_stream_buffer_view_type xz_buffer_view(cbegin, cend);
    std::istream xz_compressed_input(&xz_buffer_view);
    mutable_memory_stream_buffer_view_type unxz_writer(
     data_opt->data(), data_opt->data() + data_opt->size());
    std::ostream output_decompressing_stream(&unxz_writer);

    std::unique_ptr<lzma_stream> xz_stream_ptr{get_initialized_xz_stream()};
    if (nullptr == xz_stream_ptr) {
        return std::nullopt;
    }
    bool const decompression_result{decompress(*xz_stream_ptr,
     xz_compressed_input, output_decompressing_stream)};
    return decompression_result? std::nullopt : data_opt;
}

std::optional<std::vector<char, default_init_type<char>>>
vector_with_uncompressed_data_opt(
    std::byte const * cbegin,
    std::byte const * cend
) {
    return vector_with_uncompressed_data_opt(
     reinterpret_cast<char const *>(cbegin),
     reinterpret_cast<char const *>(cend));
}

std::optional<std::vector<char, default_init_type<char>>>
vector_with_uncompressed_data_opt(
    std::filesystem::path const & input_path
) {
    std::optional<memory_mapped_file_type> const xz_compressed_data_opt{
     memory_mapped_file_type::map_file_into_memory(
     input_path, O_LARGEFILE, O_RDONLY, PROT_READ, MAP_PRIVATE)};
    if (not xz_compressed_data_opt) {
        return std::nullopt;
    }
    return vector_with_uncompressed_data_opt(
     xz_compressed_data_opt->cbegin(), xz_compressed_data_opt->cend());
}

std::optional<std::stringstream> decompressed_stream_opt(
    std::istream & compressed_data
) {
    std::unique_ptr<lzma_stream> xz_stream_ptr{new lzma_stream(LZMA_STREAM_INIT)};
    if (LZMA_OK != lzma_stream_decoder(xz_stream_ptr.get(),
     std::numeric_limits<std::uint64_t>::max(), 0)) {
        return std::nullopt;
    }
    std::stringstream decompressed_data;
    bool const decompression_failed{decompress(*xz_stream_ptr, compressed_data,
     decompressed_data)};
    lzma_end(xz_stream_ptr.get());
    if (decompression_failed) {
        return std::nullopt;
    }
    return decompressed_data;
}
