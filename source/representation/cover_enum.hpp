#if !defined (EGT_COMPARISON_REPRESENTATION_COVER_ENUM_HPP)
#define EGT_COMPARISON_REPRESENTATION_COVER_ENUM_HPP


#include <array>
#include <iosfwd>
#include <optional>
#include <string_view>


namespace representation {


enum class cover_enum : unsigned char {
    function = 0,
    reverse = 1,
    do_not_care = 2,
};
constexpr unsigned char cover_enum_limit{3};


constexpr bool cover_enum_is_valid(cover_enum cover) {
    return static_cast<unsigned char>(cover) < cover_enum_limit;
}


// It is essential to mimic the order of the enum values.
inline constexpr
std::array<std::string_view, 3> cover_enum_names = {
    "function",
    "reverse",
    "do_not_care",
};


std::string_view as_string_view(cover_enum);


std::optional<cover_enum>
as_cover_enum(std::string_view name);


std::ostream & operator<<(std::ostream &, cover_enum);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_COVER_ENUM_HPP
