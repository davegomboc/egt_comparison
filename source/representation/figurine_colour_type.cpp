#include "representation/figurine_colour_type.hpp"


#include <ostream>


namespace representation {


std::ostream & operator<<(
    std::ostream & out,
    figurine_colour_type const colour
) {
    return out.put(colour.as_char());
}


} // namespace representation
