#include "epd/operand_type.hpp"


#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ostream>


namespace epd {


std::optional<operand_type>
operand_type::create(std::string_view operand) {
    if (operand.size() >= 2) {
        if (('"' == operand.front()) and ('"' == operand.back())) {
            if (operand.size() > 256) {
                return std::nullopt;
            }
            operand.remove_prefix(1);
            operand.remove_suffix(1);
            for (auto iter{operand.cbegin()}; iter != operand.cend(); ++iter) {
                if (not std::isprint(*iter) or ('"' == *iter)) {
                    return std::nullopt;
                }
            }
            return operand_type(operand, true);
        }
    }
    for (auto iter{operand.cbegin()}; iter != operand.cend(); ++iter) {
        if (not std::isprint(*iter) or std::isspace(*iter)) {
            return std::nullopt;
        }
    }
    return operand_type(operand, false);
}


std::string operand_type::as_epd() const {
    std::string result;
    if (the_operand_is_a_quoted_string) {
        result += '"';
        result += the_operand;
        result += '"';
    } else {
        result = the_operand;
    }
    return result;
}


std::ostream & operator<<(std::ostream & out, operand_type const & operand) {
    return out << operand.as_epd();
}


} // namespace epd
