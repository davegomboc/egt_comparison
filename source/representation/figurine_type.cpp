#include "representation/figurine_type.hpp"


#include <ostream>


namespace representation {


std::optional<figurine_type> figurine_type::create_opt(
    std::optional<figurine_enum> const enum_opt
) {
    if (not enum_opt or not valid(*enum_opt)) return std::nullopt;
    return figurine_type(static_cast<int>(*enum_opt));
}


std::optional<figurine_type> figurine_type::create_opt(char const ch) {
    return create_opt(figurine_enum_opt(ch));
}


logic_digits_type figurine_type::as_logic_digits() const {
    return representation::as_logic_digits(the_figurine);
}


void figurine_type::flip_colour() {
    the_figurine = flipped_colour(the_figurine);
}


std::ostream & operator<<(std::ostream & out, figurine_type figurine) {
    return out << figurine.as_char();
}


} // namespace representation
