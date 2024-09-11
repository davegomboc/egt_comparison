#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPE_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPE_TYPE_HPP


#include <cassert>
#include <functional>

#include "representation/figurine_shape_enum.hpp"


namespace representation {


class figurine_shape_type {
public:
    figurine_shape_type & operator=(figurine_shape_type const &) = default;
    figurine_shape_type & operator=(figurine_shape_type &&) = default;
    constexpr figurine_shape_type(figurine_shape_type const &) = default;
    constexpr figurine_shape_type(figurine_shape_type &&) = default;
    ~figurine_shape_type() = default;

    constexpr explicit figurine_shape_type(figurine_shape_enum const shape)
      : the_shape(shape) {
        assert(valid(shape));
    }
    constexpr explicit figurine_shape_type(int const shape)
      : the_shape(static_cast<figurine_shape_enum>(shape)) {
        assert(0 <= shape);
        assert(shape < 8);
        assert(valid(static_cast<figurine_shape_enum>(shape)));
    }

    constexpr figurine_shape_enum as_enum() const { return the_shape; }

    constexpr unsigned char as_index() const {
        return static_cast<unsigned char>(as_enum());
    }

    constexpr char as_char() const {
        return representation::as_char(the_shape);
    }

    logic_digits_type as_logic_digits() const {
        return representation::as_logic_digits(the_shape);
    }

private:
    figurine_shape_enum the_shape;
};


constexpr bool operator==(
    figurine_shape_type const lhs,
    figurine_shape_type const rhs
) {
    return lhs.as_enum() == rhs.as_enum();
}


constexpr bool operator!=(
    figurine_shape_type const lhs,
    figurine_shape_type const rhs
) {
    return not (lhs == rhs);
}


constexpr bool operator<(
    figurine_shape_type const lhs,
    figurine_shape_type const rhs
) {
    return lhs.as_enum() < rhs.as_enum();
}


constexpr bool operator>(
    figurine_shape_type const lhs,
    figurine_shape_type const rhs
) {
    return rhs < lhs;
}


constexpr bool operator<=(
    figurine_shape_type const lhs,
    figurine_shape_type const rhs
) {
    return not (rhs < lhs);
}


constexpr bool operator>=(
    figurine_shape_type const lhs,
    figurine_shape_type const rhs
) {
    return not (lhs < rhs);
}


std::ostream & operator<<(std::ostream & out, figurine_shape_type shape);


constexpr figurine_shape_type king{figurine_shape_enum::king};
constexpr figurine_shape_type queen{figurine_shape_enum::queen};
constexpr figurine_shape_type rook{figurine_shape_enum::rook};
constexpr figurine_shape_type bishop{figurine_shape_enum::bishop};
constexpr figurine_shape_type knight{figurine_shape_enum::knight};
constexpr figurine_shape_type pawn{figurine_shape_enum::pawn};


} // namespace representation


namespace std {


template <> class hash<representation::figurine_shape_type> {
public:
    std::size_t operator()(representation::figurine_shape_type const & figurine_shape)
     const {
        return hash<representation::figurine_shape_enum>()(figurine_shape.as_enum());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPE_TYPE
