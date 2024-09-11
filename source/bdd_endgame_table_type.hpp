#if !defined(EGT_COMPARISON_BDD_ENDGAME_TABLE_TYPE_HPP)
#define EGT_COMPARISON_BDD_ENDGAME_TABLE_TYPE_HPP


#include <filesystem>
#include <memory>
#include <vector>

#include "bdd_from_bits/fixed_decision_diagram_type.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "endgame_table_type.hpp"
#include "probe_report_type.hpp"


class bdd_endgame_table_type final : public endgame_table_type {

public:
    bdd_endgame_table_type(bdd_endgame_table_type &&) = default;
    bdd_endgame_table_type(bdd_endgame_table_type const &) = delete;
    bdd_endgame_table_type & operator=(bdd_endgame_table_type &&) = default;
    bdd_endgame_table_type & operator=(bdd_endgame_table_type const &) = delete;
    virtual ~bdd_endgame_table_type() override = default;

    static std::unique_ptr<bdd_endgame_table_type> create_ptr(
     std::istream & in);
    static std::unique_ptr<bdd_endgame_table_type> create_ptr(
     std::filesystem::path const & path);
    static std::unique_ptr<bdd_endgame_table_type> create_ptr(
     representation::material_signature_type const & material_signature,
     std::string_view const concretization_algorithm);
    static std::unique_ptr<bdd_endgame_table_type> create_ptr(
     representation::material_signature_type const & material_signature);

    virtual std::filesystem::path extension() const override {
        return ".mtbdd";
    }

    probe_report_type probe_directly_impl(
        representation::egt_position_type const & position) const override;

private:
    bdd_endgame_table_type();

    virtual void vtable_pinning_method() const override;

    unsigned int the_bdd_endgame_table_root_node_id;
    fixed_decision_diagram_type the_bdd_endgame_table;
};


#endif // EGT_COMPARISON_BDD_ENDGAME_TABLE_TYPE_HPP
