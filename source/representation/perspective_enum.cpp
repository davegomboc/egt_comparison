#include "perspective_enum.hpp"


#include <cassert>
#include <ostream>


namespace representation {


bool valid(perspective_enum const value) {
    switch (value) {
    case perspective_enum::side_to_move:
    case perspective_enum::white_player:
        return true;
    case perspective_enum::limit:
    default:
        return false;
    }
}


std::underlying_type_t<perspective_enum> as_index(
    perspective_enum const value
) {
    assert(valid(value));
    return static_cast<std::underlying_type_t<perspective_enum>>(value);
}


std::string_view as_string_view(perspective_enum const value) {
    assert(valid(value));
    return perspective_names[as_index(value)];
}


std::optional<perspective_enum>
as_perspective_enum_opt(std::string_view const name) {
    for (std::size_t i{}; i != perspective_names.size(); ++i) {
        if (name == perspective_names[i]) {
            return perspective_enum(i);
        }
    }
    return std::nullopt;
}


std::ostream & operator<<(
    std::ostream & out,
    perspective_enum const value
) {
    return out << as_string_view(value);
}


} // namespace representation
