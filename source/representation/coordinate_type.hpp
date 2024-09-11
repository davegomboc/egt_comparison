#if !defined(EGT_COMPARISON_REPRESENTATION_COORDINATE_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_COORDINATE_TYPE_HPP


#include <cstddef>
#include <functional>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "epd/en_passant_target_type.hpp"

#include "representation/coordinate_enum.hpp"
#include "representation/logic_digits_type.hpp"


namespace representation {


class coordinate_type {
public:
    coordinate_type & operator=(coordinate_type const &) = default;
    coordinate_type & operator=(coordinate_type &&) = default;
    constexpr coordinate_type(coordinate_type const &) = default;
    constexpr coordinate_type(coordinate_type &&) = default;
    ~coordinate_type() = default;

    constexpr coordinate_type() : the_coordinate{} {}
    constexpr coordinate_type(coordinate_enum const coordinate)
     : the_coordinate(coordinate) {
        assert(valid(coordinate));
    }
    constexpr explicit coordinate_type(unsigned char const coordinate)
     : the_coordinate(static_cast<coordinate_enum>(coordinate)) {
        assert(coordinate < 64);
    }
    constexpr coordinate_type(file_enum const file, rank_enum const rank)
     : coordinate_type(static_cast<unsigned char>(file) |
     static_cast<unsigned char>(rank)) {}
    explicit coordinate_type(
        epd::en_passant_target_type const en_passant_target
    ) : the_coordinate(file_enum::a_file | rank_enum::rank_8) {
        std::optional<coordinate_type> coordinate_opt{
         coordinate_type::create_opt(en_passant_target.as_epd_fen())};
        if (coordinate_opt) {
            the_coordinate = coordinate_opt->as_enum();
        }
    }

    explicit coordinate_type(std::size_t & position_id)
      : the_coordinate{remove_coordinate(position_id)} {}

    template <typename uint_type>
    explicit coordinate_type(std::vector<uint_type> & vector_id)
      : the_coordinate{remove_coordinate(vector_id)} {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
    }

    static std::optional<coordinate_type> create_opt(
        std::string_view const coordinate);

    constexpr coordinate_enum as_enum() const { return the_coordinate; }

    void append(std::size_t & position_id) const {
        append_coordinate(position_id, the_coordinate);
    }

    template <typename uint_type>
    void append(std::vector<uint_type> & vector_id) const {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
        append_coordinate(vector_id, the_coordinate);
    }

    logic_digits_type as_logic_digits() const;
    std::string as_string() const;

    bool next();
    bool prev();
    coordinate_type & operator++();
    coordinate_type & operator--();

