#if !defined(EGT_COMPARISON_EPD_OPERANDS_TYPE_HPP)
#define EGT_COMPARISON_EPD_OPERANDS_TYPE_HPP


#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "epd/operand_type.hpp"


namespace epd {


class operands_type {
public:
    operands_type & operator=(operands_type const &) = default;
    operands_type & operator=(operands_type &&) = default;
    operands_type(operands_type const &) = default;
    operands_type(operands_type &&) = default;
    ~operands_type() = default;

    operands_type(std::vector<operand_type> const &);
    operands_type(std::vector<operand_type> &&);

    static std::optional<operands_type> create(std::string_view const operands);

    bool empty() const;
    std::size_t size() const;

    operand_type const & operator[](std::size_t index) const;

    std::string as_epd() const;

private:
    std::vector<operand_type> the_operands;
};


} // namespace epd


#endif // EGT_COMPARISON_EPD_OPERANDS_TYPE_HPP
