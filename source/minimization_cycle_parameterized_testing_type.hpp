#if !defined(EGT_COMPARISON_MINIMIZATION_CYCLE_PARAMETERIZED_TESTING_TYPE)
#define EGT_COMPARISON_MINIMIZATION_CYCLE_PARAMETERIZED_TESTING_TYPE


#include "gtest/gtest.h"


#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <optional>

#include "cube_list.hpp"
#include "filesystem_stuff.hpp"
#include "indexed_cube_list.hpp"
#include "minimization_cycle.hpp"
#include "verify_equivalence.hpp"


struct minimization_cycle_bundle_type {
    std::string material_balance;
    std::string proc;
    bool do_expand_randomly;
    bool do_make_irredundant;
    bool do_reduce;
    std::optional<unsigned long> seed_opt;
};


std::ostream & operator<<(
    std::ostream & data_out,
    minimization_cycle_bundle_type const & bundle
) {
    data_out << "{\"" << bundle.material_balance << "\", \"" << bundle.proc
     << "\", ";
    data_out.put(bundle.do_expand_randomly ? 'E' : 'e');
    data_out.put(bundle.do_make_irredundant ? 'I' : 'i');
    data_out.put(bundle.do_reduce ? 'R' : 'r');
    data_out << ", ";
    if (bundle.seed_opt) {
        data_out << *(bundle.seed_opt);
    } else {
        data_out << "nullopt";
    }
    return data_out << '}';
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"


class minimization_cycle_parameterized_testing_type
  : public ::testing::TestWithParam<minimization_cycle_bundle_type> {};


TEST_P(
    minimization_cycle_parameterized_testing_type,
    VerifyEquivalence
) {
    minimization_cycle_bundle_type const & bundle{GetParam()};
    std::cout << bundle << std::endl;

    using cube_list_type = cube_list_tt<32, 8>;
    std::optional<std::filesystem::path> cubes_to_minimize_path_opt{
     cube_list_path_opt(bundle.material_balance, bundle.proc, false)};
    ASSERT_NE(std::nullopt, cubes_to_minimize_path_opt);
    std::optional<cube_list_type> cubes_to_minimize_opt{
     cube_list_type::create_opt(*cubes_to_minimize_path_opt)};
    ASSERT_NE(std::nullopt, cubes_to_minimize_opt);

    using indexed_cube_list_type
     = indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30>;
    std::optional<std::filesystem::path> D00_indexed_cube_list_path_opt{
     cube_list_path_opt(bundle.material_balance, 0, "", true)};
    ASSERT_NE(std::nullopt, D00_indexed_cube_list_path_opt);
    std::optional<indexed_cube_list_type> const D00_indexed_cube_list_opt{
     indexed_cube_list_type::create_opt(*D00_indexed_cube_list_path_opt)};
    ASSERT_NE(std::nullopt, D00_indexed_cube_list_opt);

    std::optional<std::filesystem::path> D01_indexed_cube_list_path_opt{
     cube_list_path_opt(bundle.material_balance, 1, "", true)};
    ASSERT_NE(std::nullopt, D01_indexed_cube_list_path_opt);
    std::optional<indexed_cube_list_type> const D01_indexed_cube_list_opt{
     indexed_cube_list_type::create_opt(*D01_indexed_cube_list_path_opt)};
    ASSERT_NE(std::nullopt, D01_indexed_cube_list_opt);

    bool const check_outputs{true};
    do_minimization_cycle(*cubes_to_minimize_opt,
     std::addressof(*D00_indexed_cube_list_opt),
     std::addressof(*D01_indexed_cube_list_opt), bundle.do_expand_randomly,
     bundle.do_make_irredundant, bundle.do_reduce, check_outputs,
     bundle.seed_opt);

    bool const difference_found{verify_equivalence(*cubes_to_minimize_opt,
     *D01_indexed_cube_list_opt)};
    EXPECT_FALSE(difference_found);
}


#pragma clang diagnostic pop


#endif // EGT_COMPARISON_MINIMIZATION_CYCLE_PARAMETERIZED_TESTING_TYPE
