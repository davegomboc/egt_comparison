#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_TYPE_HPP


#include <bitset>
#include <functional>
#include <iosfwd>

#include "representation/figurine_colour_type.hpp"
#include "representation/figurine_enum.hpp"
#include "representation/figurine_shape_type.hpp"


namespace representation {


class figurine_type {
public:
    figurine_type & operator=(figurine_type const &) = default;
    figurine_type & operator=(figurine_type &&) = default;
    constexpr figurine_type(figurine_type const &) = default;
    constexpr figurine_type(figurine_type &&) = default;
    ~figurine_type() = default;

    constexpr explicit figurine_type(figurine_enum const figurine)
      : the_figurine(figurine) {
        assert(valid(figurine));
    }
    constexpr explicit figurine_type(int const figurine)
     : the_figurine(static_cast<figurine_enum>(figurine)) {
        assert(0 <= figurine);
        assert(figurine < 16);
        assert(valid(static_cast<figurine_enum>(figurine)));
    }
    constexpr figurine_type(
        figurine_colour_type const colour,
        figurine_shape_type const shape
    ) : the_figurine(colour.as_enum() | shape.as_enum()) {}

    static std::optional<figurine_type> create_opt(
        std::optional<figurine_enum> enum_opt);
    static std::optional<figurine_type> create_opt(char ch);

    constexpr figurine_enum as_enum() const { return the_figurine; }

    constexpr unsigned char as_index() const {
        return static_cast<unsigned char>(as_enum());
    }

    constexpr char as_char() const {
        return representation::as_char(the_figurine);
    }

    constexpr std::string_view as_standalone_utf8() const {
        return representation::as_standalone_utf8(the_figurine);
    }

    inline std::string as_board_diagram_square(
        bool const light_square,
        bool const use_utf8
    ) const {
        return representation::as_board_diagram_square(the_figurine,
         light_square, use_utf8);
    }

    logic_digits_type as_logic_digits() const;

    constexpr figurine_colour_type colour() const {
        return figurine_colour_type(just_colour(the_figurine));
    }

    constexpr figurine_shape_type shape() const {
        return figurine_shape_type(just_shape(the_figurine));
    }

    void flip_colour();

private:
    figurine_enum the_figurine;
};


constexpr bool operator==(figurine_type const lhs, figurine_type const rhs) {
    return lhs.as_enum() == rhs.as_enum();
}


constexpr bool operator!=(figurine_type const lhs, figurine_type const rhs) {
    return not (lhs == rhs);
}


constexpr bool operator<(figurine_type const lhs, figurine_type const rhs) {
    return lhs.as_enum() < rhs.as_enum();
}


constexpr bool operator>(figurine_type const lhs, figurine_type const rhs) {
    return rhs < lhs;
}


constexpr bool operator<=(figurine_type const lhs, figurine_type const rhs) {
    return not (rhs < lhs);
}


constexpr bool operator>=(figurine_type const lhs, figurine_type const rhs) {
    return not (lhs < rhs);
}


std::ostream & operator<<(std::ostream & out, figurine_type figurine);


constexpr figurine_type black_king{figurine_enum::black_king};
constexpr figurine_type black_queen{figurine_enum::black_queen};
constexpr figurine_type black_rook{figurine_enum::black_rook};
constexpr figurine_type black_bishop{figurine_enum::black_bishop};
constexpr figurine_type black_knight{figurine_enum::black_knight};
constexpr figurine_type black_pawn{figurine_enum::black_pawn};
constexpr figurine_type white_king{figurine_enum::white_king};
constexpr figurine_type white_queen{figurine_enum::white_queen};
constexpr figurine_type white_rook{figurine_enum::white_rook};
constexpr figurine_type white_bishop{figurine_enum::white_bishop};
constexpr figurine_type white_knight{figurine_enum::white_knight};
constexpr figurine_type white_pawn{figurine_enum::white_pawn};


constexpr std::optional<figurine_enum> as_optional_enum(
    std::optional<figurine_type> const & figurine_opt
) {
    return figurine_opt ? std::make_optional(figurine_opt->as_enum()) :
     std::nullopt;
}


} // namespace representation


namespace std {


template <> class hash<representation::figurine_type> {
public:
    std::size_t operator()(representation::figurine_type const & figurine)
     const {
        return hash<representation::figurine_enum>()(figurine.as_enum());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_TYPE_HPP
