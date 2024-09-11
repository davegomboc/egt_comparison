#include "output_counting_only_stream_buffer_type_test.hpp"


#include <array>
#include <istream>
#include <string>


TEST(OutputCountingOnlyStreamBufferType, WriteZeroChars) {
    output_counting_only_stream_buffer_type output_counting_buffer;
    std::ostream output_counting_stream(&output_counting_buffer);
    EXPECT_EQ(0, output_counting_buffer.characters_written_count());
}


TEST(OutputCountingOnlyStreamBufferType, PutSingleSpace) {
    output_counting_only_stream_buffer_type output_counting_buffer;
    std::ostream output_counting_stream(&output_counting_buffer);
    output_counting_stream.put(' ');
    EXPECT_EQ(1, output_counting_buffer.characters_written_count());
}


TEST(OutputCountingOnlyStreamBufferType, WriteHelloWorld) {
    output_counting_only_stream_buffer_type output_counting_buffer;
    std::ostream output_counting_stream(&output_counting_buffer);
    output_counting_stream << "Hello, world!" << std::endl;
    EXPECT_EQ(14, output_counting_buffer.characters_written_count());
}
