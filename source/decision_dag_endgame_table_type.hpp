#if !defined(EGT_COMPARISON_DECISION_DAG_ENDGAME_TABLE_TYPE_HPP)
#define EGT_COMPARISON_DECISION_DAG_ENDGAME_TABLE_TYPE_HPP


#include <filesystem>
#include <memory>

#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "decision_dag.hpp"
#include "endgame_table_type.hpp"
#include "load_and_save.hpp"
#include "probe_report_type.hpp"


using decision_dag_type = ddag<32, 8, 27>;

class decision_dag_endgame_table_type final : public endgame_table_type {

public:
    decision_dag_endgame_table_type & operator=(
     decision_dag_endgame_table_type const &) = delete;
    decision_dag_endgame_table_type & operator=(
     decision_dag_endgame_table_type &&) = default;
    decision_dag_endgame_table_type(decision_dag_endgame_table_type const &) = delete;
    decision_dag_endgame_table_type(decision_dag_endgame_table_type &&) = default;
    virtual ~decision_dag_endgame_table_type() override = default;
    decision_dag_endgame_table_type(decision_dag_type && decision_dag);

    static std::unique_ptr<decision_dag_endgame_table_type> create_ptr(
     std::istream & in);
    static std::unique_ptr<decision_dag_endgame_table_type> create_ptr(
     std::filesystem::path const & path);
    static std::unique_ptr<decision_dag_endgame_table_type> create_ptr(
     representation::material_signature_type const & material_signature);

    virtual std::filesystem::path extension() const override {
        return ".dtr";
    }

    probe_report_type probe_directly_impl(
        representation::egt_position_type const & position) const override;

private:
    decision_dag_type the_endgame_data;
};


#endif // EGT_COMPARISON_DECISION_DAG_ENDGAME_TABLE_TYPE_HPP
