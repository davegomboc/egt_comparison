#ifndef EGT_COMPARISON_BINARY_STREAM_IO_HPP
#define EGT_COMPARISON_BINARY_STREAM_IO_HPP


#include <cstdint>
#include <iosfwd>


// Returns false on success, or true on an incomplete read.
bool read_bytes(
    std::istream & in,
    std::uint64_t count,
    void * data_ptr);

// Returns false on success, or true on an incomplete write.
bool write_bytes(
    std::ostream & out,
    std::uint64_t count,
    void const * data_ptr);


#endif // EGT_COMPARISON_BINARY_STREAM_IO_HPP
