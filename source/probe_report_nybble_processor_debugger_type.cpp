#include "probe_report_nybble_processor_debugger_type.hpp"


#include <iostream>

#include "representation/canonicalization_enum.hpp"
#include "representation/coordinates_type.hpp"
#include "representation/egt_position_type.hpp"


using representation::as_bool_vector_id;
using representation::canonicalization_enum;
using representation::coordinates_type;
using representation::egt_position_type;
using representation::logic_digits_type;


void probe_report_nybble_processor_debugger_type::on_process_commencement() {
    std::clog << "DD: Commencing debugger on " << material_signature() << '.'
     << std::endl;
}

void probe_report_nybble_processor_debugger_type::on_process_completion() {
    std::clog << "DD: Completing debugger on " << material_signature() << '.'
     << std::endl;
}

void probe_report_nybble_processor_debugger_type::record(
    representation::material_signature_type const & material_signature,
    std::size_t const position_id,
    egt_position_type const & egt_position,
    probe_report_type const wpr_probe_report
) {
    egt_position_type const canonical_egt_position(
     egt_position.canonical_position(canonicalization_enum::all_symmetries));
    std::clog << "DD:     material_signature = " << material_signature
     << "\nDD:            position_id = " << position_id
     << "\nDD:           egt_position = " << egt_position
     << "\nDD: canonical_egt_position = " << canonical_egt_position
     << "\nDD:       wpr_probe_report = " << wpr_probe_report.as_description()
     << "\n\n";
    if (egt_position == canonical_egt_position) {
        if (redundant == wpr_probe_report) {
            std::cerr << "EE: This egt_position is canonical, so the "
             "wpr_probe_report must not be redundant!\nEE: " << egt_position
             << "\nEE: (" << material_signature << ", " << position_id << ")\n";
            std::exit(EXIT_FAILURE);
        }
    } else {
        if (redundant != wpr_probe_report) {
            std::cerr << "EE: This egt_position is not canonical, so the "
             "wpr_probe_report must not be redundant!\nEE: " << egt_position
             << "\nEE: (" << material_signature << ", " << position_id << ")\n";
            std::exit(EXIT_FAILURE);
        }
    }
}
