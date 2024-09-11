#include "epd/operations_type.hpp"


#include <algorithm>
#include <cassert>
#include <ostream>


namespace epd {


operations_type::operations_type(
    std::vector<operation_type> const & operations
) : the_operations(operations) {
    std::sort(the_operations.begin(), the_operations.end());
}


operations_type::operations_type(std::vector<operation_type> && operations)
 : the_operations(std::move(operations)) {
    std::sort(the_operations.begin(), the_operations.end());
}


std::optional<operations_type>
operations_type::create(std::string_view operations) {
    if (operations.empty()) {
        return operations_type(std::vector<operation_type>());
    }
    if ((1 == operations.size()) or
     (';' != operations[operations.size() - 1])) {
        return std::nullopt;
    }
    std::vector<operation_type> parsed_operations;
    while (true) {
        std::size_t const first_semicolon{operations.find(';')};
        if (std::string_view::npos == first_semicolon) {
            if (operations.empty()) {
                return operations_type(parsed_operations);
            }
            return std::nullopt;
        }
        std::string_view const first_operation(
         operations.substr(0, 1 + first_semicolon));
        std::optional<operation_type> operation_opt{
         operation_type::create(first_operation)};
        if (not operation_opt) {
            return std::nullopt;
        }
        parsed_operations.push_back(*operation_opt);
        operations.remove_prefix(1 + first_semicolon);
        if (not operations.empty()) {
            operations.remove_prefix(1);
        }
    }
}


std::optional<operations_type>
operations_type::create_from_fen(
    std::string_view const hmvc_and_fmvn_operands
) {
    std::size_t const location_of_separating_space{
     hmvc_and_fmvn_operands.find(' ')};
    if (std::string_view::npos == location_of_separating_space) {
        return std::nullopt;
    }
    std::string_view const hmvc_operand_text{hmvc_and_fmvn_operands.substr(
     0, location_of_separating_space)};
    std::optional<operand_type> const hmvc_operand_opt{
     operand_type::create(hmvc_operand_text)};
    if (not hmvc_operand_opt) {
        return std::nullopt;
    }
    operands_type const hmvc_operands{
     operands_type(std::vector<operand_type>{*hmvc_operand_opt})};
    operation_type const hmvc_operation{
     operation_type(the_hmvc_opcode, hmvc_operands)};

    std::string_view const fmvn_operand_text{hmvc_and_fmvn_operands.substr(
     location_of_separating_space + 1)};
    std::optional<operand_type> const fmvn_operand_opt{
     operand_type::create(fmvn_operand_text)};
    if (not fmvn_operand_opt) {
        return std::nullopt;
    }
    operands_type const fmvn_operands{
     operands_type(std::vector<operand_type>{*fmvn_operand_opt})};
    operation_type const fmvn_operation{
     operation_type(the_fmvn_opcode, fmvn_operands)};

    return operations_type({fmvn_operation, hmvc_operation});
}


bool operations_type::empty() const {
    return the_operations.empty();
}


std::size_t operations_type::size() const {
    return the_operations.size();
}


std::optional<operation_type>
operations_type::find_by_opcode_opt(opcode_type const & opcode) const {
    for (operation_type const & operation: the_operations) {
        if (opcode == operation.opcode()) {
            return operation;
        }
    }
    return std::nullopt;
}


std::optional<operation_type>
operations_type::find_by_opcode_opt(
    std::optional<opcode_type> const & opcode_opt
) const {
    if (std::nullopt == opcode_opt) {
        return std::nullopt;
    }
    return find_by_opcode_opt(*opcode_opt);
}


std::string operations_type::as_epd() const {
    std::string result;
    std::for_each(the_operations.cbegin(), the_operations.cend(),
     [&](operation_type const & operation) {
        result += operation.as_epd();
	result += ' ';
    });
    if (the_operations.cbegin() != the_operations.end()) {
        result.pop_back();
    }
    return result;
}


std::optional<std::string> operations_type::as_fen_opt() const {
    std::optional<operand_type> hmvc_operand_opt{
     first_operand_opt(the_hmvc_opcode)};
    std::optional<operand_type> fmvn_operand_opt{
     first_operand_opt(the_fmvn_opcode)};
    if (not hmvc_operand_opt or not fmvn_operand_opt) {
        return std::nullopt;
    }
    std::string result{hmvc_operand_opt->as_epd()};
    result += ' ';
    result += fmvn_operand_opt->as_epd();
    return result;
}


std::optional<operand_type>
operations_type::first_operand_opt(opcode_type const opcode) const {
    std::optional<operation_type> const operation_opt{
     find_by_opcode_opt(opcode)};
    if (not operation_opt) {
        return std::nullopt;
    }
    operands_type const & operands{operation_opt->operands()};
    if (operands.empty()) {
        return std::nullopt;
    }
    return operands[0];
}


opcode_type const operations_type::the_hmvc_opcode{
 *opcode_type::create("hmvc")};
opcode_type const operations_type::the_fmvn_opcode{
 *opcode_type::create("fmvn")};


std::ostream & operator<<(
    std::ostream & out,
    operations_type const & operations
) {
    return out << operations.as_epd();
}


} // namespace epd
