#include "probe_report_enum.hpp"


#include <ostream>

#include "epd/operand_type.hpp"
#include "epd/operands_type.hpp"


std::optional<probe_report_enum>
to_probe_report_enum_opt(
    epd::operation_type const game_theoretic_value_operation
) {
    if ("GTV" != game_theoretic_value_operation.opcode().as_epd()) {
        return std::nullopt;
    }
    epd::operands_type const operands(game_theoretic_value_operation.operands());
    if (1 != operands.size()) {
        return std::nullopt;
    }
    epd::operand_type const operand(operands[0]);
    std::string const value(operand.as_epd());
    if (1 != value.size()) {
        return std::nullopt;
    }
    return to_probe_report_enum_opt(value[0]);
}


std::ostream & operator<<(std::ostream & out, probe_report_enum probe_report) {
    assert(valid(probe_report));
    return out.put(as_char(probe_report));
}
