#include "representation/coordinates_type_test.hpp"


#include "representation/coordinate_enum.hpp"


using namespace representation;


TEST(CoordinatesType, IncrementZeroCoordinateVectorWrapAround) {
    coordinates_type coordinates{};
    EXPECT_FALSE(coordinates.next());
}

TEST(CoordinatesType, IncrementOneCoordinateVectora8) {
    coordinates_type coordinates{a8};
    EXPECT_TRUE(coordinates.next());
    EXPECT_EQ(coordinates_type{b8}, coordinates);
}

TEST(CoordinatesType, IncrementOneCoordinateVectorh8) {
    coordinates_type coordinates{h8};
    EXPECT_TRUE(coordinates.next());
    EXPECT_EQ(coordinates_type{a7}, coordinates);
}

TEST(CoordinatesType, IncrementOneCoordinateVectora1) {
    coordinates_type coordinates{a1};
    EXPECT_TRUE(coordinates.next());
    EXPECT_EQ(coordinates_type{b1}, coordinates);
}

TEST(CoordinatesType, IncrementOneCoordinateVectorh1) {
    coordinates_type coordinates{h1};
    EXPECT_FALSE(coordinates.next());
    EXPECT_EQ(coordinates_type{a8}, coordinates);
}

TEST(CoordinatesType, IncrementTwoCoordinateVector) {
    coordinates_type coordinates{h1, f1};
    EXPECT_TRUE(coordinates.next());
    EXPECT_TRUE(coordinates.next());
    EXPECT_FALSE(coordinates.next());
}

TEST(CoordinatesType, Constructb8h1FromIndex) {
    coordinates_type coordinates(2,
     static_cast<unsigned char>(b8.as_enum()) * 64 +
     static_cast<unsigned char>(h1.as_enum()));
    EXPECT_EQ(2, coordinates.size());
    EXPECT_EQ(b8, coordinates[0]);
    EXPECT_EQ(h1, coordinates[1]);
    EXPECT_EQ(127, coordinates.as_index());
}

TEST(CoordinatesType, Constructc8a8FromIndex) {
    coordinates_type coordinates(2,
     static_cast<unsigned char>(c8.as_enum()) * 64 +
     static_cast<unsigned char>(a8.as_enum()));
    EXPECT_EQ(2, coordinates.size());
    EXPECT_EQ(c8, coordinates[0]);
    EXPECT_EQ(a8, coordinates[1]);
    EXPECT_EQ(128, coordinates.as_index());
}

TEST(CoordinatesType, NotOverlapping) {
    coordinates_type coordinates{e3, f7, b2, h3, h4, d7};
    EXPECT_FALSE(coordinates.overlapping());
}

TEST(CoordinatesType, Overlapping) {
    coordinates_type coordinates{e3, f7, b2, e3, h4, d7};
    EXPECT_TRUE(coordinates.overlapping());
}
