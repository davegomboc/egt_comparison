#if !defined(EGT_COMPARISON_PROBE_REPORT_NYBBLE_ENDGAME_TABLE_TYPE_HPP)
#define EGT_COMPARISON_PROBE_REPORT_NYBBLE_ENDGAME_TABLE_TYPE_HPP


#include <memory>

#include "representation/canonicalization_enum.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "endgame_table_type.hpp"
#include "memory_mapped_file_type.hpp"
#include "probe_report_type.hpp"


class probe_report_nybble_endgame_table_type final : public endgame_table_type {

public:
    probe_report_nybble_endgame_table_type(
     probe_report_nybble_endgame_table_type const & other) = delete;
    probe_report_nybble_endgame_table_type & operator=(
     probe_report_nybble_endgame_table_type const & other) = delete;
    probe_report_nybble_endgame_table_type(
     probe_report_nybble_endgame_table_type &&) = default;
    probe_report_nybble_endgame_table_type & operator=(
     probe_report_nybble_endgame_table_type &&) = default;
    virtual ~probe_report_nybble_endgame_table_type() override = default;

    static std::unique_ptr<probe_report_nybble_endgame_table_type> create_ptr(
     std::filesystem::path const & path,
     representation::canonicalization_enum canonicalization_mode,
     representation::material_signature_type const & material_signature);
    static std::unique_ptr<probe_report_nybble_endgame_table_type> create_ptr(
     std::filesystem::path const & path);
    // N.B.  Assumes perspective = wpr, and that the noncompressed file exists.
    static std::unique_ptr<probe_report_nybble_endgame_table_type> create_ptr(
     representation::canonicalization_enum canonicalization_mode,
     representation::material_signature_type const & material_signature);

    bool empty() const { return the_mapped_endgame_table.empty(); }
    std::size_t size() const { return the_mapped_endgame_table.size(); }

    virtual std::filesystem::path extension() const override {
        return ".wpr";
    }

    probe_report_type stored_wpr_probe_report(std::size_t position_id) const;
    probe_report_type stored_wpr_probe_report(
     representation::egt_position_type const & position) const;
    probe_report_type probe_directly_impl(
     representation::egt_position_type const & position) const override;

private:
    probe_report_nybble_endgame_table_type(
        representation::canonicalization_enum canonicalization_mode,
        std::size_t input_bit_pair_count,
        memory_mapped_file_type && mapped_endgame_table);

    std::size_t the_colour_to_move_bit_mask;
    memory_mapped_file_type the_mapped_endgame_table;
};


#endif // EGT_COMPARISON_PROBE_REPORT_NYBBLE_ENDGAME_TABLE_TYPE_HPP
