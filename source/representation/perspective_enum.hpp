#if !defined (EGT_COMPARISON_REPRESENTATION_PERSPECTIVE_ENUM_HPP)
#define EGT_COMPARISON_REPRESENTATION_PERSPECTIVE_ENUM_HPP


#include <array>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <type_traits>


namespace representation {


enum class perspective_enum : unsigned char {
    side_to_move,
    white_player,
    limit,
};


// It is essential to mimic the order of the enum values.
inline constexpr
std::array<
    std::string_view,
    static_cast<std::size_t>(perspective_enum::limit)
> perspective_names = {
    "side_to_move",
    "white_player"
};
static_assert(perspective_enum(perspective_names.size())
 == perspective_enum::limit);


bool valid(perspective_enum perspective);


std::underlying_type_t<perspective_enum> as_index(perspective_enum value);


std::string_view as_string_view(perspective_enum value);


std::optional<perspective_enum>
as_perspective_enum_opt(std::string_view const name);


std::ostream & operator<<(std::ostream & out, perspective_enum value);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_PERSPECTIVE_ENUM_HPP
