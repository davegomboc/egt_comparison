#if !defined(EGT_COMPARISON_EPD_OPCODE_TYPE_HPP)
#define EGT_COMPARISON_EPD_OPCODE_TYPE_HPP


#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>


namespace epd {


class opcode_type {
public:
    opcode_type & operator=(opcode_type const &) = default;
    opcode_type & operator=(opcode_type &&) = default;
    opcode_type(opcode_type const &) = default;
    opcode_type(opcode_type &&) = default;
    ~opcode_type() = default;

    static std::optional<opcode_type> create(std::string_view const opcode);

    std::string as_epd() const;

private:
    opcode_type(std::string_view const opcode) : the_opcode(opcode) {}

    std::string the_opcode;
};


bool operator==(opcode_type const & lhs, opcode_type const & rhs);
bool operator!=(opcode_type const & lhs, opcode_type const & rhs);
bool operator<(opcode_type const & lhs, opcode_type const & rhs);
bool operator>(opcode_type const & lhs, opcode_type const & rhs);
bool operator<=(opcode_type const & lhs, opcode_type const & rhs);
bool operator>=(opcode_type const & lhs, opcode_type const & rhs);


std::ostream & operator<<(std::ostream & out, opcode_type const & opcode);


} // namespace epd


#endif // EGT_COMPARISON_EPD_OPCODE_TYPE_HPP
