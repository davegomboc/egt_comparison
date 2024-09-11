#include "mutable_memory_stream_buffer_view_type_test.hpp"


#include <array>
#include <istream>
#include <string>


TEST(MutableMemoryStreamBufferViewType, RetriveData) {
    std::array<char, 14> text_block{};
    char * const beginning{text_block.data()};
    char * const past_end{text_block.data() + text_block.size()};
    mutable_memory_stream_buffer_view_type mutable_memory_view(
     beginning, past_end);
    std::istream data_in(&mutable_memory_view);
    std::ostream data_out(&mutable_memory_view);
    data_out << "Hello, world!\n";

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


TEST(MutableMemoryStreamBufferViewType, RepositionGetByBeginningAfterPut) {
    std::array<char, 14> text_block{};
    char * const beginning{text_block.data()};
    char * const past_end{text_block.data() + text_block.size()};
    mutable_memory_stream_buffer_view_type mutable_memory_view(
     beginning, past_end);
    std::istream data_in(&mutable_memory_view);
    std::ostream data_out(&mutable_memory_view);
    data_out << "Hello, world!\n";

    mutable_memory_view.pubseekoff(3, std::ios_base::beg);
    std::string first;
    data_in >> first;
    EXPECT_EQ(first, "lo,");
    EXPECT_FALSE(data_in.eof());
}


TEST(MutableMemoryStreamBufferViewType, RepositionGetByEndAfterPut) {
    std::array<char, 14> text_block{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    char * const beginning{text_block.data()};
    char * const past_end{text_block.data() + text_block.size()};
    mutable_memory_stream_buffer_view_type mutable_memory_view(
     beginning, past_end);
    std::istream data_in(&mutable_memory_view);

    mutable_memory_view.pubseekoff(-3, std::ios_base::end);
    std::string first;
    data_in >> first;
    EXPECT_EQ(first, "d!");
    EXPECT_FALSE(data_in.eof());
}


TEST(MutableMemoryStreamBufferViewType,
 SaturatedRepositionGetByBeginningAfterPut) {
    std::array<char, 14> text_block{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    char * const beginning{text_block.data()};
    char * const past_end{text_block.data() + text_block.size()};
    mutable_memory_stream_buffer_view_type mutable_memory_view(
     beginning, past_end);
    std::istream data_in(&mutable_memory_view);
    std::ostream data_out(&mutable_memory_view);
    data_out << "Hello, world!\n";

    mutable_memory_view.pubseekoff(-3, std::ios_base::beg);
    std::string first;
    data_in >> first;
    EXPECT_EQ(first, "Hello,");
    EXPECT_FALSE(data_in.eof());
}


TEST(MutableMemoryStreamBufferViewType, SaturatedRepositionGetByEndAfterPut) {
    std::array<char, 14> text_block{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\n'};
    char * const beginning{text_block.data()};
    char * const past_end{text_block.data() + text_block.size()};
    mutable_memory_stream_buffer_view_type mutable_memory_view(
     beginning, past_end);
    std::istream data_in(&mutable_memory_view);
    std::ostream data_out(&mutable_memory_view);
    data_out << "Hello, world!\n";

    mutable_memory_view.pubseekoff(3, std::ios_base::end);
    std::string first;
    data_in >> first;
    EXPECT_EQ(first, "");
    EXPECT_TRUE(data_in.eof());
}
