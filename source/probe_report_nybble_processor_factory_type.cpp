#include "probe_report_nybble_processor_factory_type.hpp"


#include <cassert>
#include <memory>

#include "chesspresso_endgame_table_generator_type.hpp"
#include "probe_report_nybble_processor_debugger_type.hpp"
#include "probe_report_nybble_processor_type.hpp"


using representation::canonicalization_enum;
using representation::material_signature_type;
using representation::perspective_enum;


std::unique_ptr<probe_report_nybble_processor_type>
probe_report_nybble_processor_factory_type::create_ptr(
    probe_report_nybble_processor_enum processor,
    canonicalization_enum const /*canonicalization*/,
    perspective_enum const /*perspective*/,
    material_signature_type const & material_signature,
    unsigned int const cover_count_to_generate
) {
    switch (processor) {
    case probe_report_nybble_processor_enum::debugger:
        return std::make_unique<probe_report_nybble_processor_debugger_type>(
         material_signature, cover_count_to_generate);
    case probe_report_nybble_processor_enum
     ::chesspresso_endgame_table_generator:
        return std::make_unique<chesspresso_endgame_table_generator_type>(
         material_signature, cover_count_to_generate);
    case probe_report_nybble_processor_enum::limit:
    default:
        assert(false);
    }
    __builtin_unreachable();
}
