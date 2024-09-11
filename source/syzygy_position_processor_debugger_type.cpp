#include "syzygy_position_processor_debugger_type.hpp"


#include <iomanip>
#include <iostream>

#include "representation/board_8x8_type.hpp"
#include "representation/bitboards_type.hpp"


using representation::board_8x8_type;
using representation::canonicalization_enum;
using representation::coordinate_type;
using representation::egt_position_type;
using representation::material_signature_type;
using representation::perspective_enum;


bool constexpr dump_only_positions_with_en_passant_targets{false};
bool constexpr dump_summary_line{true};
bool constexpr dump_bitboards{false};
bool constexpr dump_tb_position{false};
bool constexpr dump_tb_position_details{true};
bool constexpr depict_positions{false};


void syzygy_position_processor_debugger_type::on_process_table_completion(
    canonicalization_enum const /*canonicalization*/,
    perspective_enum const /*perspective*/,
    material_signature_type const & /*material_signature*/
) {
    dump_probe_report_counters(std::clog);
}

void syzygy_position_processor_debugger_type::record(
    material_signature_type const & material_signature,
    std::size_t const position_id,
    egt_position_type const & noncanonicalized_egt_position,
    probe_report_type const finalized_probe_report
) {
    std::size_t const input_bit_pair_count{
     material_signature.cube_schema().binary_variable_count()};
    if (dump_only_positions_with_en_passant_targets) {
        if (representation::no_ep
         == noncanonicalized_egt_position.en_passant_target()) {
            return;
        }
    }
    if (dump_summary_line) {
        std::vector<bool> const id_as_bool_vector{
         representation::as_bool_vector_id(input_bit_pair_count, position_id)};
        assert(id_as_bool_vector
         == noncanonicalized_egt_position.as_bool_vector_id());
        std::clog << "II: " << std::setw(12) << position_id << ' ';
        for (std::size_t i{}; i != id_as_bool_vector.size(); ++i) {
            std::clog.put(id_as_bool_vector[i] ? '1' : '0');
        }
        std::clog.put(' ') << (noncanonicalized_egt_position.white_to_move()
         ? " wtm (0) " : " btm (1) ")
         << noncanonicalized_egt_position.placements() << " -> "
         << finalized_probe_report << " ("
         << static_cast<unsigned int>(finalized_probe_report.as_enum()) << ", "
         << finalized_probe_report.as_description() << ")\n";
    }
    if (dump_bitboards) {
        auto [overlapping, bitboards]{
         noncanonicalized_egt_position.placements().as_bitboards()};
        for (std::size_t bitboard_index{}; bitboard_index != 8; ++bitboard_index) {
            std::clog << "DD: " << std::setw(6)
             << representation::bitboard_name(bitboard_index) << ": "
             << bitboards[bitboard_index] << '\n';
        }
    }
    if (dump_tb_position or depict_positions) {
        std::optional<tb_position_type> tb_position_opt{
         noncanonicalized_egt_position.as_tb_position_opt()};
        if (std::nullopt == tb_position_opt) {
            if (dump_tb_position) {
                std::clog << "DD: tb_position_opt = std::nullopt.\n";
            }
        } else {
            if (dump_tb_position) {
                std::clog << "DD: *tb_position_opt for "
                 << tb_position_opt->as_fen();
                if (dump_tb_position_details) {
                    std::clog << ":\n";
                    tb_position_opt->dump_2d_fields(std::clog);
                } else {
                    std::clog << " exists.\n";
                }
            }
            if (depict_positions) {
                std::optional<epd::board_contents_type> board_contents_opt{
                 epd::board_contents_type::create(
                 tb_position_opt->fen_board_segment())};
                assert(std::nullopt != board_contents_opt);
                std::clog << board_8x8_type(*board_contents_opt)
                 .as_renderable_string(false);
            }
        }
    }
}
