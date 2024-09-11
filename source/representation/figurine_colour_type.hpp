#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_COLOUR_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_COLOUR_TYPE_HPP


#include <cassert>
#include <functional>
#include <string_view>

#include "epd/side_to_move_type.hpp"

#include "representation/figurine_colour_enum.hpp"
#include "representation/logic_digit_type.hpp"


namespace representation {


class figurine_colour_type {
public:
    figurine_colour_type & operator=(figurine_colour_type const &) = default;
    figurine_colour_type & operator=(figurine_colour_type &&) = default;
    constexpr figurine_colour_type(figurine_colour_type const &) = default;
    constexpr figurine_colour_type(figurine_colour_type &&) = default;
    ~figurine_colour_type() = default;

    constexpr explicit figurine_colour_type(figurine_colour_enum const colour)
      : the_colour(colour) {
        assert(valid(colour));
    }
    constexpr explicit figurine_colour_type(bool const black_to_move)
     : the_colour(black_to_move ? figurine_colour_enum::black :
        figurine_colour_enum::white) {}
    constexpr explicit figurine_colour_type(int const colour)
      : the_colour(static_cast<figurine_colour_enum>(colour)) {
        assert(0 <= colour);
        assert(colour < 9);
        assert(valid(static_cast<figurine_colour_enum>(colour)));
    }
    explicit figurine_colour_type(epd::side_to_move_type const & side_to_move)
      : the_colour(side_to_move.black_is_to_move() ?
         figurine_colour_enum::black : figurine_colour_enum::white) {
    }

    constexpr figurine_colour_enum as_enum() const { return the_colour; }

    constexpr unsigned char as_index() const {
        return static_cast<unsigned char>(as_enum());
    }

    constexpr bool as_bool() const {
        return representation::as_bool(the_colour);
    }

    constexpr char as_char() const {
        return representation::as_char(the_colour);
    }

    constexpr std::string_view as_description() const {
        return representation::as_description(the_colour);
    }

    constexpr logic_digit_type as_logic_digit() const {
        return representation::as_logic_digit(the_colour);
    }

    epd::side_to_move_type as_side_to_move() const {
        return representation::as_side_to_move(the_colour);
    }

    void flip() {
        representation::flip(the_colour);
    }

private:
    figurine_colour_enum the_colour;
};


constexpr bool operator==(
    figurine_colour_type const lhs,
    figurine_colour_type const rhs
) {
    return lhs.as_enum() == rhs.as_enum();
}


constexpr bool operator!=(
    figurine_colour_type const lhs,
    figurine_colour_type const rhs
) {
    return not (lhs == rhs);
}


constexpr bool operator<(
    figurine_colour_type const lhs,
    figurine_colour_type const rhs
) {
    return lhs.as_enum() < rhs.as_enum();
}


constexpr bool operator>(
    figurine_colour_type const lhs,
    figurine_colour_type const rhs
) {
    return rhs < lhs;
}


constexpr bool operator<=(
    figurine_colour_type const lhs,
    figurine_colour_type const rhs
) {
    return not (rhs < lhs);
}


constexpr bool operator>=(
    figurine_colour_type const lhs,
    figurine_colour_type const rhs
) {
    return not (lhs < rhs);
}


std::ostream & operator<<(std::ostream & out, figurine_colour_type colour);


constexpr figurine_colour_type white{figurine_colour_enum::white};
constexpr figurine_colour_type black{figurine_colour_enum::black};


} // namespace representation


namespace std {


template <> class hash<representation::figurine_colour_type> {
public:
    std::size_t operator()(representation::figurine_colour_type const & figurine_colour)
     const {
        return hash<representation::figurine_colour_enum>()(figurine_colour.as_enum());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_COLOUR_TYPE_HPP
