#include "representation/figurine_type_test.hpp"


#include <unordered_map>


using representation::figurine_type;

using representation::black_king;
using representation::black_queen;
using representation::black_rook;
using representation::black_bishop;
using representation::black_knight;
using representation::black_pawn;
using representation::white_king;
using representation::white_queen;
using representation::white_rook;
using representation::white_bishop;
using representation::white_knight;
using representation::white_pawn;


TEST(FigurineType, LessThan) {
    EXPECT_LT(white_king, white_queen);
    EXPECT_LT(white_queen, white_rook);
    EXPECT_LT(white_rook, white_bishop);
    EXPECT_LT(white_bishop, white_knight);
    EXPECT_LT(white_knight, white_pawn);
    EXPECT_LT(white_pawn, black_king);
    EXPECT_LT(black_king, black_queen);
    EXPECT_LT(black_queen, black_rook);
    EXPECT_LT(black_rook, black_bishop);
    EXPECT_LT(black_bishop, black_knight);
    EXPECT_LT(black_knight, black_pawn);
    EXPECT_LT(white_knight, black_queen);
}


TEST(FigurineType, UsableAsKeyOfUnorderedMap) {
    std::unordered_map<figurine_type, char> associative_array;
    associative_array[black_king] = black_king.as_char();
    associative_array[black_queen] = black_queen.as_char();
    associative_array[black_rook] = black_rook.as_char();
    associative_array[black_bishop] = black_bishop.as_char();
    associative_array[black_knight] = black_knight.as_char();
    associative_array[black_pawn] = black_pawn.as_char();
    associative_array[white_king] = white_king.as_char();
    associative_array[white_queen] = white_queen.as_char();
    associative_array[white_rook] = white_rook.as_char();
    associative_array[white_bishop] = white_bishop.as_char();
    associative_array[white_knight] = white_knight.as_char();
    associative_array[white_pawn] = white_pawn.as_char();

    for (auto const & [key, value]: associative_array) {
        EXPECT_EQ(value, key.as_char());
    }
}
