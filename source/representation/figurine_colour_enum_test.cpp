#include "representation/figurine_colour_enum_test.hpp"


#include <sstream>


using representation::figurine_colour_enum;
using representation::off;
using representation::on;


TEST(FigurineColourEnum, Validity) {
    EXPECT_TRUE(valid(static_cast<figurine_colour_enum>(0)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(1)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(2)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(3)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(4)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(5)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(6)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(7)));
    EXPECT_TRUE(valid(static_cast<figurine_colour_enum>(8)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(9)));
    EXPECT_FALSE(valid(static_cast<figurine_colour_enum>(12)));
}

TEST(FigurineColourEnum, AsBool) {
    EXPECT_EQ(true, as_bool(figurine_colour_enum::black));
    EXPECT_EQ(false, as_bool(figurine_colour_enum::white));
}

TEST(FigurineColourEnum, AsChar) {
    EXPECT_EQ('b', as_char(figurine_colour_enum::black));
    EXPECT_EQ('w', as_char(figurine_colour_enum::white));
}

TEST(FigurineColourEnum, AsDescription) {
    EXPECT_EQ("black", as_description(figurine_colour_enum::black));
    EXPECT_EQ("white", as_description(figurine_colour_enum::white));
}

TEST(FigurineColourEnum, AsIndex) {
    EXPECT_EQ(8, as_index(figurine_colour_enum::black));
    EXPECT_EQ(0, as_index(figurine_colour_enum::white));
}

TEST(FigurineColourEnum, AsLogicDigit) {
    EXPECT_EQ(on, as_logic_digit(figurine_colour_enum::black));
    EXPECT_EQ(off, as_logic_digit(figurine_colour_enum::white));
}

TEST(FigurineColourEnum, Inserter) {
    std::ostringstream out;
    out << figurine_colour_enum::white << figurine_colour_enum::black;
    EXPECT_EQ("wb", out.str());
}

TEST(FigurineColourEnum, Flip) {
    figurine_colour_enum flipped_to_black{figurine_colour_enum::white};
    figurine_colour_enum flipped_to_white{figurine_colour_enum::black};
    flip(flipped_to_black);
    flip(flipped_to_white);
    EXPECT_EQ(figurine_colour_enum::black, flipped_to_black);
    EXPECT_EQ(figurine_colour_enum::white, flipped_to_white);
}
