#if !defined (EGT_COMPARISON_DECISION_DAG_ENDGAME_TABLE_PARAMETERIZED_TESTING_TYPE_HPP)
#define EGT_COMPARISON_DECISION_DAG_ENDGAME_TABLE_PARAMETERIZED_TESTING_TYPE_HPP


#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>

#include "gtest/gtest.h"

#include "epd/extended_position_description_type.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "decision_dag_endgame_table_type.hpp"
#include "fen_position_wcdbl_verification_type.hpp"
#include "paths.hpp"
#include "probe_report_type.hpp"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"

class decision_dag_endgame_table_parameterized_testing_type
  : public ::testing::TestWithParam<fen_position_wcdbl_verification_type> {};

#pragma clang diagnostic pop


TEST_P(decision_dag_endgame_table_parameterized_testing_type, ProbeEndgameTable) {
    fen_position_wcdbl_verification_type const & test(GetParam());
    std::cerr << "TT: " << test << std::endl;

    std::optional<epd::extended_position_description_type> const epd_opt{
     epd::extended_position_description_type::create_from_fen(
     test.fen_position)};
    ASSERT_NE(std::nullopt, epd_opt);
    representation::egt_position_type egt_position(*epd_opt);

    representation::material_signature_type material_signature(
     egt_position.placements().material_signature());
    material_signature.canonicalize();
    std::filesystem::path const path{decision_dag_path(material_signature,
     false)};
    std::unique_ptr<decision_dag_endgame_table_type> egt_ptr{
     decision_dag_endgame_table_type::create_ptr(path)};
    ASSERT_NE(nullptr, egt_ptr);

    probe_report_type const expected_wpr(test.expected_wpr);
    probe_report_type const observed_wpr(egt_ptr->probe_after_canonicalizing(
     egt_position));
    EXPECT_EQ(expected_wpr, observed_wpr);
}


#endif // EGT_COMPARISON_DECISION_DAG_ENDGAME_TABLE_PARAMETERIZED_TESTING_TYPE_HPP
