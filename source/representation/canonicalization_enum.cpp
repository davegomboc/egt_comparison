#include "canonicalization_enum.hpp"


#include <cassert>
#include <ostream>


namespace representation {


bool valid(canonicalization_enum const value) {
    switch (value) {
    case canonicalization_enum::single_symmetry:
    case canonicalization_enum::all_symmetries:
        return true;
    case canonicalization_enum::limit:
    default:
        return false;
    }
}


std::underlying_type_t<canonicalization_enum> as_index(
    canonicalization_enum const value
) {
    assert(valid(value));
    return static_cast<std::underlying_type_t<canonicalization_enum>>(value);
}


std::string_view as_string_view(canonicalization_enum const value) {
    assert(valid(value));
    return canonicalization_names[as_index(value)];
}


std::optional<canonicalization_enum>
as_canonicalization_enum_opt(std::string_view const name) {
    for (std::size_t i{}; i != canonicalization_names.size(); ++i) {
        if (name == canonicalization_names[i]) {
            return canonicalization_enum(i);
        }
    }
    return std::nullopt;
}


std::ostream & operator<<(
    std::ostream & out,
    canonicalization_enum const value
) {
    return out << as_string_view(value);
}


} // namespace representation
