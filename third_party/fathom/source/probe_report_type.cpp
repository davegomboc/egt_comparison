#include "probe_report_type.hpp"


#include <ostream>


std::optional<probe_report_type> probe_report_type::create(
    epd::operation_type const game_theoretic_value_operation
) {
    std::optional<probe_report_enum> const probe_report_enum_opt{
        to_probe_report_enum_opt(game_theoretic_value_operation)};
    if (std::nullopt == probe_report_enum_opt) {
        return std::nullopt;
    }
    return probe_report_type(*probe_report_enum_opt);
}


std::ostream &
operator<<(std::ostream & out, probe_report_type const probe_report) {
    return out.put(probe_report.as_char());
}
