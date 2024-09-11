#if !defined (EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_ENUM_HPP)
#define EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_ENUM_HPP


#include <array>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <type_traits>


enum class probe_report_nybble_processor_enum : unsigned char {
    debugger,
    chesspresso_endgame_table_generator,
    limit,
};


// It is essential to mimic the order of the enum values.
inline constexpr
std::array<
    std::string_view,
    static_cast<std::size_t>(probe_report_nybble_processor_enum::limit)
> probe_report_nybble_processor_names = {
    "debugger",
    "chesspresso_endgame_table_generator",
};
static_assert(probe_report_nybble_processor_enum(
 probe_report_nybble_processor_names.size())
 == probe_report_nybble_processor_enum::limit);


bool valid(probe_report_nybble_processor_enum value);


std::underlying_type_t<probe_report_nybble_processor_enum> as_index(
    probe_report_nybble_processor_enum value);


std::string_view as_string_view(probe_report_nybble_processor_enum value);


std::optional<probe_report_nybble_processor_enum>
as_probe_report_nybble_processor_enum_opt(std::string_view const name);


std::ostream & operator<<(
    std::ostream & out,
    probe_report_nybble_processor_enum value);


#endif // EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_ENUM_HPP
