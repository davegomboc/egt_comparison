#include "syzygy_position_processor_factory_type.hpp"


#include <cassert>
#include <memory>

#include "probe_report_nybble_emitter_type.hpp"
#include "syzygy_position_processor_debugger_type.hpp"


std::unique_ptr<syzygy_position_processor_type>
syzygy_position_processor_factory_type::create_ptr(
    syzygy_position_processor_enum const processor
) {
    switch (processor) {
    case syzygy_position_processor_enum::debugger:
        return std::make_unique<syzygy_position_processor_debugger_type>();
    case syzygy_position_processor_enum::probe_report_nybble_emitter:
        return std::make_unique<probe_report_nybble_emitter_type>();
    case syzygy_position_processor_enum::limit:
    default:
        assert(false);
    }
    __builtin_unreachable();
}
