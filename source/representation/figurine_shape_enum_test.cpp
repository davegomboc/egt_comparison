#include "representation/figurine_shape_enum_test.hpp"


#include <sstream>


using representation::figurine_shape_enum;


TEST(FigurineShapeEnum, Validity) {
    EXPECT_FALSE(valid(static_cast<figurine_shape_enum>(0)));
    EXPECT_TRUE(valid(static_cast<figurine_shape_enum>(1)));
    EXPECT_TRUE(valid(static_cast<figurine_shape_enum>(2)));
    EXPECT_TRUE(valid(static_cast<figurine_shape_enum>(3)));
    EXPECT_TRUE(valid(static_cast<figurine_shape_enum>(4)));
    EXPECT_TRUE(valid(static_cast<figurine_shape_enum>(5)));
    EXPECT_TRUE(valid(static_cast<figurine_shape_enum>(6)));
    EXPECT_FALSE(valid(static_cast<figurine_shape_enum>(7)));
    EXPECT_FALSE(valid(static_cast<figurine_shape_enum>(8)));
    EXPECT_FALSE(valid(static_cast<figurine_shape_enum>(9)));
    EXPECT_FALSE(valid(static_cast<figurine_shape_enum>(12)));
}

TEST(FigurineShapeEnum, Predecrement) {
    figurine_shape_enum before_king{figurine_shape_enum::king};
    figurine_shape_enum before_queen{figurine_shape_enum::queen};
    figurine_shape_enum before_rook{figurine_shape_enum::rook};
    figurine_shape_enum before_bishop{figurine_shape_enum::bishop};
    figurine_shape_enum before_knight{figurine_shape_enum::knight};
    figurine_shape_enum before_pawn{figurine_shape_enum::pawn};

    --before_king;
    --before_queen;
    --before_rook;
    --before_bishop;
    --before_knight;
    --before_pawn;

    EXPECT_EQ(before_king, figurine_shape_enum::pawn);
    EXPECT_EQ(before_queen, figurine_shape_enum::king);
    EXPECT_EQ(before_rook, figurine_shape_enum::queen);
    EXPECT_EQ(before_bishop, figurine_shape_enum::rook);
    EXPECT_EQ(before_knight, figurine_shape_enum::bishop);
    EXPECT_EQ(before_pawn, figurine_shape_enum::knight);
}

TEST(FigurineShapeEnum, Preincrement) {
    figurine_shape_enum after_king{figurine_shape_enum::king};
    figurine_shape_enum after_queen{figurine_shape_enum::queen};
    figurine_shape_enum after_rook{figurine_shape_enum::rook};
    figurine_shape_enum after_bishop{figurine_shape_enum::bishop};
    figurine_shape_enum after_knight{figurine_shape_enum::knight};
    figurine_shape_enum after_pawn{figurine_shape_enum::pawn};

    ++after_king;
    ++after_queen;
    ++after_rook;
    ++after_bishop;
    ++after_knight;
    ++after_pawn;

    EXPECT_EQ(after_king, figurine_shape_enum::queen);
    EXPECT_EQ(after_queen, figurine_shape_enum::rook);
    EXPECT_EQ(after_rook, figurine_shape_enum::bishop);
    EXPECT_EQ(after_bishop, figurine_shape_enum::knight);
    EXPECT_EQ(after_knight, figurine_shape_enum::pawn);
    EXPECT_EQ(after_pawn, figurine_shape_enum::king);
}

TEST(FigurineShapeEnum, Inserter) {
    std::ostringstream out;
    out << figurine_shape_enum::pawn << figurine_shape_enum::knight
     << figurine_shape_enum::king << figurine_shape_enum::queen
     << figurine_shape_enum::bishop << figurine_shape_enum::rook;
    EXPECT_EQ("PNKQBR", out.str());
}
