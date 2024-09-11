#include "representation/figurine_shape_type_test.hpp"


#include <unordered_map>


using representation::figurine_shape_type;

using representation::king;
using representation::queen;
using representation::rook;
using representation::bishop;
using representation::knight;
using representation::pawn;


TEST(FigurineShapeType, LessThan) {
    EXPECT_LT(king, queen);
    EXPECT_LT(queen, rook);
    EXPECT_LT(rook, bishop);
    EXPECT_LT(bishop, knight);
    EXPECT_LT(knight, pawn);
}


TEST(FigurineShapeType, UsableAsKeyOfUnorderedMap) {
    std::unordered_map<figurine_shape_type, char> associative_array;
    associative_array[king] = king.as_char();
    associative_array[queen] = queen.as_char();
    associative_array[rook] = rook.as_char();
    associative_array[bishop] = bishop.as_char();
    associative_array[knight] = knight.as_char();
    associative_array[pawn] = pawn.as_char();

    for (auto const & [key, value]: associative_array) {
        EXPECT_EQ(value, key.as_char());
    }
}
