#include "representation/figurine_shape_type.hpp"


#include <ostream>


namespace representation {


std::ostream & operator<<(std::ostream & out, figurine_shape_type const shape) {
    return out.put(shape.as_char());
}


} // namespace representation
