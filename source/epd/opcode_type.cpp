#include "epd/opcode_type.hpp"


#include <cctype>
#include <ostream>


namespace epd {


std::optional<opcode_type> opcode_type::create(std::string_view const opcode) {
    if (opcode.empty() or (15 < opcode.size())) {
        return std::nullopt;
    }
    if (not std::isalpha(opcode[0])) {
        return std::nullopt;
    }
    for (std::size_t i{1}; i != opcode.size(); ++i) {
        if (not std::isalnum(opcode[i]) and ('_' != opcode[i])) {
            return std::nullopt;
        }
    }
    return opcode_type(opcode);
}


std::string opcode_type::as_epd() const {
    return the_opcode;
}


bool operator==(opcode_type const & lhs, opcode_type const & rhs) {
    return lhs.as_epd() == rhs.as_epd();
}


bool operator!=(opcode_type const & lhs, opcode_type const & rhs) {
    return lhs.as_epd() != rhs.as_epd();
}


bool operator<(opcode_type const & lhs, opcode_type const & rhs) {
    return lhs.as_epd() < rhs.as_epd();
}


bool operator>(opcode_type const & lhs, opcode_type const & rhs) {
    return lhs.as_epd() > rhs.as_epd();
}


bool operator<=(opcode_type const & lhs, opcode_type const & rhs) {
    return lhs.as_epd() <= rhs.as_epd();
}


bool operator>=(opcode_type const & lhs, opcode_type const & rhs) {
    return lhs.as_epd() >= rhs.as_epd();
}


std::ostream & operator<<(std::ostream & out, opcode_type const & opcode) {
    return out << opcode.as_epd();
}


} // namespace epd
