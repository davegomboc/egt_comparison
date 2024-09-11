#include "representation/figurine_colour_type_test.hpp"


#include <unordered_map>


using representation::figurine_colour_type;

using representation::white;
using representation::black;


TEST(FigurineColourType, LessThan) {
    EXPECT_LT(white, black);
}


TEST(FigurineColourType, UsableAsKeyOfUnorderedMap) {
    std::unordered_map<figurine_colour_type, char> associative_array;
    associative_array[black] = black.as_char();
    associative_array[white] = white.as_char();

    for (auto const & [key, value]: associative_array) {
        EXPECT_EQ(value, key.as_char());
    }
}
