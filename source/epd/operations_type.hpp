#if !defined(EGT_COMPARISON_EPD_OPERATIONS_TYPE_HPP)
#define EGT_COMPARISON_EPD_OPERATIONS_TYPE_HPP


#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


#include "epd/opcode_type.hpp"
#include "epd/operation_type.hpp"


namespace epd {


class operations_type {
public:
    operations_type & operator=(operations_type const &) = default;
    operations_type & operator=(operations_type &&) = default;
    operations_type(operations_type const &) = default;
    operations_type(operations_type &&) = default;
    ~operations_type() = default;

    operations_type(std::vector<operation_type> const &);
    operations_type(std::vector<operation_type> &&);

    operations_type() : the_operations{} {}

    static std::optional<operations_type> create(
        std::string_view const operations);

    static std::optional<operations_type> create_from_fen(
        std::string_view const hmvc_and_fmvn_operands);

    bool empty() const;
    std::size_t size() const;

    std::optional<operation_type> find_by_opcode_opt(
        opcode_type const & opcode) const;
    std::optional<operation_type> find_by_opcode_opt(
        std::optional<opcode_type> const & opcode_opt) const;

    std::string as_epd() const;
    std::optional<std::string> as_fen_opt() const;

private:
    std::optional<operand_type> first_operand_opt(
        opcode_type const opcode) const;

    std::vector<operation_type> the_operations;

    static opcode_type const the_hmvc_opcode;
    static opcode_type const the_fmvn_opcode;
};


std::ostream & operator<<(
    std::ostream & out,
    operations_type const & operations);


} // namespace epd


#endif // EGT_COMPARISON_EPD_OPERATIONS_TYPE_HPP
