#include "binary_stream_io.hpp"


#include <cassert>
#include <iostream>
#include <limits>


bool read_bytes(
    std::istream & in,
    std::uint64_t const count,
    void * const data_ptr
) {
    assert(count <= std::numeric_limits<std::streamsize>::max());
    in.read(reinterpret_cast<char *>(data_ptr),
     static_cast<std::streamsize>(count));
    return not in.good();
}

bool write_bytes(
    std::ostream & out,
    std::uint64_t const count,
    void const * const data_ptr
) {
    assert(count <= std::numeric_limits<std::streamsize>::max());
    out.write(reinterpret_cast<char const *>(data_ptr),
     static_cast<std::streamsize>(count));
    return not out.good();
}
