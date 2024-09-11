#include "representation/coordinate_type.hpp"


#include <ostream>


namespace representation {


std::optional<coordinate_type>
coordinate_type::create_opt(std::string_view const coordinate) {
    if (2 != coordinate.size()) {
        return std::nullopt;
    }
    file_enum file;
    switch (coordinate[0]) {
    case 'a':
        file = file_enum::a_file;
        break;
    case 'b':
        file = file_enum::b_file;
        break;
    case 'c':
        file = file_enum::c_file;
        break;
    case 'd':
        file = file_enum::d_file;
        break;
    case 'e':
        file = file_enum::e_file;
        break;
    case 'f':
        file = file_enum::f_file;
        break;
    case 'g':
        file = file_enum::g_file;
        break;
    case 'h':
        file = file_enum::h_file;
        break;
    default:
        return std::nullopt;
    }
    rank_enum rank;
    switch (coordinate[1]) {
    case '8':
        rank = rank_enum::rank_8;
        break;
    case '7':
        rank = rank_enum::rank_7;
        break;
    case '6':
        rank = rank_enum::rank_6;
        break;
    case '5':
        rank = rank_enum::rank_5;
        break;
    case '4':
        rank = rank_enum::rank_4;
        break;
    case '3':
        rank = rank_enum::rank_3;
        break;
    case '2':
        rank = rank_enum::rank_2;
        break;
    case '1':
        rank = rank_enum::rank_1;
        break;
    default:
        return std::nullopt;
    }
    return coordinate_type(file, rank);
}


bool coordinate_type::next() {
    return representation::next(the_coordinate);
}


bool coordinate_type::prev() {
    return representation::prev(the_coordinate);
}


coordinate_type & coordinate_type::operator++() {
    ++the_coordinate;
    return *this;
}


coordinate_type & coordinate_type::operator--() {
    --the_coordinate;
    return *this;
}


logic_digits_type coordinate_type::as_logic_digits() const {
    return representation::as_logic_digits(the_coordinate);
}


std::string coordinate_type::as_string() const {
    return representation::as_string(the_coordinate);
}


void coordinate_type::flip_file() {
    the_coordinate = flipped_file(the_coordinate);
}


void coordinate_type::flip_rank() {
    the_coordinate = flipped_rank(the_coordinate);
}


void coordinate_type::flip_a8h1() {
    the_coordinate = flipped_a8h1(the_coordinate);
}


std::ostream & operator<<(std::ostream & out, coordinate_type coordinate) {
    return out << coordinate.as_string();
}


template coordinate_type::coordinate_type(std::vector<bool> &);
template coordinate_type::coordinate_type(std::vector<unsigned char> &);
template coordinate_type::coordinate_type(std::vector<unsigned short> &);
template coordinate_type::coordinate_type(std::vector<unsigned int> &);
template coordinate_type::coordinate_type(std::vector<unsigned long> &);
template coordinate_type::coordinate_type(std::vector<unsigned long long> &);

template void coordinate_type::append(std::vector<bool> &) const;
template void coordinate_type::append(std::vector<unsigned char> &) const;
template void coordinate_type::append(std::vector<unsigned short> &) const;
template void coordinate_type::append(std::vector<unsigned int> &) const;
template void coordinate_type::append(std::vector<unsigned long> &) const;
template void coordinate_type::append(std::vector<unsigned long long> &) const;


} // namespace representation
