#if !defined(EGT_COMPARISON_LOOK_UP_PARAMETERIZED_TESTING_TYPE_HPP)
#define EGT_COMPARISON_LOOK_UP_PARAMETERIZED_TESTING_TYPE_HPP


#include "gtest/gtest.h"


#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cube_list.hpp"
#include "filesystem_stuff.hpp"
#include "indexed_cube_list.hpp"


using cube_binary_inputs_type = cube_binary_inputs_tt<32>;
using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


struct look_up_bundle_type {
    std::string const material_balance;
    std::string const proc;
    cube_binary_inputs_type probe;
    std::vector<unsigned int> const care_outputs;
};


std::ostream & operator<<(
    std::ostream & data_out,
    look_up_bundle_type const & bundle
) {
    data_out << "{\"" << bundle.material_balance << "\", \"" << bundle.proc
     << "\", " << bundle.probe << ", {";
    if (not bundle.care_outputs.empty()) {
        bool have_emitted_a_care_output{};
        for (auto const care_output: bundle.care_outputs) {
            if (have_emitted_a_care_output) {
                data_out << ", ";
            }
            data_out << unsigned(care_output);
            have_emitted_a_care_output = true;
        }
    }
    return data_out << "}}";
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"


void check_obtaining_all_outputs(
    indexed_cube_list_type const & indexed_cube_list,
    cube_binary_inputs_type probe,
    std::vector<unsigned int> const & care_outputs
) {
    auto const all_outputs{indexed_cube_list.lookupall(probe)};
    std::cout << "all_outputs.size() = " << all_outputs.size() << std::endl;
    EXPECT_EQ(all_outputs.size(), care_outputs.size());
    for (auto const output: all_outputs) {
        std::cout << "found output: " << unsigned(output) << std::endl;
        EXPECT_NE(care_outputs.end(), std::find(care_outputs.cbegin(),
         care_outputs.cend(), unsigned(output)));
    }
}

void check_obtaining_some_output(
    indexed_cube_list_type const & indexed_cube_list,
    cube_binary_inputs_type probe,
    std::vector<unsigned int> const & care_outputs
) {
    std::cout << "care_outputs =";
    for (auto const care_output: care_outputs) {
        std::cout << ' ' << unsigned(care_output);
    }
    auto const some_output{indexed_cube_list.lookup(probe)};
    std::cout << "\nsome_output = " << unsigned(some_output) << std::endl;
    if (0 != some_output) {
        EXPECT_NE(care_outputs.end(), std::find(care_outputs.cbegin(),
         care_outputs.cend(), unsigned(some_output)));
    }
}


class look_up_parameterized_testing_type
  : public ::testing::TestWithParam<look_up_bundle_type> {};


TEST_P(
    look_up_parameterized_testing_type,
    ObtainAllOutputsAfterLoadingCubeListThenIndexing
) {
    look_up_bundle_type const & bundle{GetParam()};
    std::cout << bundle << std::endl;

    std::optional<std::filesystem::path> path_opt{
     cube_list_path_opt(bundle.material_balance, bundle.proc, false)};
    ASSERT_NE(std::nullopt, path_opt);
    std::optional<cube_list_type> cube_list_opt{
     cube_list_type::create_opt(*path_opt)};
    ASSERT_NE(std::nullopt, cube_list_opt);
    indexed_cube_list_type const indexed_cube_list(*cube_list_opt);

    check_obtaining_all_outputs(indexed_cube_list, bundle.probe,
     bundle.care_outputs);
}

TEST_P(
    look_up_parameterized_testing_type,
    ObtainAllOutputsAfterDirectlyLoadingIndexedCubeList
) {
    look_up_bundle_type const & bundle{GetParam()};
    std::cout << bundle << std::endl;

    std::optional<std::filesystem::path> path_opt{
     cube_list_path_opt(bundle.material_balance, bundle.proc, true)};
    ASSERT_NE(std::nullopt, path_opt);
    std::optional<indexed_cube_list_type> indexed_cube_list_opt{
     indexed_cube_list_type::create_opt(*path_opt)};
    ASSERT_NE(std::nullopt, indexed_cube_list_opt);

    check_obtaining_all_outputs(*indexed_cube_list_opt, bundle.probe,
     bundle.care_outputs);
}

TEST_P(
    look_up_parameterized_testing_type,
    ObtainSomeOutputAfterLoadingCubeListThenIndexing
) {
    look_up_bundle_type const & bundle{GetParam()};
    std::cout << bundle << std::endl;

    std::optional<std::filesystem::path> path_opt{
     cube_list_path_opt(bundle.material_balance, bundle.proc, false)};
    ASSERT_NE(std::nullopt, path_opt);
    std::optional<cube_list_type> cube_list_opt{
     cube_list_type::create_opt(*path_opt)};
    ASSERT_NE(std::nullopt, cube_list_opt);
    indexed_cube_list_type const indexed_cube_list(*cube_list_opt);

    check_obtaining_some_output(indexed_cube_list, bundle.probe,
     bundle.care_outputs);
}

TEST_P(
    look_up_parameterized_testing_type,
    ObtainSomeOutputAfterDirectlyLoadingIndexedCubeList
) {
    look_up_bundle_type const & bundle{GetParam()};
    std::cout << bundle << std::endl;

    std::optional<std::filesystem::path> path_opt{
     cube_list_path_opt(bundle.material_balance, bundle.proc, true)};
    ASSERT_NE(std::nullopt, path_opt);
    std::optional<indexed_cube_list_type> indexed_cube_list_opt{
     indexed_cube_list_type::create_opt(*path_opt)};
    ASSERT_NE(std::nullopt, indexed_cube_list_opt);

    check_obtaining_some_output(*indexed_cube_list_opt, bundle.probe,
     bundle.care_outputs);
}


#pragma clang diagnostic pop


#endif // EGT_COMPARISON_LOOK_UP_PARAMETERIZED_TESTING_TYPE_HPP
