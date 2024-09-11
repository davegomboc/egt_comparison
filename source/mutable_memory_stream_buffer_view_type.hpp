#if !defined(EGT_COMPARISON_MUTABLE_MEMORY_STREAM_BUFFER_VIEW_TYPE_HPP)
#define EGT_COMPARISON_MUTABLE_MEMORY_STREAM_BUFFER_VIEW_TYPE_HPP


#include <cassert>
#include <ios>
#include <limits>
#include <optional>
#include <streambuf>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"


class mutable_memory_stream_buffer_view_type final
 : public std::basic_streambuf<char> {
public:
    mutable_memory_stream_buffer_view_type(
        mutable_memory_stream_buffer_view_type const & other) = default;
    mutable_memory_stream_buffer_view_type(
        mutable_memory_stream_buffer_view_type && other) = default;
    mutable_memory_stream_buffer_view_type & operator=(
        mutable_memory_stream_buffer_view_type const & other) = default;
    mutable_memory_stream_buffer_view_type & operator=(
        mutable_memory_stream_buffer_view_type & other) = default;
    virtual ~mutable_memory_stream_buffer_view_type() override = default;
    mutable_memory_stream_buffer_view_type()
     : the_beginning_address(nullptr), the_past_end_address(nullptr) {
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);
    }
    mutable_memory_stream_buffer_view_type(char * const begin, char * const end)
     : the_beginning_address(begin), the_past_end_address(end)
    {
        assert(0 <= end - begin);
        assert(end - begin <= std::numeric_limits<std::streamoff>::max());
        setp(begin, end);
        setg(begin, begin, end);
    }

    bool singular() const {
        return (nullptr == the_beginning_address) or
         (the_beginning_address == the_past_end_address);
    }

    // This exists because pbump() only takes an int.
    void advance_current_put_location(std::streamoff count);

    virtual std::streamsize showmanyc() override;

    virtual pos_type seekoff(
        off_type offset,
        std::ios_base::seekdir dir,
        std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out
    ) override;

    virtual pos_type seekpos(
        pos_type pos,
        std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out
    ) override;

private:
    pos_type repositioning_failed() {
        return pos_type(off_type(-1));
    }

    char * the_beginning_address;
    char * the_past_end_address;
};


inline void mutable_memory_stream_buffer_view_type::advance_current_put_location(
    std::streamoff remaining_count
) {
    while (std::numeric_limits<int>::max() < remaining_count) {
        pbump(std::numeric_limits<int>::max());
        remaining_count -= std::numeric_limits<int>::max();
    }
    pbump(static_cast<int>(remaining_count));
}


inline mutable_memory_stream_buffer_view_type::pos_type
mutable_memory_stream_buffer_view_type::seekoff(
    mutable_memory_stream_buffer_view_type::off_type offset,
    std::ios_base::seekdir dir,
    std::ios_base::openmode mode
) {
    if (singular() or not (mode & (std::ios_base::in | std::ios_base::out))) {
        return repositioning_failed();
    }
    off_type const past_end_offset{the_past_end_address - the_beginning_address};
    std::optional<off_type> new_get_offset_opt;
    std::optional<off_type> new_put_offset_opt;
    switch (dir) {
    case std::ios_base::beg:
        if (std::ios_base::in & mode) {
            new_get_offset_opt = 0;
        }
        if (std::ios_base::out & mode) {
            new_put_offset_opt = 0;
        }
        break;
    case std::ios_base::end:
        if (std::ios_base::in & mode) {
            new_get_offset_opt = past_end_offset;
        }
        if (std::ios_base::out & mode) {
            new_put_offset_opt = past_end_offset;
        }
        break;
    case std::ios_base::cur:
        if (std::ios_base::in & mode) {
            new_get_offset_opt = gptr() - the_beginning_address;
        }
        if (std::ios_base::out & mode) {
            new_put_offset_opt = pptr() - the_beginning_address;
        }
        break;
    default:
        return repositioning_failed();
    }
    if (new_get_offset_opt) {
        *new_get_offset_opt += offset;
        *new_get_offset_opt = std::min(past_end_offset,
         std::max(off_type{}, *new_get_offset_opt));
        setg(the_beginning_address, the_beginning_address + *new_get_offset_opt,
         the_past_end_address);
    }
    if (new_put_offset_opt) {
        *new_put_offset_opt += offset;
        *new_put_offset_opt = std::min(past_end_offset,
         std::max(off_type{}, *new_put_offset_opt));
        setp(the_beginning_address, the_past_end_address);
        advance_current_put_location(*new_put_offset_opt);
        return pos_type(*new_put_offset_opt);
    }
    return pos_type(*new_get_offset_opt);
}

inline mutable_memory_stream_buffer_view_type::pos_type
mutable_memory_stream_buffer_view_type::seekpos(
    pos_type position,
    std::ios_base::openmode mode
) {
    return seekoff(position - pos_type(off_type(0)), std::ios_base::beg, mode);
}

inline std::streamsize mutable_memory_stream_buffer_view_type::showmanyc() {
    std::ptrdiff_t chars_available{egptr() - gptr()};
    return chars_available ? chars_available : -1;
}


#pragma clang diagnostic pop


#endif // EGT_COMPARISON_MUTABLE_MEMORY_STREAM_BUFFER_VIEW_TYPE_HPP
