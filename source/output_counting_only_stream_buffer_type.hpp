#if !defined(EGT_COMPARISON_OUTPUT_COUNTING_ONLY_STREAM_BUFFER_TYPE_HPP)
#define EGT_COMPARISON_OUTPUT_COUNTING_ONLY_STREAM_BUFFER_TYPE_HPP


#include <streambuf>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"


class output_counting_only_stream_buffer_type
 : public std::basic_streambuf<char> {
public:
    using char_type = std::basic_streambuf<char>::char_type;
    using int_type = std::basic_streambuf<char>::int_type;
    using traits_type = std::char_traits<char>;

    output_counting_only_stream_buffer_type(
        output_counting_only_stream_buffer_type const & other) = default;
    output_counting_only_stream_buffer_type(
        output_counting_only_stream_buffer_type && other) = default;
    output_counting_only_stream_buffer_type & operator=(
        output_counting_only_stream_buffer_type const & other) = default;
    output_counting_only_stream_buffer_type & operator=(
        output_counting_only_stream_buffer_type && other) = default;
    virtual ~output_counting_only_stream_buffer_type() override = default;
    output_counting_only_stream_buffer_type() {
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);
    }

    virtual int_type overflow(int_type ch) override {
        if (traits_type::eof() != ch) {
            ++the_characters_written_count;
        }
        return ch;
    }

    std::size_t characters_written_count() const {
        return the_characters_written_count;
    }

private:
    std::size_t the_characters_written_count{};
};


#pragma clang diagnostic pop


#endif // EGT_COMPARISON_OUTPUT_COUNTING_ONLY_STREAM_BUFFER_TYPE_HPP
