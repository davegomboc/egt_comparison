#if !defined(EGT_COMPARISON_EPD_OPERAND_TYPE_HPP)
#define EGT_COMPARISON_EPD_OPERAND_TYPE_HPP


#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>


namespace epd {


class operand_type {
public:
    operand_type & operator=(operand_type const &) = default;
    operand_type & operator=(operand_type &&) = default;
    operand_type(operand_type const &) = default;
    operand_type(operand_type &&) = default;
    ~operand_type() = default;

    static std::optional<operand_type> create(std::string_view operand);

    std::string as_epd() const;

private:
    operand_type(std::string_view const operand, bool const quoted)
      : the_operand(operand),
        the_operand_is_a_quoted_string(quoted) {}

    std::string the_operand;
    bool the_operand_is_a_quoted_string;
};


std::ostream & operator<<(std::ostream & out, operand_type const & operand);


} // namespace epd


#endif // EGT_COMPARISON_EPD_OPERAND_TYPE_HPP
