#include "cube_list_test.hpp"


#include <random>
#include <sstream>

#include "filesystem_stuff.hpp"


using cube_list_type = cube_list_tt<32, 8>;


TEST(CubeList, LoadKVskD00I) {
    auto const path_opt(cube_list_path_opt("K_vs_k", 0, "", false));
    ASSERT_TRUE(path_opt);
    std::optional<cube_list_type> const cube_list_opt{
     cube_list_type::create_opt(*path_opt)};
    EXPECT_TRUE(cube_list_opt);
}

TEST(CubeList, LoadKVskD01I) {
    auto const path_opt(cube_list_path_opt("K_vs_k", 1, "", false));
    ASSERT_TRUE(path_opt);
    std::optional<cube_list_type> const cube_list_opt{
     cube_list_type::create_opt(*path_opt)};
    EXPECT_TRUE(cube_list_opt);
}

TEST(CubeList, RoundTrippingKVsKYieldsEquivalentObjects) {
    auto const path_opt(cube_list_path_opt("K_vs_k", 0, "", false));
    ASSERT_TRUE(path_opt);
    auto const cube_list_opt{cube_list_type::create_opt(*path_opt)};
    ASSERT_TRUE(cube_list_opt);

    std::stringstream data_out_one;
    bool const write_to_one_failed{cube_list_opt->serialize(data_out_one)};
    ASSERT_FALSE(write_to_one_failed);

    auto const cube_list_one_opt{cube_list_type::create_opt(data_out_one)};
    ASSERT_TRUE(cube_list_one_opt);

    std::stringstream data_out_two;
    bool const write_to_two_failed{cube_list_one_opt->serialize(data_out_two)};
    ASSERT_FALSE(write_to_two_failed);

    auto const cube_list_two_opt{cube_list_type::create_opt(data_out_two)};
    ASSERT_TRUE(cube_list_two_opt);

    EXPECT_EQ(*cube_list_opt, *cube_list_one_opt);
    EXPECT_EQ(data_out_one.str(), data_out_two.str());
    EXPECT_EQ(*cube_list_one_opt, *cube_list_two_opt);
}

TEST(CubeList, RoundTrippingKBVsKYieldsEquivalentObjects) {
    auto const path_opt(cube_list_path_opt("KB_vs_k", 0, "", false));
    ASSERT_TRUE(path_opt);
    auto const cube_list_opt{cube_list_type::create_opt(*path_opt)};
    ASSERT_TRUE(cube_list_opt);

    std::stringstream data_out_one;
    bool const write_to_one_failed{cube_list_opt->serialize(data_out_one)};
    ASSERT_FALSE(write_to_one_failed);

    auto const cube_list_one_opt{cube_list_type::create_opt(data_out_one)};
    ASSERT_TRUE(cube_list_one_opt);

    std::stringstream data_out_two;
    bool const write_to_two_failed{cube_list_one_opt->serialize(data_out_two)};
    ASSERT_FALSE(write_to_two_failed);

    auto const cube_list_two_opt{cube_list_type::create_opt(data_out_two)};
    ASSERT_TRUE(cube_list_two_opt);

    EXPECT_EQ(*cube_list_opt, *cube_list_one_opt);
    EXPECT_EQ(data_out_one.str(), data_out_two.str());
    EXPECT_EQ(*cube_list_one_opt, *cube_list_two_opt);
}

TEST(CubeList, RoundTrippingKNVsKYieldsEquivalentObjects) {
    auto const path_opt(cube_list_path_opt("KN_vs_k", 0, "", false));
    ASSERT_TRUE(path_opt);
    auto const cube_list_opt{cube_list_type::create_opt(*path_opt)};
    ASSERT_TRUE(cube_list_opt);

    std::stringstream data_out_one;
    bool const write_to_one_failed{cube_list_opt->serialize(data_out_one)};
    ASSERT_FALSE(write_to_one_failed);

    auto const cube_list_one_opt{cube_list_type::create_opt(data_out_one)};
    ASSERT_TRUE(cube_list_one_opt);

    std::stringstream data_out_two;
    bool const write_to_two_failed{cube_list_one_opt->serialize(data_out_two)};
    ASSERT_FALSE(write_to_two_failed);

    auto const cube_list_two_opt{cube_list_type::create_opt(data_out_two)};
    ASSERT_TRUE(cube_list_two_opt);

    EXPECT_EQ(*cube_list_opt, *cube_list_one_opt);
    EXPECT_EQ(data_out_one.str(), data_out_two.str());
    EXPECT_EQ(*cube_list_one_opt, *cube_list_two_opt);
}

