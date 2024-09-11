#include "representation/figurine_colour_enum.hpp"


#include <ostream>


namespace representation {


std::ostream & operator<<(
    std::ostream & out,
    figurine_colour_enum const colour
) {
    assert(valid(colour));
    return out.put(as_char(colour));
}


} // namespace representation
