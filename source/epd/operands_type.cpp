#include "epd/operands_type.hpp"


#include <algorithm>
#include <ostream>


namespace epd {


operands_type::operands_type(std::vector<operand_type> const & operands)
 : the_operands(operands) {}


operands_type::operands_type(std::vector<operand_type> && operands)
 : the_operands(std::move(operands)) {}


std::optional<operands_type>
operands_type::create(std::string_view const operands) {
    // TODO: Support the presence of less or more than a single operand.
    std::optional<operand_type> operand_opt{operand_type::create(operands)};
    if (not operand_opt) {
        return std::nullopt;
    }
    return operands_type(std::vector<operand_type>{*operand_opt});
}


bool operands_type::empty() const {
    return the_operands.empty();
}


std::size_t operands_type::size() const {
    return the_operands.size();
}


operand_type const & operands_type::operator[](std::size_t const index) const {
    return the_operands[index];
}


std::string operands_type::as_epd() const {
    std::string result;
    std::for_each(the_operands.cbegin(), the_operands.cend(),
     [&](operand_type const & operand) {
         result += ' ';
         result += operand.as_epd();
    });
    return result;
}


std::ostream & operator<<(std::ostream & out, operands_type const & operands) {
    return out << operands.as_epd();
}


} // namespace epd
