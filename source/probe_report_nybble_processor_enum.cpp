#include "probe_report_nybble_processor_enum.hpp"


#include <cassert>
#include <ostream>


bool valid(probe_report_nybble_processor_enum const value) {
    switch (value) {
    case probe_report_nybble_processor_enum::debugger:
    case probe_report_nybble_processor_enum::chesspresso_endgame_table_generator:
        return true;
    case probe_report_nybble_processor_enum::limit:
    default:
        return false;
    }
}


std::underlying_type_t<probe_report_nybble_processor_enum> as_index(
    probe_report_nybble_processor_enum const value
) {
    assert(valid(value));
    return static_cast<std::underlying_type_t<
     probe_report_nybble_processor_enum>>(value);
}


std::string_view as_string_view(
    probe_report_nybble_processor_enum const value
) {
    assert(valid(value));
    return probe_report_nybble_processor_names[as_index(value)];
}


std::optional<probe_report_nybble_processor_enum>
as_probe_report_nybble_processor_enum_opt(std::string_view const name) {
    for (std::size_t i{}; i != probe_report_nybble_processor_names.size();
     ++i) {
        if (name == probe_report_nybble_processor_names[i]) {
             return probe_report_nybble_processor_enum(i);
        }
    }
    return std::nullopt;
}


std::ostream &
operator<<(std::ostream & out, probe_report_nybble_processor_enum const value) {
    return out << as_string_view(value);
}
