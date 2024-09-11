#if !defined(EGT_COMPARISON_EXPAND_RANDOMLY_PARAMETERIZED_TESTING_TYPE)
#define EGT_COMPARISON_EXPAND_RANDOMLY_PARAMETERIZED_TESTING_TYPE


#include "gtest/gtest.h"


#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

#include "cube_list.hpp"
#include "expand_randomly.hpp"
#include "filesystem_stuff.hpp"
#include "indexed_cube_list.hpp"
#include "verify_equivalence.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


struct expand_randomly_bundle_type {
    std::string const material_balance;
    std::string const proc;
    std::optional<unsigned long> seed_opt;
};


std::ostream & operator<<(
    std::ostream & data_out,
    expand_randomly_bundle_type const & bundle
) {
    data_out << "{\"" << bundle.material_balance << "\", \"" << bundle.proc
     << "\", ";
    if (bundle.seed_opt) {
        data_out << *(bundle.seed_opt);
    } else {
        data_out << "nullopt";
    }
    return data_out.put('}');
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"


class expand_randomly_parameterized_testing_type
  : public ::testing::TestWithParam<expand_randomly_bundle_type> {};


TEST_P(
    expand_randomly_parameterized_testing_type,
    VerifyPostExpansionEquivalence
) {
    expand_randomly_bundle_type const & bundle{GetParam()};
    std::cout << bundle << std::endl;

    std::optional<std::filesystem::path> cube_list_to_expand_path_opt{
     cube_list_path_opt(bundle.material_balance, bundle.proc, false)};
    ASSERT_NE(std::nullopt, cube_list_to_expand_path_opt);
    std::optional<cube_list_type> cube_list_to_expand_opt{
     cube_list_type::create_opt(*cube_list_to_expand_path_opt)};
    ASSERT_NE(std::nullopt, cube_list_to_expand_opt);

    std::optional<std::filesystem::path> D01_indexed_cube_list_path_opt{
     cube_list_path_opt(bundle.material_balance, 1, "", true)};
    ASSERT_NE(std::nullopt, D01_indexed_cube_list_path_opt);
    std::optional<indexed_cube_list_type> const D01_indexed_cube_list_opt{
     indexed_cube_list_type::create_opt(*D01_indexed_cube_list_path_opt)};
    ASSERT_NE(std::nullopt, D01_indexed_cube_list_opt);

    expand_randomly(*cube_list_to_expand_opt, *D01_indexed_cube_list_opt);

    bool const difference_found{verify_equivalence(*cube_list_to_expand_opt,
     *D01_indexed_cube_list_opt)};
    EXPECT_FALSE(difference_found);
}


#pragma clang diagnostic pop


#endif // EGT_COMPARISON_EXPAND_RANDOMLY_PARAMETERIZED_TESTING_TYPE
