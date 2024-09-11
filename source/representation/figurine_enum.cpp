#include "representation/figurine_enum.hpp"


#include <algorithm>
#include <array>
#include <cassert>
#include <ostream>


namespace representation {


logic_digits_type as_logic_digits(figurine_enum const figurine) {
    assert(valid(figurine));
    logic_digits_type result;
    result.push_back(as_logic_digit(just_colour(figurine)));
    result.append(as_logic_digits(just_shape(figurine)));
    return result;
}


std::ostream & operator<<(std::ostream & out, figurine_enum figurine) {
    assert(valid(figurine));
    return out << as_char(figurine);
}


} // namespace representation