TEST(CubeList, RoundTrippingKPVsKYieldsEquivalentObjects) {
    auto const path_opt(cube_list_path_opt("KP_vs_k", 0, "", false));
    ASSERT_TRUE(path_opt);
    auto const cube_list_opt{cube_list_type::create_opt(*path_opt)};
    ASSERT_TRUE(cube_list_opt);

    std::stringstream data_out_one;
    bool const write_to_one_failed{cube_list_opt->serialize(data_out_one)};
    ASSERT_FALSE(write_to_one_failed);

    auto const cube_list_one_opt{cube_list_type::create_opt(data_out_one)};
    ASSERT_TRUE(cube_list_one_opt);

    std::stringstream data_out_two;
    bool const write_to_two_failed{cube_list_one_opt->serialize(data_out_two)};
    ASSERT_FALSE(write_to_two_failed);

    auto const cube_list_two_opt{cube_list_type::create_opt(data_out_two)};
    ASSERT_TRUE(cube_list_two_opt);

    EXPECT_EQ(*cube_list_opt, *cube_list_one_opt);
    EXPECT_EQ(data_out_one.str(), data_out_two.str());
    EXPECT_EQ(*cube_list_one_opt, *cube_list_two_opt);
}

TEST(CubeList, RoundTrippingKQVsKYieldsEquivalentObjects) {
    auto const path_opt(cube_list_path_opt("KQ_vs_k", 0, "", false));
    ASSERT_TRUE(path_opt);
    auto const cube_list_opt{cube_list_type::create_opt(*path_opt)};
    ASSERT_TRUE(cube_list_opt);

    std::stringstream data_out_one;
    bool const write_to_one_failed{cube_list_opt->serialize(data_out_one)};
    ASSERT_FALSE(write_to_one_failed);

    auto const cube_list_one_opt{cube_list_type::create_opt(data_out_one)};
    ASSERT_TRUE(cube_list_one_opt);

    std::stringstream data_out_two;
    bool const write_to_two_failed{cube_list_one_opt->serialize(data_out_two)};
    ASSERT_FALSE(write_to_two_failed);

    auto const cube_list_two_opt{cube_list_type::create_opt(data_out_two)};
    ASSERT_TRUE(cube_list_two_opt);

    EXPECT_EQ(*cube_list_opt, *cube_list_one_opt);
    EXPECT_EQ(data_out_one.str(), data_out_two.str());
    EXPECT_EQ(*cube_list_one_opt, *cube_list_two_opt);
}

TEST(CubeList, RoundTrippingKRVsKYieldsEquivalentObjects) {
    auto const path_opt(cube_list_path_opt("KR_vs_k", 0, "", false));
    ASSERT_TRUE(path_opt);
    auto const cube_list_opt{cube_list_type::create_opt(*path_opt)};
    ASSERT_TRUE(cube_list_opt);

    std::stringstream data_out_one;
    bool const write_to_one_failed{cube_list_opt->serialize(data_out_one)};
    ASSERT_FALSE(write_to_one_failed);

    auto const cube_list_one_opt{cube_list_type::create_opt(data_out_one)};
    ASSERT_TRUE(cube_list_one_opt);

    std::stringstream data_out_two;
    bool const write_to_two_failed{cube_list_one_opt->serialize(data_out_two)};
    ASSERT_FALSE(write_to_two_failed);

    auto const cube_list_two_opt{cube_list_type::create_opt(data_out_two)};
    ASSERT_TRUE(cube_list_two_opt);

    EXPECT_EQ(*cube_list_opt, *cube_list_one_opt);
    EXPECT_EQ(data_out_one.str(), data_out_two.str());
    EXPECT_EQ(*cube_list_one_opt, *cube_list_two_opt);
}

TEST(CubeList, CanBeShuffledAndSorted) {
    auto const path_opt(cube_list_path_opt("K_vs_k", 0, "", false));
    ASSERT_TRUE(path_opt);
    std::optional<cube_list_type> cube_list_opt{
     cube_list_type::create_opt(*path_opt)};
    ASSERT_TRUE(cube_list_opt);
    std::mt19937_64 generator;
    while (std::is_sorted(cube_list_opt->begin(), cube_list_opt->end())) {
        std::shuffle(cube_list_opt->begin(), cube_list_opt->end(), generator);
    }
    std::sort(cube_list_opt->begin(), cube_list_opt->end());
    EXPECT_TRUE(std::is_sorted(cube_list_opt->begin(), cube_list_opt->end()));
}
