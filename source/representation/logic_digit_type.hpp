#if !defined(EGT_COMPARISON_REPRESENTATION_LOGIC_DIGIT_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_LOGIC_DIGIT_TYPE_HPP


#include <cassert>
#include <iosfwd>

#include "logic_digit_enum.hpp"


namespace representation {


class logic_digit_type {
public:
    logic_digit_type & operator=(logic_digit_type const &) = default;
    logic_digit_type & operator=(logic_digit_type &&) = default;
    constexpr logic_digit_type(logic_digit_type const &) = default;
    constexpr logic_digit_type(logic_digit_type &&) = default;
    constexpr logic_digit_type()
     : the_logic_digit(logic_digit_enum::singular) {}
    ~logic_digit_type() = default;

    static constexpr std::optional<logic_digit_type> create_opt(char const ch) {
        std::optional<logic_digit_enum> const logic_digit_enum_opt{
         as_logic_digit_enum_opt(ch)};
        return logic_digit_enum_opt ? std::make_optional<logic_digit_type>(
         *logic_digit_enum_opt) : std::nullopt;
    }

    constexpr explicit logic_digit_type(logic_digit_enum const logic_digit)
     : the_logic_digit(logic_digit) {
        assert(valid(logic_digit));
    }
    constexpr explicit logic_digit_type(bool const value)
     : the_logic_digit(
     value ? logic_digit_enum::set : logic_digit_enum::reset) {}

    constexpr bool singular() const {
        return ::singular(the_logic_digit);
    }

    constexpr logic_digit_enum as_logic_digit_enum() const {
        return the_logic_digit;
    }

    constexpr char as_char() const {
        return ::as_char(the_logic_digit);
    }

    void flip() {
        the_logic_digit = flipped(the_logic_digit);
    }

private:
    logic_digit_enum the_logic_digit;
};


constexpr logic_digit_type sng{logic_digit_enum::singular};
constexpr logic_digit_type off{logic_digit_enum::reset};
constexpr logic_digit_type on{logic_digit_enum::set};
constexpr logic_digit_type dc{logic_digit_enum::do_not_care};


constexpr bool operator==(
    logic_digit_type const lhs,
    logic_digit_type const rhs
) {
    return lhs.as_logic_digit_enum() == rhs.as_logic_digit_enum();
}

constexpr bool operator!=(
    logic_digit_type const lhs,
    logic_digit_type const rhs
) {
    return not operator==(lhs, rhs);
}


std::ostream & operator<<(std::ostream & out, logic_digit_type logic_digit);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_LOGIC_DIGIT_TYPE_HPP
