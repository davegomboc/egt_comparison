#include "const_memory_stream_buffer_view_type_test.hpp"


#include <array>
#include <istream>
#include <string>


TEST(ConstMemoryStreamBufferViewType, RetrieveData) {
    std::array<char const, 14> const text_block{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    char const * const beginning{text_block.data()};
    char const * const past_end{text_block.data() + text_block.size()};
    const_memory_stream_buffer_view_type const_memory_view(beginning, past_end);
    std::istream data_in(&const_memory_view);

    std::string first;
    data_in >> first;
    std::string second;
    data_in >> second;
    std::string third;
    data_in >> third;

    EXPECT_EQ(first, "Hello,");
    EXPECT_EQ(second, "world!");
    EXPECT_EQ(third, "");
    EXPECT_TRUE(data_in.eof());
}


TEST(ConstMemoryStreamBufferViewType, RepositionGetByBeginning) {
    std::array<char const, 14> const text_block{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    char const * const beginning{text_block.data()};
    char const * const past_end{text_block.data() + text_block.size()};
    const_memory_stream_buffer_view_type const_memory_view(beginning, past_end);
    std::istream data_in(&const_memory_view);

    const_memory_view.pubseekoff(3, std::ios_base::beg);
    std::string first;
    data_in >> first;
    EXPECT_EQ(first, "lo,");
    EXPECT_FALSE(data_in.eof());
}


TEST(ConstMemoryStreamBufferViewType, RepositionGetByEnd) {
    std::array<char const, 14> const text_block{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    char const * const beginning{text_block.data()};
    char const * const past_end{text_block.data() + text_block.size()};
    const_memory_stream_buffer_view_type const_memory_view(beginning, past_end);
    std::istream data_in(&const_memory_view);

    const_memory_view.pubseekoff(-3, std::ios_base::end);
    std::string first;
    data_in >> first;
    EXPECT_EQ(first, "d!");
    EXPECT_FALSE(data_in.eof());
}


TEST(ConstMemoryStreamBufferViewType, SaturatedRepositionGetByBeginning) {
    std::array<char const, 14> const text_block{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    char const * const beginning{text_block.data()};
    char const * const past_end{text_block.data() + text_block.size()};
    const_memory_stream_buffer_view_type const_memory_view(beginning, past_end);
    std::istream data_in(&const_memory_view);

    const_memory_view.pubseekoff(-3, std::ios_base::beg);
    std::string first;
    data_in >> first;
    EXPECT_EQ(first, "Hello,");
    EXPECT_FALSE(data_in.eof());
}


TEST(ConstMemoryStreamBufferViewType, SaturatedRepositionGetByEnd) {
    std::array<char const, 14> const text_block{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    char const * const beginning{text_block.data()};
    char const * const past_end{text_block.data() + text_block.size()};
    const_memory_stream_buffer_view_type const_memory_view(beginning, past_end);
    std::istream data_in(&const_memory_view);

    const_memory_view.pubseekoff(3, std::ios_base::end);
    std::string first;
    data_in >> first;
    EXPECT_EQ(first, "");
    EXPECT_TRUE(data_in.eof());
}
