#include "epd/operation_type.hpp"


#include <ostream>


namespace epd {


operation_type::operation_type(
    opcode_type const & opcode,
    operands_type const & operands
) : the_opcode(opcode), the_operands(operands) {}


operation_type::operation_type(
    opcode_type && opcode,
    operands_type && operands
) : the_opcode(std::move(opcode)), the_operands(std::move(operands)) {}


std::optional<operation_type>
operation_type::create(std::string_view operation) {
    if (2 > operation.size()) {
        return std::nullopt;
    }
    if (';' != operation[operation.size() - 1]) {
        return std::nullopt;
    }
    operation.remove_suffix(1);
    std::size_t const first_space{operation.find(' ')};
    if (std::string_view::npos == first_space) {
        // There are no operands.
        std::optional<opcode_type> opcode_opt{opcode_type::create(operation)};
        if (not opcode_opt) {
            return std::nullopt;
        }
        return operation_type(*opcode_opt, operands_type({}));
    }

    // There is at least one operand.
    std::string_view const opcode{operation.substr(0, first_space)};
    std::optional<opcode_type> opcode_opt{opcode_type::create(opcode)};
    if (not opcode_opt) {
        return std::nullopt;
    }
    std::string_view const operands{operation.substr(1 + first_space)};
    std::optional<operands_type> operands_opt{operands_type::create(operands)};
    if (not operands_opt) {
        return std::nullopt;
    }
    return operation_type(*opcode_opt, *operands_opt);
}


opcode_type const & operation_type::opcode() const {
    return the_opcode;
}


operands_type const & operation_type::operands() const {
    return the_operands;
}


std::string operation_type::as_epd() const {
    std::string result;
    result += the_opcode.as_epd();
    result += the_operands.as_epd();
    result += ';';
    return result;
}


bool operator<(operation_type const & lhs, operation_type const & rhs) {
    return lhs.opcode() < rhs.opcode();
}


std::ostream & operator<<(
    std::ostream & out,
    operation_type const & operation
) {
    return out << operation.as_epd();
}


} // namespace epd
