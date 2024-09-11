#include "parallel_for_each_test.hpp"

#include <cctype>
#include <cstddef>
#include <numeric>
#include <vector>


TEST(ParallelForEach, CompilesAndRunsProperly) {
    std::vector<char> const upper_chars{'H', 'E', 'L', 'L', 'O'};
    std::vector<char> chars{'h', 'e', 'l', 'l', 'o'};
    desire_parallel_for_each(chars.begin(), chars.end(), [&](char & value) {
        value = static_cast<char>(std::toupper(value));
    });
    EXPECT_EQ(upper_chars, chars);
}
