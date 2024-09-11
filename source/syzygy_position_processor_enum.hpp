#if !defined (EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_ENUM_HPP)
#define EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_ENUM_HPP


#include <array>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <type_traits>


enum class syzygy_position_processor_enum : unsigned char {
    debugger,
    probe_report_nybble_emitter,
    limit
};


// It is essential to mimic the order of the enum values.
inline constexpr
std::array<
    std::string_view,
    static_cast<std::size_t>(syzygy_position_processor_enum::limit)
> syzygy_position_processor_names = {
    "debugger",
    "probe_report_nybble_emitter",
};
static_assert(syzygy_position_processor_enum(syzygy_position_processor_names.size())
 == syzygy_position_processor_enum::limit);


bool valid(syzygy_position_processor_enum value);


std::underlying_type_t<syzygy_position_processor_enum> as_index(
    syzygy_position_processor_enum value);


std::string_view as_string_view(syzygy_position_processor_enum value);


std::optional<syzygy_position_processor_enum>
as_syzygy_position_processor_enum_opt(std::string_view const name);


std::ostream & operator<<(
    std::ostream & out,
    syzygy_position_processor_enum value);


#endif // EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_ENUM_HPP
