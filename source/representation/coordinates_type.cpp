#include "representation/coordinates_type.hpp"


#include <ostream>


namespace representation {


coordinates_type::coordinates_type(
    std::size_t const figurine_count,
    std::size_t const index
) : the_coordinates(figurine_count) {
    assert(figurine_count < 10);
    for (unsigned nth_place{}; nth_place != figurine_count; ++nth_place) {
        unsigned char const base_64_digit{static_cast<unsigned char>(
         (index >> (6 * nth_place)) & 0b111111)};
        the_coordinates[figurine_count - nth_place - 1] =
         coordinate_type{base_64_digit};
    }
}


void coordinates_type::push_back(coordinate_type const & coordinate) {
    the_coordinates.push_back(coordinate);
    assert(the_coordinates.size() < 10);
}


void coordinates_type::push_back(coordinate_type && coordinate) {
    the_coordinates.push_back(std::move(coordinate));
    assert(the_coordinates.size() < 10);
}


std::string coordinates_type::as_string() const {
    std::string result{'['};
    for (std::size_t place{}; place != the_coordinates.size(); ++place) {
        if (0 != place) {
            result += ", ";
        }
        result += the_coordinates[place].as_string();
    }
    result += ']';
    return result;
}


// Returns false iff the increment wrapped around from the maximum back to zero.
bool coordinates_type::next() {
    auto current_digit_iter{the_coordinates.end()};
    while (the_coordinates.begin() != current_digit_iter) {
        --current_digit_iter;
        if (representation::coordinate_enum::square_mask !=
         *current_digit_iter) {
            ++(*current_digit_iter);
            return true;
        }
        *current_digit_iter = representation::coordinate_enum{};
    }
    return false;
}


coordinates_type & coordinates_type::operator++() {
    next();
    return *this;
}


bool operator==(
    coordinates_type const & lhs,
    coordinates_type const & rhs
) {
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}


bool operator!=(
    coordinates_type const & lhs,
    coordinates_type const & rhs
) {
    return not (lhs == rhs);
}


bool operator<(
    coordinates_type const & lhs,
    coordinates_type const & rhs
) {
    if (lhs.size() != rhs.size()) {
        return lhs.size() < rhs.size();
    }
    return std::lexicographical_compare(
     lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}


bool operator>(
    coordinates_type const & lhs,
    coordinates_type const & rhs
) {
    return rhs < lhs;
}


bool operator<=(
    coordinates_type const & lhs,
    coordinates_type const & rhs
) {
    return not (rhs < lhs);
}


bool operator>=(
    coordinates_type const & lhs,
    coordinates_type const & rhs
) {
    return not (lhs < rhs);
}


std::ostream & operator<<(
    std::ostream & out,
    coordinates_type const & coordinates
) {
    return out << coordinates.as_string();
}


} // namespace representation
