#include "probe_report_nybble_processor_type.hpp"

#include <fcntl.h>
#include <sys/mman.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "representation/canonicalization_enum.hpp"

#include "probe_report_nybble_endgame_table_type.hpp"


using representation::canonicalization_enum;
using representation::egt_position_type;
using representation::figurine_colour_enum;
using representation::material_signature_type;


probe_report_nybble_processor_type::probe_report_nybble_processor_type(
    material_signature_type const & material_signature,
    unsigned int const cover_count_to_generate
) : the_material_signature(material_signature),
    the_cover_count_to_generate(cover_count_to_generate),
    table_ptr(probe_report_nybble_endgame_table_type::create_ptr(
     canonicalization_enum::all_symmetries, material_signature))
{
    if ((cover_count_to_generate < 1) or (3 < cover_count_to_generate)) {
        std::cerr << "Invalid cover count to generate ("
         << cover_count_to_generate << ").\n";
        std::exit(EXIT_FAILURE);
    }
    if (nullptr == table_ptr) {
        std::cerr << "EE: Unable to open probe report table for "
         << material_signature << ".\n";
        std::exit(EXIT_FAILURE);
    }
}


void probe_report_nybble_processor_type::process() {
    on_process_commencement();
    std::size_t const wtm_position_count{table_ptr->size()};
    std::size_t const position_count{2 * wtm_position_count};
    for (std::size_t position_id{}; position_id != position_count;
     ++position_id) {
        egt_position_type const egt_position(the_material_signature,
         position_id);
        probe_report_type const wpr_probe_report{
         table_ptr->stored_wpr_probe_report(egt_position)};
        record(the_material_signature, position_id, egt_position,
         wpr_probe_report);
    }
    on_process_completion();
}


// This primarily exists so that clang++ will emit vtables for this class in
// this translation unit only, instead of all of them.  (See -Wweak-vtables
// documentation for details.)
void probe_report_nybble_processor_type::vtable_pinning_method() const {}
