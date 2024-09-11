#if !defined(EGT_COMPARISON_EPD_CASTLING_RIGHTS_TYPE_HPP)
#define EGT_COMPARISON_EPD_CASTLING_RIGHTS_TYPE_HPP


#include <cstdint>
#include <optional>
#include <string>
#include <string_view>


namespace epd {


class castling_rights_type {
public:
    std::uint8_t static constexpr white_OO{0b0001};
    std::uint8_t static constexpr white_OOO{0b0010};
    std::uint8_t static constexpr black_OO{0b0100};
    std::uint8_t static constexpr black_OOO{0b1000};

    castling_rights_type & operator=(castling_rights_type const &) = default;
    castling_rights_type & operator=(castling_rights_type &&) = default;
    constexpr castling_rights_type(castling_rights_type const &) = default;
    constexpr castling_rights_type(castling_rights_type &&) = default;
    ~castling_rights_type() = default;

    constexpr castling_rights_type() : the_castling_rights(UINT8_C(0)) {}

    constexpr castling_rights_type(
        bool const white_kingside_allowed,
        bool const white_queenside_allowed,
        bool const black_kingside_allowed,
        bool const black_queenside_allowed
    ) : the_castling_rights{} {
        if (white_kingside_allowed) {
            the_castling_rights |= white_OO;
        }
        if (white_queenside_allowed) {
            the_castling_rights |= white_OOO;
        }
        if (black_kingside_allowed) {
            the_castling_rights |= black_OO;
        }
        if (black_queenside_allowed) {
            the_castling_rights |= black_OOO;
        }
    }

    static constexpr castling_rights_type prohibit_all() {
        return castling_rights_type(UINT8_C(0));
    }

    static constexpr castling_rights_type allow_all() {
        return castling_rights_type(
         white_OO | white_OOO | black_OO | black_OOO);
    }

    static std::optional<castling_rights_type> create(
        std::uint8_t const castling_rights);
    static std::optional<castling_rights_type> create(
        std::string_view const castling_rights);

    constexpr bool black_kingside() const {
        return the_castling_rights & black_OO;
    }
    constexpr bool black_queenside() const {
        return the_castling_rights & black_OOO;
    }
    constexpr bool white_kingside() const {
        return the_castling_rights & white_OO;
    }
    constexpr bool white_queenside() const {
        return the_castling_rights & white_OOO;
    }

    constexpr bool any() const {
        return not none();
    }
    constexpr bool all() const {
        return (white_OO | white_OOO | black_OO | black_OOO)
         == the_castling_rights;
    }
    constexpr bool none() const {
        return 0 == the_castling_rights;
    }

    constexpr std::uint8_t as_uint8_t() const {
        return the_castling_rights;
    }

    std::string as_epd_fen() const;

    void black_kingside(bool allowed);
    void black_queenside(bool allowed);
    void white_kingside(bool allowed);
    void white_queenside(bool allowed);

    void flip_colours();

private:
    constexpr castling_rights_type(std::uint8_t const castling_rights)
      : the_castling_rights(castling_rights) {}

    std::uint8_t the_castling_rights;
};


constexpr bool operator==(
    castling_rights_type const lhs,
    castling_rights_type const rhs
) {
    return lhs.as_uint8_t() == rhs.as_uint8_t();
}


constexpr bool operator!=(
    castling_rights_type const lhs,
    castling_rights_type const rhs
) {
    return lhs.as_uint8_t() != rhs.as_uint8_t();
}


constexpr bool operator<(
    castling_rights_type const lhs,
    castling_rights_type const rhs
) {
    return lhs.as_uint8_t() < rhs.as_uint8_t();
}


constexpr bool operator>(
    castling_rights_type const lhs,
    castling_rights_type const rhs
) {
    return lhs.as_uint8_t() > rhs.as_uint8_t();
}


constexpr bool operator<=(
    castling_rights_type const lhs,
    castling_rights_type const rhs
) {
    return lhs.as_uint8_t() <= rhs.as_uint8_t();
}


constexpr bool operator>=(
    castling_rights_type const lhs,
    castling_rights_type const rhs
) {
    return lhs.as_uint8_t() >= rhs.as_uint8_t();
}


std::ostream & operator<<(
    std::ostream & out,
    castling_rights_type const & castling_rights);


} // namespace epd


#endif // EGT_COMPARISON_EPD_CASTLING_RIGHTS_TYPE_HPP
