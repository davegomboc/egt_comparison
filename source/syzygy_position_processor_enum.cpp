#include "syzygy_position_processor_enum.hpp"


#include <cassert>
#include <ostream>


bool valid(syzygy_position_processor_enum const value) {
    switch (value) {
    case syzygy_position_processor_enum::debugger:
    case syzygy_position_processor_enum::probe_report_nybble_emitter:
        return true;
    case syzygy_position_processor_enum::limit:
    default:
        return false;
    }
}


std::underlying_type_t<syzygy_position_processor_enum> as_index(
    syzygy_position_processor_enum const value
) {
    assert(valid(value));
    return static_cast<std::underlying_type_t<syzygy_position_processor_enum>>(value);
}


std::string_view as_string_view(syzygy_position_processor_enum const value) {
    assert(valid(value));
    return syzygy_position_processor_names[as_index(value)];
}


std::optional<syzygy_position_processor_enum>
as_syzygy_position_processor_enum_opt(std::string_view const name) {
    for (std::size_t i{}; i != syzygy_position_processor_names.size(); ++i) {
        if (name == syzygy_position_processor_names[i]) {
            return syzygy_position_processor_enum(i);
        }
    }
    return std::nullopt;
}


std::ostream & operator<<(
    std::ostream & out,
    syzygy_position_processor_enum const value
) {
    return out << as_string_view(value);
}
