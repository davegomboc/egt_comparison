#include "representation/logic_digits_type.hpp"


#include <algorithm>
#include <ostream>


namespace representation {


std::string logic_digits_type::to_string() const {
    std::string result;
    result.reserve(the_logic_digits.size());
    std::for_each(the_logic_digits.cbegin(), the_logic_digits.cend(),
     [&](logic_digit_type logic_digit) {
        result += logic_digit.as_char();
    });
    return result;
}

std::ostream & operator<<(std::ostream & out, logic_digits_type logic_digits) {
    std::size_t const grouping_size{6};
    std::string spaced_digits{logic_digits.to_string()};
    std::size_t space_insertion_point{spaced_digits.size()};
    while (grouping_size <= space_insertion_point) {
        space_insertion_point -= grouping_size;
        spaced_digits.insert(space_insertion_point, 1, ' ');
    }
    return out << spaced_digits;
}


} // namespace representation
