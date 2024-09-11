#include "synchronized_shuffle_test.hpp"

#include <cctype>
#include <cstddef>
#include <numeric>
#include <random>
#include <vector>


TEST(SynchronizedShuffle, ShufflingMultipleItemsYieldsSamePermutationInEach) {
    std::mt19937_64 generator;
    std::vector<char> const chars{
     'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!'};
    std::vector<char> first{chars};
    std::vector<char> second{chars};
    std::vector<char> third{chars};
    std::vector<char> fourth{chars};

    ASSERT_EQ(chars, first);
    ASSERT_EQ(chars, second);
    ASSERT_EQ(chars, third);
    ASSERT_EQ(chars, fourth);

    synchronized_shuffle(generator, static_cast<std::ptrdiff_t>(chars.size()),
     first.begin(), second.begin(), third.begin(), fourth.begin());

    EXPECT_NE(chars, first);
    EXPECT_EQ(first, second);
    EXPECT_EQ(second, third);
    EXPECT_EQ(third, fourth);
}