    void flip_file();
    void flip_rank();
    void flip_a8h1();

private:
    coordinate_enum the_coordinate;
};


std::ostream & operator<<(std::ostream & out, coordinate_type coordinate);


constexpr bool operator==(
    coordinate_type const lhs,
    coordinate_type const rhs
) {
    return lhs.as_enum() == rhs.as_enum();
}


constexpr bool operator!=(
    coordinate_type const lhs,
    coordinate_type const rhs
) {
    return not (lhs == rhs);
}


constexpr bool operator<(
    coordinate_type const lhs,
    coordinate_type const rhs
) {
    return lhs.as_enum() < rhs.as_enum();
}


constexpr bool operator>(
    coordinate_type const lhs,
    coordinate_type const rhs
) {
    return rhs < lhs;
}


constexpr bool operator<=(
    coordinate_type const lhs,
    coordinate_type const rhs
) {
    return not (rhs < lhs);
}


constexpr bool operator>=(
    coordinate_type const lhs,
    coordinate_type const rhs
) {
    return not (lhs < rhs);
}


constexpr coordinate_type a8{file_enum::a_file | rank_enum::rank_8};
static_assert( 0 == static_cast<unsigned char>(a8.as_enum()));
constexpr coordinate_type no_ep{a8};
constexpr coordinate_type b8{file_enum::b_file | rank_enum::rank_8};
static_assert( 1 == static_cast<unsigned char>(b8.as_enum()));
constexpr coordinate_type c8{file_enum::c_file | rank_enum::rank_8};
static_assert( 2 == static_cast<unsigned char>(c8.as_enum()));
constexpr coordinate_type d8{file_enum::d_file | rank_enum::rank_8};
static_assert( 3 == static_cast<unsigned char>(d8.as_enum()));
constexpr coordinate_type e8{file_enum::e_file | rank_enum::rank_8};
static_assert( 4 == static_cast<unsigned char>(e8.as_enum()));
constexpr coordinate_type f8{file_enum::f_file | rank_enum::rank_8};
static_assert( 5 == static_cast<unsigned char>(f8.as_enum()));
constexpr coordinate_type g8{file_enum::g_file | rank_enum::rank_8};
static_assert( 6 == static_cast<unsigned char>(g8.as_enum()));
constexpr coordinate_type h8{file_enum::h_file | rank_enum::rank_8};
static_assert( 7 == static_cast<unsigned char>(h8.as_enum()));
constexpr coordinate_type a7{file_enum::a_file | rank_enum::rank_7};
static_assert( 8 == static_cast<unsigned char>(a7.as_enum()));
constexpr coordinate_type b7{file_enum::b_file | rank_enum::rank_7};
static_assert( 9 == static_cast<unsigned char>(b7.as_enum()));
constexpr coordinate_type c7{file_enum::c_file | rank_enum::rank_7};
static_assert(10 == static_cast<unsigned char>(c7.as_enum()));
constexpr coordinate_type d7{file_enum::d_file | rank_enum::rank_7};
static_assert(11 == static_cast<unsigned char>(d7.as_enum()));
constexpr coordinate_type e7{file_enum::e_file | rank_enum::rank_7};
static_assert(12 == static_cast<unsigned char>(e7.as_enum()));
constexpr coordinate_type f7{file_enum::f_file | rank_enum::rank_7};
static_assert(13 == static_cast<unsigned char>(f7.as_enum()));
constexpr coordinate_type g7{file_enum::g_file | rank_enum::rank_7};
static_assert(14 == static_cast<unsigned char>(g7.as_enum()));
constexpr coordinate_type h7{file_enum::h_file | rank_enum::rank_7};
static_assert(15 == static_cast<unsigned char>(h7.as_enum()));
constexpr coordinate_type a6{file_enum::a_file | rank_enum::rank_6};
static_assert(16 == static_cast<unsigned char>(a6.as_enum()));
constexpr coordinate_type b6{file_enum::b_file | rank_enum::rank_6};
static_assert(17 == static_cast<unsigned char>(b6.as_enum()));
constexpr coordinate_type c6{file_enum::c_file | rank_enum::rank_6};
static_assert(18 == static_cast<unsigned char>(c6.as_enum()));
constexpr coordinate_type d6{file_enum::d_file | rank_enum::rank_6};
static_assert(19 == static_cast<unsigned char>(d6.as_enum()));
constexpr coordinate_type e6{file_enum::e_file | rank_enum::rank_6};
static_assert(20 == static_cast<unsigned char>(e6.as_enum()));
constexpr coordinate_type f6{file_enum::f_file | rank_enum::rank_6};
static_assert(21 == static_cast<unsigned char>(f6.as_enum()));
constexpr coordinate_type g6{file_enum::g_file | rank_enum::rank_6};
static_assert(22 == static_cast<unsigned char>(g6.as_enum()));
constexpr coordinate_type h6{file_enum::h_file | rank_enum::rank_6};
static_assert(23 == static_cast<unsigned char>(h6.as_enum()));
constexpr coordinate_type a5{file_enum::a_file | rank_enum::rank_5};
static_assert(24 == static_cast<unsigned char>(a5.as_enum()));
constexpr coordinate_type b5{file_enum::b_file | rank_enum::rank_5};
static_assert(25 == static_cast<unsigned char>(b5.as_enum()));
constexpr coordinate_type c5{file_enum::c_file | rank_enum::rank_5};
static_assert(26 == static_cast<unsigned char>(c5.as_enum()));
constexpr coordinate_type d5{file_enum::d_file | rank_enum::rank_5};
static_assert(27 == static_cast<unsigned char>(d5.as_enum()));
constexpr coordinate_type e5{file_enum::e_file | rank_enum::rank_5};
static_assert(28 == static_cast<unsigned char>(e5.as_enum()));
constexpr coordinate_type f5{file_enum::f_file | rank_enum::rank_5};
static_assert(29 == static_cast<unsigned char>(f5.as_enum()));
constexpr coordinate_type g5{file_enum::g_file | rank_enum::rank_5};
static_assert(30 == static_cast<unsigned char>(g5.as_enum()));
constexpr coordinate_type h5{file_enum::h_file | rank_enum::rank_5};
static_assert(31 == static_cast<unsigned char>(h5.as_enum()));
constexpr coordinate_type a4{file_enum::a_file | rank_enum::rank_4};
static_assert(32 == static_cast<unsigned char>(a4.as_enum()));
constexpr coordinate_type b4{file_enum::b_file | rank_enum::rank_4};
static_assert(33 == static_cast<unsigned char>(b4.as_enum()));
constexpr coordinate_type c4{file_enum::c_file | rank_enum::rank_4};
static_assert(34 == static_cast<unsigned char>(c4.as_enum()));
constexpr coordinate_type d4{file_enum::d_file | rank_enum::rank_4};
static_assert(35 == static_cast<unsigned char>(d4.as_enum()));
constexpr coordinate_type e4{file_enum::e_file | rank_enum::rank_4};
static_assert(36 == static_cast<unsigned char>(e4.as_enum()));
constexpr coordinate_type f4{file_enum::f_file | rank_enum::rank_4};
static_assert(37 == static_cast<unsigned char>(f4.as_enum()));
constexpr coordinate_type g4{file_enum::g_file | rank_enum::rank_4};
static_assert(38 == static_cast<unsigned char>(g4.as_enum()));
constexpr coordinate_type h4{file_enum::h_file | rank_enum::rank_4};
static_assert(39 == static_cast<unsigned char>(h4.as_enum()));
constexpr coordinate_type a3{file_enum::a_file | rank_enum::rank_3};
static_assert(40 == static_cast<unsigned char>(a3.as_enum()));
constexpr coordinate_type b3{file_enum::b_file | rank_enum::rank_3};
static_assert(41 == static_cast<unsigned char>(b3.as_enum()));
constexpr coordinate_type c3{file_enum::c_file | rank_enum::rank_3};
static_assert(42 == static_cast<unsigned char>(c3.as_enum()));
constexpr coordinate_type d3{file_enum::d_file | rank_enum::rank_3};
static_assert(43 == static_cast<unsigned char>(d3.as_enum()));
constexpr coordinate_type e3{file_enum::e_file | rank_enum::rank_3};
static_assert(44 == static_cast<unsigned char>(e3.as_enum()));
constexpr coordinate_type f3{file_enum::f_file | rank_enum::rank_3};
static_assert(45 == static_cast<unsigned char>(f3.as_enum()));
constexpr coordinate_type g3{file_enum::g_file | rank_enum::rank_3};
static_assert(46 == static_cast<unsigned char>(g3.as_enum()));
constexpr coordinate_type h3{file_enum::h_file | rank_enum::rank_3};
static_assert(47 == static_cast<unsigned char>(h3.as_enum()));
constexpr coordinate_type a2{file_enum::a_file | rank_enum::rank_2};
static_assert(48 == static_cast<unsigned char>(a2.as_enum()));
constexpr coordinate_type b2{file_enum::b_file | rank_enum::rank_2};
static_assert(49 == static_cast<unsigned char>(b2.as_enum()));
constexpr coordinate_type c2{file_enum::c_file | rank_enum::rank_2};
static_assert(50 == static_cast<unsigned char>(c2.as_enum()));
constexpr coordinate_type d2{file_enum::d_file | rank_enum::rank_2};
static_assert(51 == static_cast<unsigned char>(d2.as_enum()));
constexpr coordinate_type e2{file_enum::e_file | rank_enum::rank_2};
static_assert(52 == static_cast<unsigned char>(e2.as_enum()));
constexpr coordinate_type f2{file_enum::f_file | rank_enum::rank_2};
static_assert(53 == static_cast<unsigned char>(f2.as_enum()));
constexpr coordinate_type g2{file_enum::g_file | rank_enum::rank_2};
static_assert(54 == static_cast<unsigned char>(g2.as_enum()));
constexpr coordinate_type h2{file_enum::h_file | rank_enum::rank_2};
static_assert(55 == static_cast<unsigned char>(h2.as_enum()));
constexpr coordinate_type a1{file_enum::a_file | rank_enum::rank_1};
static_assert(56 == static_cast<unsigned char>(a1.as_enum()));
constexpr coordinate_type b1{file_enum::b_file | rank_enum::rank_1};
static_assert(57 == static_cast<unsigned char>(b1.as_enum()));
constexpr coordinate_type c1{file_enum::c_file | rank_enum::rank_1};
static_assert(58 == static_cast<unsigned char>(c1.as_enum()));
constexpr coordinate_type d1{file_enum::d_file | rank_enum::rank_1};
static_assert(59 == static_cast<unsigned char>(d1.as_enum()));
constexpr coordinate_type e1{file_enum::e_file | rank_enum::rank_1};
static_assert(60 == static_cast<unsigned char>(e1.as_enum()));
constexpr coordinate_type f1{file_enum::f_file | rank_enum::rank_1};
static_assert(61 == static_cast<unsigned char>(f1.as_enum()));
constexpr coordinate_type g1{file_enum::g_file | rank_enum::rank_1};
static_assert(62 == static_cast<unsigned char>(g1.as_enum()));
constexpr coordinate_type h1{file_enum::h_file | rank_enum::rank_1};
static_assert(63 == static_cast<unsigned char>(h1.as_enum()));


} // namespace representation


namespace std {


template <> class hash<representation::coordinate_type> {
public:
    std::size_t operator()(representation::coordinate_type const & coordinate)
     const {
        return hash<representation::coordinate_enum>()(coordinate.as_enum());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_COORDINATE_TYPE_HPP
