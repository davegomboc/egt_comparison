#if !defined(EGT_COMPARISON_EPD_OPERATION_TYPE_HPP)
#define EGT_COMPARISON_EPD_OPERATION_TYPE_HPP


#include <optional>
#include <string>
#include <string_view>

#include "epd/opcode_type.hpp"
#include "epd/operands_type.hpp"


namespace epd {


class operation_type {
public:
    operation_type & operator=(operation_type const &) = default;
    operation_type & operator=(operation_type &&) = default;
    operation_type(operation_type const &) = default;
    operation_type(operation_type &&) = default;
    ~operation_type() = default;

    operation_type(opcode_type const &, operands_type const &);
    operation_type(opcode_type &&, operands_type &&);

    static std::optional<operation_type> create(
        std::string_view const operation);

    opcode_type const & opcode() const;
    operands_type const & operands() const;

    std::string as_epd() const;

private:
    opcode_type the_opcode;
    operands_type the_operands;
};


bool operator<(operation_type const & lhs, operation_type const & rhs);


std::ostream & operator<<(
 std::ostream & out, operation_type const & operation);


} // namespace epd


#endif // EGT_COMPARISON_EPD_OPERATION_TYPE_HPP
