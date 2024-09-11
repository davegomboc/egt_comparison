#include "representation/logic_digit_type.hpp"


#include <ostream>


namespace representation {


std::ostream & operator<<(
    std::ostream & out,
    logic_digit_type const logic_digit
) {
    return out << logic_digit.as_char();
}


} // namespace representation
