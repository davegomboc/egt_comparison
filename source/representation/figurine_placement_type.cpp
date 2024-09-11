#include "representation/figurine_placement_type.hpp"


#include <ostream>

#include "representation/logic_digits_type.hpp"


namespace representation {


std::string figurine_placement_type::as_string() const {
    std::string result{figurine().as_char()};
    result.push_back('@');
    result += coordinate().as_string();
    result += " (";
    result += coordinate().as_logic_digits().to_string();
    result += ")";
    return result;
}


logic_digits_type figurine_placement_type::as_logic_digits() const {
    // Each material signature is in its own table, so we only need to record
    // the coordinate.
    return coordinate().as_logic_digits();
}


void figurine_placement_type::flip_a8h1() {
    the_coordinate.flip_a8h1();
}


void figurine_placement_type::flip_colour() {
    the_figurine.flip_colour();
}


void figurine_placement_type::flip_file() {
    the_coordinate.flip_file();
}


void figurine_placement_type::flip_rank() {
    the_coordinate.flip_rank();
}


std::ostream & operator<<(
    std::ostream & out,
    figurine_placement_type figurine_placement
) {
    return out << figurine_placement.as_string();
}


template figurine_placement_type::figurine_placement_type(
    figurine_type figurine, std::vector<bool> &);
template figurine_placement_type::figurine_placement_type(
    figurine_type figurine, std::vector<unsigned char> &);
template figurine_placement_type::figurine_placement_type(
    figurine_type figurine, std::vector<unsigned short> &);
template figurine_placement_type::figurine_placement_type(
    figurine_type figurine, std::vector<unsigned int> &);
template figurine_placement_type::figurine_placement_type(
    figurine_type figurine, std::vector<unsigned long> &);
template figurine_placement_type::figurine_placement_type(
    figurine_type figurine, std::vector<unsigned long long> &);

template void figurine_placement_type::append(std::vector<bool> &) const;
template void figurine_placement_type::append(
    std::vector<unsigned char> &) const;
template void figurine_placement_type::append(
    std::vector<unsigned short> &) const;
template void figurine_placement_type::append(
    std::vector<unsigned int> &) const;
template void figurine_placement_type::append(
    std::vector<unsigned long> &) const;
template void figurine_placement_type::append(
    std::vector<unsigned long long> &) const;


} // namespace representation
