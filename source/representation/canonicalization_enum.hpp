#if !defined (EGT_COMPARISON_REPRESENTATION_CANONICALIZATION_ENUM_HPP)
#define EGT_COMPARISON_REPRESENTATION_CANONICALIZATION_ENUM_HPP


#include <array>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <type_traits>


namespace representation {


enum class canonicalization_enum : unsigned char {
    single_symmetry,
    all_symmetries,
    limit,
};


// It is essential to mimic the order of the enum values.
inline constexpr std::array<
    std::string_view,
    static_cast<std::size_t>(canonicalization_enum::limit)
> canonicalization_names = {
    "single_symmetry",
    "all_symmetries"
};
static_assert(canonicalization_enum(canonicalization_names.size())
 == canonicalization_enum::limit);


bool valid(canonicalization_enum value);


std::underlying_type_t<canonicalization_enum> as_index(
    canonicalization_enum value);


std::string_view as_string_view(canonicalization_enum value);


std::optional<canonicalization_enum>
as_canonicalization_enum_opt(std::string_view const name);


std::ostream & operator<<(
    std::ostream & out,
    canonicalization_enum value);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_CANONICALIZATION_ENUM_HPP
