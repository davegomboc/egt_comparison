#include "representation/coordinate_type_test.hpp"


#include <unordered_map>


using namespace representation;


TEST(CoordinateType, LessThan) {
    EXPECT_LT(a8, b8);
    EXPECT_LT(b8, h8);
    EXPECT_LT(h8, a7);
    EXPECT_LT(a7, f4);
    EXPECT_LT(f4, f3);
    EXPECT_LT(f3, e2);
    EXPECT_LT(e2, a1);
    EXPECT_LT(a1, g1);
    EXPECT_LT(g1, h1);
    EXPECT_LT(f6, c3);
}


TEST(CoordinateType, AppendToBoolVector) {
    std::vector<bool> expected_bits{true, false, true, true, true, true};
    std::vector<bool> bits{};
    h3.append(bits);
    EXPECT_TRUE(std::equal(expected_bits.cbegin(), expected_bits.cend(),
     bits.cbegin(), bits.cend()));
}

TEST(CoordinateType, AppendToUnsignedCharVector) {
    std::vector<unsigned char> expected_bits_as_uchars{
     true, false, true, true, true, true};
    std::vector<unsigned char> bits_as_uchars{};
    h3.append(bits_as_uchars);
    EXPECT_TRUE(std::equal(expected_bits_as_uchars.cbegin(),
     expected_bits_as_uchars.cend(), bits_as_uchars.cbegin(),
     bits_as_uchars.cend()));
}


TEST(CoordinateType, InitializeFromIdAsBoolVector) {
    std::vector<bool> bits{true, false, true, true, true, true};
    coordinate_type const coordinate(bits);
    EXPECT_TRUE(bits.empty());
    EXPECT_EQ(h3, coordinate);
}


TEST(CoordinateType, InitializeFromIdAsUnsignedCharVector) {
    std::vector<unsigned char> bits_as_uchars{
     true, false, true, true, true, true};
    coordinate_type const coordinate(bits_as_uchars);
    EXPECT_TRUE(bits_as_uchars.empty());
    EXPECT_EQ(h3, coordinate);
}


TEST(CoordinateType, ParseEmptyString) {
    std::string const coordinate{""};
    auto const coordinate_opt{coordinate_type::create_opt(coordinate)};
    ASSERT_FALSE(coordinate_opt);
}


TEST(CoordinateType, ParseInvalidCoordinate) {
    std::string const coordinate{"4e"};
    auto const coordinate_opt{coordinate_type::create_opt(coordinate)};
    ASSERT_FALSE(coordinate_opt);
}


TEST(CoordinateType, ParseValidCoordinate) {
    std::string const coordinate{"e4"};
    auto const coordinate_opt{coordinate_type::create_opt(coordinate)};
    ASSERT_TRUE(coordinate_opt);
    ASSERT_EQ(e4, *coordinate_opt);
}


TEST(CoordinateType, UsableAsKeyOfUnorderedMap) {
    std::unordered_map<coordinate_type, std::string> associative_array;

    for (unsigned char square{}; square != 64; ++square) {
        coordinate_type const coordinate(square);
        associative_array[coordinate] = coordinate.as_string();
    }

    for (auto const & [key, value]: associative_array) {
        EXPECT_EQ(value, key.as_string());
    }
}
