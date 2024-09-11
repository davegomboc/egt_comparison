#include "representation/cube_type_test.hpp"


#include <array>
#include <cstdint>


#include "const_memory_stream_buffer_view_type.hpp"


using representation::cube_type;


TEST(RepresentationCubeType, OperatorSquareBracketsFetchesProperBits) {
    cube_type cube(101);
    for (std::size_t i{}; i != 101; ++i) {
        switch (i) {
        case 0 * 0:
        case 1 * 1:
        case 2 * 2:
        case 3 * 3:
        case 4 * 4:
        case 5 * 5:
        case 6 * 6:
        case 7 * 7:
        case 8 * 8:
        case 9 * 9:
        case 10 * 10:
            cube.set(i);
            break;
        default:
            cube.reset(i);
        }
    }
    for (std::size_t i{}; i != 101; ++i) {
        switch (i) {
        case 0 * 0:
        case 1 * 1:
        case 2 * 2:
        case 3 * 3:
        case 4 * 4:
        case 5 * 5:
        case 6 * 6:
        case 7 * 7:
        case 8 * 8:
        case 9 * 9:
        case 10 * 10:
            EXPECT_EQ(true, cube[i]);
            break;
        default:
            EXPECT_EQ(false, cube[i]);
        }
    }
}

TEST(RepresentationCubeType, InitializeWithNothing) {
    cube_type cube(0);
    EXPECT_TRUE(cube.empty());
}

TEST(RepresentationCubeType, InitializeSixBitsWithOneUint64) {
    std::array<std::uint64_t const, 1> init{0b011011};
    char const * const beginning{reinterpret_cast<char const *>(init.data())};
    char const * const past_end{
     reinterpret_cast<char const *>(init.data() + init.size())};
    const_memory_stream_buffer_view_type const_memory_view(beginning, past_end);
    std::istream data_in(&const_memory_view);
    cube_type cube(6);
    cube.read(data_in);
    EXPECT_TRUE(cube[0]);
    EXPECT_TRUE(cube[1]);
    EXPECT_FALSE(cube[2]);
    EXPECT_TRUE(cube[3]);
    EXPECT_TRUE(cube[4]);
    for (std::size_t bit_n{5}; bit_n != 64; ++bit_n) {
        EXPECT_FALSE(cube[bit_n]);
    }
}

TEST(RepresentationCubeType, InitializeFifteenBitsWithUint64InitializerList) {
    cube_type cube(15, {0b0110110000011011});
    EXPECT_TRUE(cube[0]);
    EXPECT_TRUE(cube[1]);
    EXPECT_FALSE(cube[2]);
    EXPECT_TRUE(cube[3]);
    EXPECT_TRUE(cube[4]);
    EXPECT_FALSE(cube[5]);
    EXPECT_FALSE(cube[6]);
    EXPECT_FALSE(cube[7]);
    EXPECT_FALSE(cube[8]);
    EXPECT_FALSE(cube[9]);
    EXPECT_TRUE(cube[10]);
    EXPECT_TRUE(cube[11]);
    EXPECT_FALSE(cube[12]);
    EXPECT_TRUE(cube[13]);
    EXPECT_TRUE(cube[14]);
    for (std::size_t bit_n{15}; bit_n != 64; ++bit_n) {
        EXPECT_FALSE(cube[bit_n]);
    }
}

TEST(RepresentationCubeType, LessThanPrioritizesHighUint64s) {
    cube_type higher(128, {UINT64_C( 0), UINT64_C(0x1)});
    cube_type  lower(128, {UINT64_C(-1), UINT64_C(0x0)});
    EXPECT_TRUE(lower < higher);
}

TEST(RepresentationCubeType, LessThanWorksLater) {
    cube_type higher(128, {UINT64_C(0xdeadbeefcafebabe), UINT64_C(0x3)});
    cube_type  lower(128, {UINT64_C(0xcafebabedeadbeef), UINT64_C(0x3)});
    EXPECT_TRUE(lower < higher);
}

TEST(RepresentationCubeType, LessThanFalseWhenEqual) {
    cube_type higher(128, {UINT64_C(0xdeadbeef), UINT64_C(0x1)});
    cube_type  lower(128, {UINT64_C(0xdeadbeef), UINT64_C(0x1)});
    EXPECT_FALSE(lower < higher);
}
