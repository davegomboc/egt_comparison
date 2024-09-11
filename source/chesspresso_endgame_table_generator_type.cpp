#include "chesspresso_endgame_table_generator_type.hpp"


#include <algorithm>
#include <cassert>
#include <cstdlib>

#include "representation/coordinates_type.hpp"
#include "representation/cover_type.hpp"
#include "representation/cube_type.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/logic_digits_type.hpp"


using representation::as_bool_vector_id;
using representation::as_string_view;
using representation::coordinates_type;
using representation::cover_enum;
using representation::cover_type;
using representation::cube_type;
using representation::egt_position_type;
using representation::logic_digits_type;


void chesspresso_endgame_table_generator_type::on_process_commencement() {
    std::filesystem::path output_file{output_path_prefix()};
    output_file /= chesspresso_path_suffix(material_signature(), std::nullopt,
     true, false);
    the_output_file_stream.open(output_file);
    if (not the_output_file_stream) {
        std::cerr << "EE: Failed to open " << output_file << ".\n";
        std::exit(EXIT_FAILURE);
    }
    std::cerr << "II: Opened " << output_file << ".\n";
}


void chesspresso_endgame_table_generator_type::on_process_completion() {
    for (std::size_t nth_cover{}; nth_cover != 3; ++nth_cover) {
        the_egt.the_header[1 + nth_cover] =
         the_egt.the_FRD_cover_ptrs[nth_cover]->size();
        cover_type & cover{*the_egt.the_FRD_cover_ptrs[nth_cover]};
        if (not cover.is_sorted()) {
            cover.sort();
        }
    }

    bool const write_failed{the_egt.write(the_output_file_stream, true)};
    the_output_file_stream.flush();
    the_output_file_stream.close();
    if (write_failed) {
        std::cerr <<
         "EE: A failure occurred during chesspresso endgame table generation.";
        std::exit(EXIT_FAILURE);
    }
}


void dump_cube(cube_type const & cube, cover_enum const cover) {
    std::clog << "II: Adding to cover ";
    switch (cover) {
    case cover_enum::function:
        std::clog << 'F';
        break;
    case cover_enum::reverse:
        std::clog << 'R';
        break;
    case cover_enum::do_not_care:
        std::clog << 'D';
        break;
    default:
        assert(false);
        __builtin_unreachable();
    }
    std::clog << ":\nII: ";
    cube.print_as_hex_chars(std::clog);
    std::clog << "\nII: ";
    cube.print_as_hex(std::clog);
    std::clog << "\nII:";
    cube.debug_print(std::clog);
    std::clog << "\nII: ";
    cube.probe_debug_print(std::clog);
    std::clog << "\n\n";
}


void chesspresso_endgame_table_generator_type::record(
    representation::material_signature_type const & /*material_signature*/,
    std::size_t const /*position_id*/,
    representation::egt_position_type const & egt_position,
    probe_report_type const wpr_probe_report
) {
    bool const emit_tracing{};
    if (emit_tracing) {
        std::clog << "II: egt_position = " << egt_position
         << "\nII: wpr_probe_report = " << wpr_probe_report << '\n';
    }
    logic_digits_type const logic_digits{egt_position.as_logic_digits()};
    if (emit_tracing) {
        std::clog << "II: logic_digits = " << logic_digits << '\n';
    }

    if ((unknown == wpr_probe_report) or (illegal == wpr_probe_report)
     or (invalid == wpr_probe_report) or (redundant == wpr_probe_report)) {
        // There is only a D cube to be recorded.
        if (record_D()) {
            auto & D_cover{*the_egt.the_FRD_cover_ptrs[
             static_cast<unsigned char>(cover_enum::do_not_care)]};
            cube_type && D_cube(prepare_cube_for_cover(logic_digits,
             cover_enum::do_not_care, wpr_probe_report));
            if (emit_tracing) {
                dump_cube(D_cube, cover_enum::do_not_care);
            }
            D_cover.push_back(D_cube);
        }
        return;
    }

    // There are only F and R cubes to be recorded.  (Though, for our
    // application, we can actually infer R from F.)
    if (record_F()) {
        auto & F_cover{*the_egt.the_FRD_cover_ptrs[
         static_cast<unsigned char>(cover_enum::function)]};
        cube_type && F_cube(prepare_cube_for_cover(logic_digits,
         cover_enum::function, wpr_probe_report));
        if (emit_tracing) {
            dump_cube(F_cube, cover_enum::function);
        }
        F_cover.push_back(F_cube);
    }

    if (record_R()) {
        auto & R_cover{*the_egt.the_FRD_cover_ptrs[
         static_cast<unsigned char>(cover_enum::reverse)]};
        cube_type && R_cube(prepare_cube_for_cover(logic_digits,
         cover_enum::reverse, wpr_probe_report));
        if (emit_tracing) {
            dump_cube(R_cube, cover_enum::reverse);
        }
        R_cover.push_back(R_cube);
    }
}
