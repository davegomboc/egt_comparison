#if !defined(EGT_COMPARISON_CONST_MEMORY_STREAM_BUFFER_VIEW_TYPE_HPP)
#define EGT_COMPARISON_CONST_MEMORY_STREAM_BUFFER_VIEW_TYPE_HPP


#include <cassert>
#include <ios>
#include <limits>
#include <streambuf>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"


class const_memory_stream_buffer_view_type final
 : public std::basic_streambuf<char> {
public:
    const_memory_stream_buffer_view_type(
        const_memory_stream_buffer_view_type const & other) = default;
    const_memory_stream_buffer_view_type(
        const_memory_stream_buffer_view_type && other) = default;
    const_memory_stream_buffer_view_type & operator=(
        const_memory_stream_buffer_view_type const & other) = default;
    const_memory_stream_buffer_view_type & operator=(
        const_memory_stream_buffer_view_type & other) = default;
    virtual ~const_memory_stream_buffer_view_type() override = default;
    const_memory_stream_buffer_view_type()
     : the_beginning_address(nullptr), the_past_end_address(nullptr) {
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);
    }
    const_memory_stream_buffer_view_type(
        char const * const begin,
        char const * const end
    ) : the_beginning_address(begin), the_past_end_address(end) {
        assert(0 <= end - begin);
        assert(end - begin <= std::numeric_limits<std::streamoff>::max());
        setp(nullptr, nullptr);
        setg(const_cast<char *>(begin), const_cast<char *>(begin),
         const_cast<char *>(end));
    }

    bool singular() const {
        return (nullptr == the_beginning_address) or
         (the_beginning_address == the_past_end_address);
    }

    virtual std::streamsize showmanyc() override;

    virtual pos_type seekoff(
        off_type offset,
        std::ios_base::seekdir dir,
        std::ios_base::openmode mode = std::ios_base::in
    ) override;

    virtual pos_type seekpos(
        pos_type pos,
        std::ios_base::openmode mode = std::ios_base::in
    ) override;

private:
    pos_type repositioning_failed() {
        return pos_type(off_type(-1));
    }

    char const * the_beginning_address;
    char const * the_past_end_address;
};


inline const_memory_stream_buffer_view_type::pos_type
const_memory_stream_buffer_view_type::seekoff(
    const_memory_stream_buffer_view_type::off_type offset,
    std::ios_base::seekdir dir,
    std::ios_base::openmode mode
) {
    if (singular() or not (mode & std::ios_base::in)) {
        return repositioning_failed();
    }
    off_type const past_end_offset{the_past_end_address - the_beginning_address};
    off_type new_offset;
    switch (dir) {
    case std::ios_base::beg:
        new_offset = 0;
        break;
    case std::ios_base::end:
        new_offset = past_end_offset;
        break;
    case std::ios_base::cur:
        new_offset = gptr() - the_beginning_address;
        break;
    default:
        return repositioning_failed();
    }
    new_offset += offset;
    new_offset = std::min(past_end_offset, std::max(off_type{}, new_offset));
    setg(eback(), eback() + new_offset, egptr());
    return pos_type(new_offset);
}

inline const_memory_stream_buffer_view_type::pos_type
const_memory_stream_buffer_view_type::seekpos(
    pos_type position,
    std::ios_base::openmode mode
) {
    return seekoff(position - pos_type(off_type(0)), std::ios_base::beg, mode);
}

inline std::streamsize const_memory_stream_buffer_view_type::showmanyc() {
    std::ptrdiff_t chars_available{egptr() - gptr()};
    return chars_available ? chars_available : -1;
}


#pragma clang diagnostic pop


#endif // EGT_COMPARISON_CONST_MEMORY_STREAM_BUFFER_VIEW_TYPE_HPP
