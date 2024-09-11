#if !defined (EGT_COMPARISON_CHESSPRESSO_ENDGAME_TABLE_GENERATOR_TYPE_HPP)
#define EGT_COMPARISON_CHESSPRESSO_ENDGAME_TABLE_GENERATOR_TYPE_HPP


#include <filesystem>
#include <fstream>
#include <string>

#include "representation/canonicalization_enum.hpp"
#include "representation/figurine_enum.hpp"
#include "representation/material_signature_type.hpp"
#include "representation/perspective_enum.hpp"
#include "chesspresso_endgame_table_type.hpp"
#include "paths.hpp"
#include "probe_report_nybble_processor_type.hpp"
#include "probe_report_type.hpp"


class chesspresso_endgame_table_generator_type
: public probe_report_nybble_processor_type {

public:
    virtual ~chesspresso_endgame_table_generator_type() override = default;
    chesspresso_endgame_table_generator_type(
        representation::material_signature_type const & material_signature,
        unsigned int const cover_count_to_generate
    ) : probe_report_nybble_processor_type(material_signature,
        cover_count_to_generate)
    {
        // TODO: Update for castling support.
        std::uint64_t const binary_input_logic_digits_required{
         1 + material_signature.size() * 6};
        std::uint64_t const total_bits_required{
         2 * binary_input_logic_digits_required + 3};
        the_egt.the_header[0] = total_bits_required;
    }

private:
    virtual std::string name() const override {
        return "chesspresso_endgame_table_generator_type";
    }

    virtual std::filesystem::path output_path_prefix() const override {
        std::filesystem::path result(chesspresso_path_prefix());
        return result;
    }

    virtual void on_process_commencement() override;
    virtual void on_process_completion() override;

    virtual void record(
        representation::material_signature_type const & material_signature,
        std::size_t position_id,
        representation::egt_position_type const & egt_position,
        probe_report_type wpr_probe_report) override;

    chesspresso_endgame_table_type the_egt;
    std::ofstream the_output_file_stream;
};


#endif // EGT_COMPARISON_CHESSPRESSO_ENDGAME_TABLE_GENERATOR_TYPE_HPP
