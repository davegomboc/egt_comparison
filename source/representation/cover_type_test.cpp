#include "representation/cover_type_test.hpp"


#include <array>
#include <cstdint>


#include "const_memory_stream_buffer_view_type.hpp"


using representation::cover_enum;
using representation::cube_type;
using representation::dc;
using representation::logic_digits_type;


std::array<std::uint64_t const, 6> constexpr test_data{
    UINT64_C(0x0000'0000'0955'9955),
    UINT64_C(0x0000'0000'0955'a555),
    UINT64_C(0x0000'0000'0956'5955),
    UINT64_C(0x0000'0000'0956'6555),
    UINT64_C(0x0000'0000'0959'a555),
    UINT64_C(0x0000'0000'095a'5955)};


using representation::cover_type;


TEST(RepresentationCoverType, CreateFromTestArray) {
    char const * const beginning{reinterpret_cast<char const *>(
     test_data.data())};
    char const * const past_end{reinterpret_cast<char const *>(
     test_data.data() + test_data.size())};
    const_memory_stream_buffer_view_type const_memory_view(beginning, past_end);
    std::istream data_in(&const_memory_view);

    cover_type six_cubes(6, 28);
    six_cubes.read(data_in);

    EXPECT_EQ(6, six_cubes.size());
    EXPECT_EQ(1, six_cubes[5].uint64s().size());
    EXPECT_EQ(UINT64_C(0x0000000009559955), six_cubes[0].uint64s()[0]);
    EXPECT_EQ(UINT64_C(0x000000000955a555), six_cubes[1].uint64s()[0]);
    EXPECT_EQ(UINT64_C(0x0000000009565955), six_cubes[2].uint64s()[0]);
    EXPECT_EQ(UINT64_C(0x0000000009566555), six_cubes[3].uint64s()[0]);
    EXPECT_EQ(UINT64_C(0x000000000959a555), six_cubes[4].uint64s()[0]);
    EXPECT_EQ(UINT64_C(0x00000000095a5955), six_cubes[5].uint64s()[0]);
}

TEST(RepresentationCoverType, PrepareUniversalDoNotCareCubeForDoNotCareCover) {
    logic_digits_type const do_not_cares{dc, dc, dc, dc, dc, dc};
    cube_type const expected{true, true, true, true, true, true, true, true,
     true, true, true, true, true, true, true};
    cube_type const universal_dc_cube(prepare_cube_for_cover(do_not_cares,
     cover_enum::do_not_care, redundant));
    EXPECT_EQ(expected, universal_dc_cube);
}
