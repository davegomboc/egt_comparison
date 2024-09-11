#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_PLACEMENT_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_PLACEMENT_TYPE_HPP


#include <functional>
#include <string>
#include <type_traits>
#include <vector>

#include "representation/coordinate_type.hpp"
#include "representation/figurine_type.hpp"
#include "representation/logic_digits_type.hpp"


namespace representation {


class figurine_placement_type {
public:
    figurine_placement_type & operator=(figurine_placement_type const &) =
     default;
    figurine_placement_type & operator=(figurine_placement_type &&) = default;
    constexpr figurine_placement_type(figurine_placement_type const &) =
     default;
    constexpr figurine_placement_type(figurine_placement_type &&) = default;
    ~figurine_placement_type() = default;

    constexpr figurine_placement_type(
     figurine_type figurine, coordinate_type coordinate)
     : the_coordinate(coordinate), the_figurine(figurine) {}

    figurine_placement_type(
        figurine_type figurine,
        std::size_t & position_id
    ) : the_coordinate(position_id), the_figurine(figurine) {}

    template <typename uint_type>
    figurine_placement_type(
        figurine_type figurine,
        std::vector<uint_type> & vector_id
    ) : the_coordinate(vector_id), the_figurine(figurine) {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
    }

    constexpr coordinate_type coordinate() const { return the_coordinate; }
    constexpr figurine_type figurine() const { return the_figurine; }

    // Returns the potential en_passant_target whenever it is indicated that a
    // double pawn push has just occurred by the pawn being on its own back rank.
    // Otherwise, returns no_ep.
    constexpr coordinate_type
    identify_en_passant_target_from_illegal_pawn_placement() const {
        if ((black_pawn == the_figurine)
         and (rank_enum::rank_8 == just_rank(the_coordinate.as_enum()))) {
            return just_file(the_coordinate.as_enum()) | rank_enum::rank_6;
        }
        if ((white_pawn == the_figurine)
         and (rank_enum::rank_1 == just_rank(the_coordinate.as_enum()))) {
            return just_file(the_coordinate.as_enum()) | rank_enum::rank_3;
        }
        return no_ep;
    }

    std::string as_string() const;
    logic_digits_type as_logic_digits() const;

    void append(std::size_t & position_id) const {
        the_coordinate.append(position_id);
    }

    template <typename uint_type>
    void append(std::vector<uint_type> & vector_id) const {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
        // Each material signature is in its own table, so we only need to
        // record the coordinate.
        the_coordinate.append(vector_id);
    }

    void flip_a8h1();
    void flip_colour();
    void flip_file();
    void flip_rank();

private:
    coordinate_type the_coordinate;
    figurine_type the_figurine;
};


constexpr bool operator==(
    figurine_placement_type const lhs,
    figurine_placement_type const rhs
) {
    return (lhs.figurine() == rhs.figurine()) and
     (lhs.coordinate() == rhs.coordinate());
}


constexpr bool operator!=(
    figurine_placement_type const lhs,
    figurine_placement_type const rhs
) {
    return not (lhs == rhs);
}


constexpr bool operator<(
    figurine_placement_type const lhs,
    figurine_placement_type const rhs
) {
    return (lhs.figurine() == rhs.figurine()) ?
     (lhs.coordinate() < rhs.coordinate()) : (lhs.figurine() < rhs.figurine());
}


constexpr bool operator>(
    figurine_placement_type const lhs,
    figurine_placement_type const rhs
) {
    return rhs < lhs;
}


constexpr bool operator<=(
    figurine_placement_type const lhs,
    figurine_placement_type const rhs
) {
    return not (lhs > rhs);
}


constexpr bool operator>=(
    figurine_placement_type const lhs,
    figurine_placement_type const rhs
) {
    return not (lhs < rhs);
}


std::ostream & operator<<(
    std::ostream & out,
    figurine_placement_type figurine_placement);


constexpr figurine_placement_type Ka8{white_king, a8};
constexpr figurine_placement_type Kb8{white_king, b8};
constexpr figurine_placement_type Kc8{white_king, c8};
constexpr figurine_placement_type Kd8{white_king, d8};
constexpr figurine_placement_type Ke8{white_king, e8};
constexpr figurine_placement_type Kf8{white_king, f8};
constexpr figurine_placement_type Kg8{white_king, g8};
constexpr figurine_placement_type Kh8{white_king, h8};
constexpr figurine_placement_type Ka7{white_king, a7};
constexpr figurine_placement_type Kb7{white_king, b7};
constexpr figurine_placement_type Kc7{white_king, c7};
constexpr figurine_placement_type Kd7{white_king, d7};
constexpr figurine_placement_type Ke7{white_king, e7};
constexpr figurine_placement_type Kf7{white_king, f7};
constexpr figurine_placement_type Kg7{white_king, g7};
constexpr figurine_placement_type Kh7{white_king, h7};
constexpr figurine_placement_type Ka6{white_king, a6};
constexpr figurine_placement_type Kb6{white_king, b6};
constexpr figurine_placement_type Kc6{white_king, c6};
constexpr figurine_placement_type Kd6{white_king, d6};
constexpr figurine_placement_type Ke6{white_king, e6};
constexpr figurine_placement_type Kf6{white_king, f6};
constexpr figurine_placement_type Kg6{white_king, g6};
constexpr figurine_placement_type Kh6{white_king, h6};
constexpr figurine_placement_type Ka5{white_king, a5};
constexpr figurine_placement_type Kb5{white_king, b5};
constexpr figurine_placement_type Kc5{white_king, c5};
constexpr figurine_placement_type Kd5{white_king, d5};
constexpr figurine_placement_type Ke5{white_king, e5};
constexpr figurine_placement_type Kf5{white_king, f5};
constexpr figurine_placement_type Kg5{white_king, g5};
constexpr figurine_placement_type Kh5{white_king, h5};
constexpr figurine_placement_type Ka4{white_king, a4};
constexpr figurine_placement_type Kb4{white_king, b4};
constexpr figurine_placement_type Kc4{white_king, c4};
constexpr figurine_placement_type Kd4{white_king, d4};
constexpr figurine_placement_type Ke4{white_king, e4};
constexpr figurine_placement_type Kf4{white_king, f4};
constexpr figurine_placement_type Kg4{white_king, g4};
constexpr figurine_placement_type Kh4{white_king, h4};
constexpr figurine_placement_type Ka3{white_king, a3};
constexpr figurine_placement_type Kb3{white_king, b3};
constexpr figurine_placement_type Kc3{white_king, c3};
constexpr figurine_placement_type Kd3{white_king, d3};
constexpr figurine_placement_type Ke3{white_king, e3};
constexpr figurine_placement_type Kf3{white_king, f3};
constexpr figurine_placement_type Kg3{white_king, g3};
constexpr figurine_placement_type Kh3{white_king, h3};
constexpr figurine_placement_type Ka2{white_king, a2};
constexpr figurine_placement_type Kb2{white_king, b2};
constexpr figurine_placement_type Kc2{white_king, c2};
constexpr figurine_placement_type Kd2{white_king, d2};
constexpr figurine_placement_type Ke2{white_king, e2};
constexpr figurine_placement_type Kf2{white_king, f2};
constexpr figurine_placement_type Kg2{white_king, g2};
constexpr figurine_placement_type Kh2{white_king, h2};
constexpr figurine_placement_type Ka1{white_king, a1};
constexpr figurine_placement_type Kb1{white_king, b1};
constexpr figurine_placement_type Kc1{white_king, c1};
constexpr figurine_placement_type Kd1{white_king, d1};
constexpr figurine_placement_type Ke1{white_king, e1};
constexpr figurine_placement_type Kf1{white_king, f1};
constexpr figurine_placement_type Kg1{white_king, g1};
constexpr figurine_placement_type Kh1{white_king, h1};
constexpr figurine_placement_type ka8{black_king, a8};
constexpr figurine_placement_type kb8{black_king, b8};
constexpr figurine_placement_type kc8{black_king, c8};
constexpr figurine_placement_type kd8{black_king, d8};
constexpr figurine_placement_type ke8{black_king, e8};
constexpr figurine_placement_type kf8{black_king, f8};
constexpr figurine_placement_type kg8{black_king, g8};
constexpr figurine_placement_type kh8{black_king, h8};
constexpr figurine_placement_type ka7{black_king, a7};
constexpr figurine_placement_type kb7{black_king, b7};
constexpr figurine_placement_type kc7{black_king, c7};
constexpr figurine_placement_type kd7{black_king, d7};
constexpr figurine_placement_type ke7{black_king, e7};
constexpr figurine_placement_type kf7{black_king, f7};
constexpr figurine_placement_type kg7{black_king, g7};
constexpr figurine_placement_type kh7{black_king, h7};
constexpr figurine_placement_type ka6{black_king, a6};
constexpr figurine_placement_type kb6{black_king, b6};
constexpr figurine_placement_type kc6{black_king, c6};
constexpr figurine_placement_type kd6{black_king, d6};
constexpr figurine_placement_type ke6{black_king, e6};
constexpr figurine_placement_type kf6{black_king, f6};
constexpr figurine_placement_type kg6{black_king, g6};
constexpr figurine_placement_type kh6{black_king, h6};
constexpr figurine_placement_type ka5{black_king, a5};
constexpr figurine_placement_type kb5{black_king, b5};
constexpr figurine_placement_type kc5{black_king, c5};
constexpr figurine_placement_type kd5{black_king, d5};
constexpr figurine_placement_type ke5{black_king, e5};
constexpr figurine_placement_type kf5{black_king, f5};
constexpr figurine_placement_type kg5{black_king, g5};
constexpr figurine_placement_type kh5{black_king, h5};
constexpr figurine_placement_type ka4{black_king, a4};
constexpr figurine_placement_type kb4{black_king, b4};
constexpr figurine_placement_type kc4{black_king, c4};
constexpr figurine_placement_type kd4{black_king, d4};
constexpr figurine_placement_type ke4{black_king, e4};
constexpr figurine_placement_type kf4{black_king, f4};
constexpr figurine_placement_type kg4{black_king, g4};
constexpr figurine_placement_type kh4{black_king, h4};
constexpr figurine_placement_type ka3{black_king, a3};
constexpr figurine_placement_type kb3{black_king, b3};
constexpr figurine_placement_type kc3{black_king, c3};
constexpr figurine_placement_type kd3{black_king, d3};
constexpr figurine_placement_type ke3{black_king, e3};
constexpr figurine_placement_type kf3{black_king, f3};
constexpr figurine_placement_type kg3{black_king, g3};
constexpr figurine_placement_type kh3{black_king, h3};
constexpr figurine_placement_type ka2{black_king, a2};
constexpr figurine_placement_type kb2{black_king, b2};
constexpr figurine_placement_type kc2{black_king, c2};
constexpr figurine_placement_type kd2{black_king, d2};
constexpr figurine_placement_type ke2{black_king, e2};
constexpr figurine_placement_type kf2{black_king, f2};
constexpr figurine_placement_type kg2{black_king, g2};
constexpr figurine_placement_type kh2{black_king, h2};
constexpr figurine_placement_type ka1{black_king, a1};
constexpr figurine_placement_type kb1{black_king, b1};
constexpr figurine_placement_type kc1{black_king, c1};
constexpr figurine_placement_type kd1{black_king, d1};
constexpr figurine_placement_type ke1{black_king, e1};
constexpr figurine_placement_type kf1{black_king, f1};
constexpr figurine_placement_type kg1{black_king, g1};
constexpr figurine_placement_type kh1{black_king, h1};
constexpr figurine_placement_type Qa8{white_queen, a8};
constexpr figurine_placement_type Qb8{white_queen, b8};
constexpr figurine_placement_type Qc8{white_queen, c8};
constexpr figurine_placement_type Qd8{white_queen, d8};
constexpr figurine_placement_type Qe8{white_queen, e8};
constexpr figurine_placement_type Qf8{white_queen, f8};
constexpr figurine_placement_type Qg8{white_queen, g8};
constexpr figurine_placement_type Qh8{white_queen, h8};
constexpr figurine_placement_type Qa7{white_queen, a7};
constexpr figurine_placement_type Qb7{white_queen, b7};
constexpr figurine_placement_type Qc7{white_queen, c7};
constexpr figurine_placement_type Qd7{white_queen, d7};
constexpr figurine_placement_type Qe7{white_queen, e7};
constexpr figurine_placement_type Qf7{white_queen, f7};
constexpr figurine_placement_type Qg7{white_queen, g7};
constexpr figurine_placement_type Qh7{white_queen, h7};
constexpr figurine_placement_type Qa6{white_queen, a6};
constexpr figurine_placement_type Qb6{white_queen, b6};
constexpr figurine_placement_type Qc6{white_queen, c6};
constexpr figurine_placement_type Qd6{white_queen, d6};
constexpr figurine_placement_type Qe6{white_queen, e6};
constexpr figurine_placement_type Qf6{white_queen, f6};
constexpr figurine_placement_type Qg6{white_queen, g6};
constexpr figurine_placement_type Qh6{white_queen, h6};
constexpr figurine_placement_type Qa5{white_queen, a5};
constexpr figurine_placement_type Qb5{white_queen, b5};
constexpr figurine_placement_type Qc5{white_queen, c5};
constexpr figurine_placement_type Qd5{white_queen, d5};
constexpr figurine_placement_type Qe5{white_queen, e5};
constexpr figurine_placement_type Qf5{white_queen, f5};
constexpr figurine_placement_type Qg5{white_queen, g5};
constexpr figurine_placement_type Qh5{white_queen, h5};
constexpr figurine_placement_type Qa4{white_queen, a4};
constexpr figurine_placement_type Qb4{white_queen, b4};
constexpr figurine_placement_type Qc4{white_queen, c4};
constexpr figurine_placement_type Qd4{white_queen, d4};
constexpr figurine_placement_type Qe4{white_queen, e4};
constexpr figurine_placement_type Qf4{white_queen, f4};
constexpr figurine_placement_type Qg4{white_queen, g4};
constexpr figurine_placement_type Qh4{white_queen, h4};
constexpr figurine_placement_type Qa3{white_queen, a3};
constexpr figurine_placement_type Qb3{white_queen, b3};
constexpr figurine_placement_type Qc3{white_queen, c3};
constexpr figurine_placement_type Qd3{white_queen, d3};
constexpr figurine_placement_type Qe3{white_queen, e3};
constexpr figurine_placement_type Qf3{white_queen, f3};
constexpr figurine_placement_type Qg3{white_queen, g3};
constexpr figurine_placement_type Qh3{white_queen, h3};
constexpr figurine_placement_type Qa2{white_queen, a2};
constexpr figurine_placement_type Qb2{white_queen, b2};
constexpr figurine_placement_type Qc2{white_queen, c2};
constexpr figurine_placement_type Qd2{white_queen, d2};
constexpr figurine_placement_type Qe2{white_queen, e2};
constexpr figurine_placement_type Qf2{white_queen, f2};
constexpr figurine_placement_type Qg2{white_queen, g2};
constexpr figurine_placement_type Qh2{white_queen, h2};
constexpr figurine_placement_type Qa1{white_queen, a1};
constexpr figurine_placement_type Qb1{white_queen, b1};
constexpr figurine_placement_type Qc1{white_queen, c1};
constexpr figurine_placement_type Qd1{white_queen, d1};
constexpr figurine_placement_type Qe1{white_queen, e1};
constexpr figurine_placement_type Qf1{white_queen, f1};
constexpr figurine_placement_type Qg1{white_queen, g1};
constexpr figurine_placement_type Qh1{white_queen, h1};
constexpr figurine_placement_type qa8{black_queen, a8};
constexpr figurine_placement_type qb8{black_queen, b8};
constexpr figurine_placement_type qc8{black_queen, c8};
constexpr figurine_placement_type qd8{black_queen, d8};
constexpr figurine_placement_type qe8{black_queen, e8};
constexpr figurine_placement_type qf8{black_queen, f8};
constexpr figurine_placement_type qg8{black_queen, g8};
constexpr figurine_placement_type qh8{black_queen, h8};
constexpr figurine_placement_type qa7{black_queen, a7};
constexpr figurine_placement_type qb7{black_queen, b7};
constexpr figurine_placement_type qc7{black_queen, c7};
constexpr figurine_placement_type qd7{black_queen, d7};
constexpr figurine_placement_type qe7{black_queen, e7};
constexpr figurine_placement_type qf7{black_queen, f7};
constexpr figurine_placement_type qg7{black_queen, g7};
constexpr figurine_placement_type qh7{black_queen, h7};
constexpr figurine_placement_type qa6{black_queen, a6};
constexpr figurine_placement_type qb6{black_queen, b6};
constexpr figurine_placement_type qc6{black_queen, c6};
constexpr figurine_placement_type qd6{black_queen, d6};
constexpr figurine_placement_type qe6{black_queen, e6};
constexpr figurine_placement_type qf6{black_queen, f6};
constexpr figurine_placement_type qg6{black_queen, g6};
constexpr figurine_placement_type qh6{black_queen, h6};
constexpr figurine_placement_type qa5{black_queen, a5};
constexpr figurine_placement_type qb5{black_queen, b5};
constexpr figurine_placement_type qc5{black_queen, c5};
constexpr figurine_placement_type qd5{black_queen, d5};
constexpr figurine_placement_type qe5{black_queen, e5};
constexpr figurine_placement_type qf5{black_queen, f5};
constexpr figurine_placement_type qg5{black_queen, g5};
constexpr figurine_placement_type qh5{black_queen, h5};
constexpr figurine_placement_type qa4{black_queen, a4};
constexpr figurine_placement_type qb4{black_queen, b4};
constexpr figurine_placement_type qc4{black_queen, c4};
constexpr figurine_placement_type qd4{black_queen, d4};
constexpr figurine_placement_type qe4{black_queen, e4};
constexpr figurine_placement_type qf4{black_queen, f4};
constexpr figurine_placement_type qg4{black_queen, g4};
constexpr figurine_placement_type qh4{black_queen, h4};
constexpr figurine_placement_type qa3{black_queen, a3};
constexpr figurine_placement_type qb3{black_queen, b3};
constexpr figurine_placement_type qc3{black_queen, c3};
constexpr figurine_placement_type qd3{black_queen, d3};
constexpr figurine_placement_type qe3{black_queen, e3};
constexpr figurine_placement_type qf3{black_queen, f3};
constexpr figurine_placement_type qg3{black_queen, g3};
constexpr figurine_placement_type qh3{black_queen, h3};
constexpr figurine_placement_type qa2{black_queen, a2};
constexpr figurine_placement_type qb2{black_queen, b2};
constexpr figurine_placement_type qc2{black_queen, c2};
constexpr figurine_placement_type qd2{black_queen, d2};
constexpr figurine_placement_type qe2{black_queen, e2};
constexpr figurine_placement_type qf2{black_queen, f2};
constexpr figurine_placement_type qg2{black_queen, g2};
constexpr figurine_placement_type qh2{black_queen, h2};
constexpr figurine_placement_type qa1{black_queen, a1};
constexpr figurine_placement_type qb1{black_queen, b1};
constexpr figurine_placement_type qc1{black_queen, c1};
constexpr figurine_placement_type qd1{black_queen, d1};
constexpr figurine_placement_type qe1{black_queen, e1};
constexpr figurine_placement_type qf1{black_queen, f1};
constexpr figurine_placement_type qg1{black_queen, g1};
constexpr figurine_placement_type qh1{black_queen, h1};
constexpr figurine_placement_type Ra8{white_rook, a8};
constexpr figurine_placement_type Rb8{white_rook, b8};
constexpr figurine_placement_type Rc8{white_rook, c8};
constexpr figurine_placement_type Rd8{white_rook, d8};
constexpr figurine_placement_type Re8{white_rook, e8};
constexpr figurine_placement_type Rf8{white_rook, f8};
constexpr figurine_placement_type Rg8{white_rook, g8};
constexpr figurine_placement_type Rh8{white_rook, h8};
constexpr figurine_placement_type Ra7{white_rook, a7};
constexpr figurine_placement_type Rb7{white_rook, b7};
constexpr figurine_placement_type Rc7{white_rook, c7};
constexpr figurine_placement_type Rd7{white_rook, d7};
constexpr figurine_placement_type Re7{white_rook, e7};
constexpr figurine_placement_type Rf7{white_rook, f7};
constexpr figurine_placement_type Rg7{white_rook, g7};
constexpr figurine_placement_type Rh7{white_rook, h7};
constexpr figurine_placement_type Ra6{white_rook, a6};
constexpr figurine_placement_type Rb6{white_rook, b6};
constexpr figurine_placement_type Rc6{white_rook, c6};
constexpr figurine_placement_type Rd6{white_rook, d6};
constexpr figurine_placement_type Re6{white_rook, e6};
constexpr figurine_placement_type Rf6{white_rook, f6};
constexpr figurine_placement_type Rg6{white_rook, g6};
constexpr figurine_placement_type Rh6{white_rook, h6};
constexpr figurine_placement_type Ra5{white_rook, a5};
constexpr figurine_placement_type Rb5{white_rook, b5};
constexpr figurine_placement_type Rc5{white_rook, c5};
constexpr figurine_placement_type Rd5{white_rook, d5};
constexpr figurine_placement_type Re5{white_rook, e5};
constexpr figurine_placement_type Rf5{white_rook, f5};
constexpr figurine_placement_type Rg5{white_rook, g5};
constexpr figurine_placement_type Rh5{white_rook, h5};
constexpr figurine_placement_type Ra4{white_rook, a4};
constexpr figurine_placement_type Rb4{white_rook, b4};
constexpr figurine_placement_type Rc4{white_rook, c4};
constexpr figurine_placement_type Rd4{white_rook, d4};
constexpr figurine_placement_type Re4{white_rook, e4};
constexpr figurine_placement_type Rf4{white_rook, f4};
constexpr figurine_placement_type Rg4{white_rook, g4};
constexpr figurine_placement_type Rh4{white_rook, h4};
constexpr figurine_placement_type Ra3{white_rook, a3};
constexpr figurine_placement_type Rb3{white_rook, b3};
constexpr figurine_placement_type Rc3{white_rook, c3};
constexpr figurine_placement_type Rd3{white_rook, d3};
constexpr figurine_placement_type Re3{white_rook, e3};
constexpr figurine_placement_type Rf3{white_rook, f3};
constexpr figurine_placement_type Rg3{white_rook, g3};
constexpr figurine_placement_type Rh3{white_rook, h3};
constexpr figurine_placement_type Ra2{white_rook, a2};
constexpr figurine_placement_type Rb2{white_rook, b2};
constexpr figurine_placement_type Rc2{white_rook, c2};
constexpr figurine_placement_type Rd2{white_rook, d2};
constexpr figurine_placement_type Re2{white_rook, e2};
constexpr figurine_placement_type Rf2{white_rook, f2};
constexpr figurine_placement_type Rg2{white_rook, g2};
constexpr figurine_placement_type Rh2{white_rook, h2};
constexpr figurine_placement_type Ra1{white_rook, a1};
constexpr figurine_placement_type Rb1{white_rook, b1};
constexpr figurine_placement_type Rc1{white_rook, c1};
constexpr figurine_placement_type Rd1{white_rook, d1};
constexpr figurine_placement_type Re1{white_rook, e1};
constexpr figurine_placement_type Rf1{white_rook, f1};
constexpr figurine_placement_type Rg1{white_rook, g1};
constexpr figurine_placement_type Rh1{white_rook, h1};
constexpr figurine_placement_type ra8{black_rook, a8};
constexpr figurine_placement_type rb8{black_rook, b8};
constexpr figurine_placement_type rc8{black_rook, c8};
constexpr figurine_placement_type rd8{black_rook, d8};
constexpr figurine_placement_type re8{black_rook, e8};
constexpr figurine_placement_type rf8{black_rook, f8};
constexpr figurine_placement_type rg8{black_rook, g8};
constexpr figurine_placement_type rh8{black_rook, h8};
constexpr figurine_placement_type ra7{black_rook, a7};
constexpr figurine_placement_type rb7{black_rook, b7};
constexpr figurine_placement_type rc7{black_rook, c7};
constexpr figurine_placement_type rd7{black_rook, d7};
constexpr figurine_placement_type re7{black_rook, e7};
constexpr figurine_placement_type rf7{black_rook, f7};
constexpr figurine_placement_type rg7{black_rook, g7};
constexpr figurine_placement_type rh7{black_rook, h7};
constexpr figurine_placement_type ra6{black_rook, a6};
constexpr figurine_placement_type rb6{black_rook, b6};
constexpr figurine_placement_type rc6{black_rook, c6};
constexpr figurine_placement_type rd6{black_rook, d6};
constexpr figurine_placement_type re6{black_rook, e6};
constexpr figurine_placement_type rf6{black_rook, f6};
constexpr figurine_placement_type rg6{black_rook, g6};
constexpr figurine_placement_type rh6{black_rook, h6};
constexpr figurine_placement_type ra5{black_rook, a5};
constexpr figurine_placement_type rb5{black_rook, b5};
constexpr figurine_placement_type rc5{black_rook, c5};
constexpr figurine_placement_type rd5{black_rook, d5};
constexpr figurine_placement_type re5{black_rook, e5};
constexpr figurine_placement_type rf5{black_rook, f5};
constexpr figurine_placement_type rg5{black_rook, g5};
constexpr figurine_placement_type rh5{black_rook, h5};
constexpr figurine_placement_type ra4{black_rook, a4};
constexpr figurine_placement_type rb4{black_rook, b4};
constexpr figurine_placement_type rc4{black_rook, c4};
constexpr figurine_placement_type rd4{black_rook, d4};
constexpr figurine_placement_type re4{black_rook, e4};
constexpr figurine_placement_type rf4{black_rook, f4};
constexpr figurine_placement_type rg4{black_rook, g4};
constexpr figurine_placement_type rh4{black_rook, h4};
constexpr figurine_placement_type ra3{black_rook, a3};
constexpr figurine_placement_type rb3{black_rook, b3};
constexpr figurine_placement_type rc3{black_rook, c3};
constexpr figurine_placement_type rd3{black_rook, d3};
constexpr figurine_placement_type re3{black_rook, e3};
constexpr figurine_placement_type rf3{black_rook, f3};
constexpr figurine_placement_type rg3{black_rook, g3};
constexpr figurine_placement_type rh3{black_rook, h3};
constexpr figurine_placement_type ra2{black_rook, a2};
constexpr figurine_placement_type rb2{black_rook, b2};
constexpr figurine_placement_type rc2{black_rook, c2};
constexpr figurine_placement_type rd2{black_rook, d2};
constexpr figurine_placement_type re2{black_rook, e2};
constexpr figurine_placement_type rf2{black_rook, f2};
constexpr figurine_placement_type rg2{black_rook, g2};
constexpr figurine_placement_type rh2{black_rook, h2};
constexpr figurine_placement_type ra1{black_rook, a1};
constexpr figurine_placement_type rb1{black_rook, b1};
constexpr figurine_placement_type rc1{black_rook, c1};
constexpr figurine_placement_type rd1{black_rook, d1};
constexpr figurine_placement_type re1{black_rook, e1};
constexpr figurine_placement_type rf1{black_rook, f1};
constexpr figurine_placement_type rg1{black_rook, g1};
constexpr figurine_placement_type rh1{black_rook, h1};
constexpr figurine_placement_type Ba8{white_bishop, a8};
constexpr figurine_placement_type Bb8{white_bishop, b8};
constexpr figurine_placement_type Bc8{white_bishop, c8};
constexpr figurine_placement_type Bd8{white_bishop, d8};
constexpr figurine_placement_type Be8{white_bishop, e8};
constexpr figurine_placement_type Bf8{white_bishop, f8};
constexpr figurine_placement_type Bg8{white_bishop, g8};
constexpr figurine_placement_type Bh8{white_bishop, h8};
constexpr figurine_placement_type Ba7{white_bishop, a7};
constexpr figurine_placement_type Bb7{white_bishop, b7};
constexpr figurine_placement_type Bc7{white_bishop, c7};
constexpr figurine_placement_type Bd7{white_bishop, d7};
constexpr figurine_placement_type Be7{white_bishop, e7};
constexpr figurine_placement_type Bf7{white_bishop, f7};
constexpr figurine_placement_type Bg7{white_bishop, g7};
constexpr figurine_placement_type Bh7{white_bishop, h7};
constexpr figurine_placement_type Ba6{white_bishop, a6};
constexpr figurine_placement_type Bb6{white_bishop, b6};
constexpr figurine_placement_type Bc6{white_bishop, c6};
constexpr figurine_placement_type Bd6{white_bishop, d6};
constexpr figurine_placement_type Be6{white_bishop, e6};
constexpr figurine_placement_type Bf6{white_bishop, f6};
constexpr figurine_placement_type Bg6{white_bishop, g6};
constexpr figurine_placement_type Bh6{white_bishop, h6};
constexpr figurine_placement_type Ba5{white_bishop, a5};
constexpr figurine_placement_type Bb5{white_bishop, b5};
constexpr figurine_placement_type Bc5{white_bishop, c5};
constexpr figurine_placement_type Bd5{white_bishop, d5};
constexpr figurine_placement_type Be5{white_bishop, e5};
constexpr figurine_placement_type Bf5{white_bishop, f5};
constexpr figurine_placement_type Bg5{white_bishop, g5};
constexpr figurine_placement_type Bh5{white_bishop, h5};
constexpr figurine_placement_type Ba4{white_bishop, a4};
constexpr figurine_placement_type Bb4{white_bishop, b4};
constexpr figurine_placement_type Bc4{white_bishop, c4};
constexpr figurine_placement_type Bd4{white_bishop, d4};
constexpr figurine_placement_type Be4{white_bishop, e4};
constexpr figurine_placement_type Bf4{white_bishop, f4};
constexpr figurine_placement_type Bg4{white_bishop, g4};
constexpr figurine_placement_type Bh4{white_bishop, h4};
constexpr figurine_placement_type Ba3{white_bishop, a3};
constexpr figurine_placement_type Bb3{white_bishop, b3};
constexpr figurine_placement_type Bc3{white_bishop, c3};
constexpr figurine_placement_type Bd3{white_bishop, d3};
constexpr figurine_placement_type Be3{white_bishop, e3};
constexpr figurine_placement_type Bf3{white_bishop, f3};
constexpr figurine_placement_type Bg3{white_bishop, g3};
constexpr figurine_placement_type Bh3{white_bishop, h3};
constexpr figurine_placement_type Ba2{white_bishop, a2};
constexpr figurine_placement_type Bb2{white_bishop, b2};
constexpr figurine_placement_type Bc2{white_bishop, c2};
constexpr figurine_placement_type Bd2{white_bishop, d2};
constexpr figurine_placement_type Be2{white_bishop, e2};
constexpr figurine_placement_type Bf2{white_bishop, f2};
constexpr figurine_placement_type Bg2{white_bishop, g2};
constexpr figurine_placement_type Bh2{white_bishop, h2};
constexpr figurine_placement_type Ba1{white_bishop, a1};
constexpr figurine_placement_type Bb1{white_bishop, b1};
constexpr figurine_placement_type Bc1{white_bishop, c1};
constexpr figurine_placement_type Bd1{white_bishop, d1};
constexpr figurine_placement_type Be1{white_bishop, e1};
constexpr figurine_placement_type Bf1{white_bishop, f1};
constexpr figurine_placement_type Bg1{white_bishop, g1};
constexpr figurine_placement_type Bh1{white_bishop, h1};
constexpr figurine_placement_type ba8{black_bishop, a8};
constexpr figurine_placement_type bb8{black_bishop, b8};
constexpr figurine_placement_type bc8{black_bishop, c8};
constexpr figurine_placement_type bd8{black_bishop, d8};
constexpr figurine_placement_type be8{black_bishop, e8};
constexpr figurine_placement_type bf8{black_bishop, f8};
constexpr figurine_placement_type bg8{black_bishop, g8};
constexpr figurine_placement_type bh8{black_bishop, h8};
constexpr figurine_placement_type ba7{black_bishop, a7};
constexpr figurine_placement_type bb7{black_bishop, b7};
constexpr figurine_placement_type bc7{black_bishop, c7};
constexpr figurine_placement_type bd7{black_bishop, d7};
constexpr figurine_placement_type be7{black_bishop, e7};
constexpr figurine_placement_type bf7{black_bishop, f7};
constexpr figurine_placement_type bg7{black_bishop, g7};
constexpr figurine_placement_type bh7{black_bishop, h7};
constexpr figurine_placement_type ba6{black_bishop, a6};
constexpr figurine_placement_type bb6{black_bishop, b6};
constexpr figurine_placement_type bc6{black_bishop, c6};
constexpr figurine_placement_type bd6{black_bishop, d6};
constexpr figurine_placement_type be6{black_bishop, e6};
constexpr figurine_placement_type bf6{black_bishop, f6};
constexpr figurine_placement_type bg6{black_bishop, g6};
constexpr figurine_placement_type bh6{black_bishop, h6};
constexpr figurine_placement_type ba5{black_bishop, a5};
constexpr figurine_placement_type bb5{black_bishop, b5};
constexpr figurine_placement_type bc5{black_bishop, c5};
constexpr figurine_placement_type bd5{black_bishop, d5};
constexpr figurine_placement_type be5{black_bishop, e5};
constexpr figurine_placement_type bf5{black_bishop, f5};
constexpr figurine_placement_type bg5{black_bishop, g5};
constexpr figurine_placement_type bh5{black_bishop, h5};
constexpr figurine_placement_type ba4{black_bishop, a4};
constexpr figurine_placement_type bb4{black_bishop, b4};
constexpr figurine_placement_type bc4{black_bishop, c4};
constexpr figurine_placement_type bd4{black_bishop, d4};
constexpr figurine_placement_type be4{black_bishop, e4};
constexpr figurine_placement_type bf4{black_bishop, f4};
constexpr figurine_placement_type bg4{black_bishop, g4};
constexpr figurine_placement_type bh4{black_bishop, h4};
constexpr figurine_placement_type ba3{black_bishop, a3};
constexpr figurine_placement_type bb3{black_bishop, b3};
constexpr figurine_placement_type bc3{black_bishop, c3};
constexpr figurine_placement_type bd3{black_bishop, d3};
constexpr figurine_placement_type be3{black_bishop, e3};
constexpr figurine_placement_type bf3{black_bishop, f3};
constexpr figurine_placement_type bg3{black_bishop, g3};
constexpr figurine_placement_type bh3{black_bishop, h3};
constexpr figurine_placement_type ba2{black_bishop, a2};
constexpr figurine_placement_type bb2{black_bishop, b2};
constexpr figurine_placement_type bc2{black_bishop, c2};
constexpr figurine_placement_type bd2{black_bishop, d2};
constexpr figurine_placement_type be2{black_bishop, e2};
constexpr figurine_placement_type bf2{black_bishop, f2};
constexpr figurine_placement_type bg2{black_bishop, g2};
constexpr figurine_placement_type bh2{black_bishop, h2};
constexpr figurine_placement_type ba1{black_bishop, a1};
constexpr figurine_placement_type bb1{black_bishop, b1};
constexpr figurine_placement_type bc1{black_bishop, c1};
constexpr figurine_placement_type bd1{black_bishop, d1};
constexpr figurine_placement_type be1{black_bishop, e1};
constexpr figurine_placement_type bf1{black_bishop, f1};
constexpr figurine_placement_type bg1{black_bishop, g1};
constexpr figurine_placement_type bh1{black_bishop, h1};
constexpr figurine_placement_type Na8{white_knight, a8};
constexpr figurine_placement_type Nb8{white_knight, b8};
constexpr figurine_placement_type Nc8{white_knight, c8};
constexpr figurine_placement_type Nd8{white_knight, d8};
constexpr figurine_placement_type Ne8{white_knight, e8};
constexpr figurine_placement_type Nf8{white_knight, f8};
constexpr figurine_placement_type Ng8{white_knight, g8};
constexpr figurine_placement_type Nh8{white_knight, h8};
constexpr figurine_placement_type Na7{white_knight, a7};
constexpr figurine_placement_type Nb7{white_knight, b7};
constexpr figurine_placement_type Nc7{white_knight, c7};
constexpr figurine_placement_type Nd7{white_knight, d7};
constexpr figurine_placement_type Ne7{white_knight, e7};
constexpr figurine_placement_type Nf7{white_knight, f7};
constexpr figurine_placement_type Ng7{white_knight, g7};
constexpr figurine_placement_type Nh7{white_knight, h7};
constexpr figurine_placement_type Na6{white_knight, a6};
constexpr figurine_placement_type Nb6{white_knight, b6};
constexpr figurine_placement_type Nc6{white_knight, c6};
constexpr figurine_placement_type Nd6{white_knight, d6};
constexpr figurine_placement_type Ne6{white_knight, e6};
constexpr figurine_placement_type Nf6{white_knight, f6};
constexpr figurine_placement_type Ng6{white_knight, g6};
constexpr figurine_placement_type Nh6{white_knight, h6};
constexpr figurine_placement_type Na5{white_knight, a5};
constexpr figurine_placement_type Nb5{white_knight, b5};
constexpr figurine_placement_type Nc5{white_knight, c5};
constexpr figurine_placement_type Nd5{white_knight, d5};
constexpr figurine_placement_type Ne5{white_knight, e5};
constexpr figurine_placement_type Nf5{white_knight, f5};
constexpr figurine_placement_type Ng5{white_knight, g5};
constexpr figurine_placement_type Nh5{white_knight, h5};
constexpr figurine_placement_type Na4{white_knight, a4};
constexpr figurine_placement_type Nb4{white_knight, b4};
constexpr figurine_placement_type Nc4{white_knight, c4};
constexpr figurine_placement_type Nd4{white_knight, d4};
constexpr figurine_placement_type Ne4{white_knight, e4};
constexpr figurine_placement_type Nf4{white_knight, f4};
constexpr figurine_placement_type Ng4{white_knight, g4};
constexpr figurine_placement_type Nh4{white_knight, h4};
constexpr figurine_placement_type Na3{white_knight, a3};
constexpr figurine_placement_type Nb3{white_knight, b3};
constexpr figurine_placement_type Nc3{white_knight, c3};
constexpr figurine_placement_type Nd3{white_knight, d3};
constexpr figurine_placement_type Ne3{white_knight, e3};
constexpr figurine_placement_type Nf3{white_knight, f3};
constexpr figurine_placement_type Ng3{white_knight, g3};
constexpr figurine_placement_type Nh3{white_knight, h3};
constexpr figurine_placement_type Na2{white_knight, a2};
constexpr figurine_placement_type Nb2{white_knight, b2};
constexpr figurine_placement_type Nc2{white_knight, c2};
constexpr figurine_placement_type Nd2{white_knight, d2};
constexpr figurine_placement_type Ne2{white_knight, e2};
constexpr figurine_placement_type Nf2{white_knight, f2};
constexpr figurine_placement_type Ng2{white_knight, g2};
constexpr figurine_placement_type Nh2{white_knight, h2};
constexpr figurine_placement_type Na1{white_knight, a1};
constexpr figurine_placement_type Nb1{white_knight, b1};
constexpr figurine_placement_type Nc1{white_knight, c1};
constexpr figurine_placement_type Nd1{white_knight, d1};
constexpr figurine_placement_type Ne1{white_knight, e1};
constexpr figurine_placement_type Nf1{white_knight, f1};
constexpr figurine_placement_type Ng1{white_knight, g1};
constexpr figurine_placement_type Nh1{white_knight, h1};
constexpr figurine_placement_type na8{black_knight, a8};
constexpr figurine_placement_type nb8{black_knight, b8};
constexpr figurine_placement_type nc8{black_knight, c8};
constexpr figurine_placement_type nd8{black_knight, d8};
constexpr figurine_placement_type ne8{black_knight, e8};
constexpr figurine_placement_type nf8{black_knight, f8};
constexpr figurine_placement_type ng8{black_knight, g8};
constexpr figurine_placement_type nh8{black_knight, h8};
constexpr figurine_placement_type na7{black_knight, a7};
constexpr figurine_placement_type nb7{black_knight, b7};
constexpr figurine_placement_type nc7{black_knight, c7};
constexpr figurine_placement_type nd7{black_knight, d7};
constexpr figurine_placement_type ne7{black_knight, e7};
constexpr figurine_placement_type nf7{black_knight, f7};
constexpr figurine_placement_type ng7{black_knight, g7};
constexpr figurine_placement_type nh7{black_knight, h7};
constexpr figurine_placement_type na6{black_knight, a6};
constexpr figurine_placement_type nb6{black_knight, b6};
constexpr figurine_placement_type nc6{black_knight, c6};
constexpr figurine_placement_type nd6{black_knight, d6};
constexpr figurine_placement_type ne6{black_knight, e6};
constexpr figurine_placement_type nf6{black_knight, f6};
constexpr figurine_placement_type ng6{black_knight, g6};
constexpr figurine_placement_type nh6{black_knight, h6};
constexpr figurine_placement_type na5{black_knight, a5};
constexpr figurine_placement_type nb5{black_knight, b5};
constexpr figurine_placement_type nc5{black_knight, c5};
constexpr figurine_placement_type nd5{black_knight, d5};
constexpr figurine_placement_type ne5{black_knight, e5};
constexpr figurine_placement_type nf5{black_knight, f5};
constexpr figurine_placement_type ng5{black_knight, g5};
constexpr figurine_placement_type nh5{black_knight, h5};
constexpr figurine_placement_type na4{black_knight, a4};
constexpr figurine_placement_type nb4{black_knight, b4};
constexpr figurine_placement_type nc4{black_knight, c4};
constexpr figurine_placement_type nd4{black_knight, d4};
constexpr figurine_placement_type ne4{black_knight, e4};
constexpr figurine_placement_type nf4{black_knight, f4};
constexpr figurine_placement_type ng4{black_knight, g4};
constexpr figurine_placement_type nh4{black_knight, h4};
constexpr figurine_placement_type na3{black_knight, a3};
constexpr figurine_placement_type nb3{black_knight, b3};
constexpr figurine_placement_type nc3{black_knight, c3};
constexpr figurine_placement_type nd3{black_knight, d3};
constexpr figurine_placement_type ne3{black_knight, e3};
constexpr figurine_placement_type nf3{black_knight, f3};
constexpr figurine_placement_type ng3{black_knight, g3};
constexpr figurine_placement_type nh3{black_knight, h3};
constexpr figurine_placement_type na2{black_knight, a2};
constexpr figurine_placement_type nb2{black_knight, b2};
constexpr figurine_placement_type nc2{black_knight, c2};
constexpr figurine_placement_type nd2{black_knight, d2};
constexpr figurine_placement_type ne2{black_knight, e2};
constexpr figurine_placement_type nf2{black_knight, f2};
constexpr figurine_placement_type ng2{black_knight, g2};
constexpr figurine_placement_type nh2{black_knight, h2};
constexpr figurine_placement_type na1{black_knight, a1};
constexpr figurine_placement_type nb1{black_knight, b1};
constexpr figurine_placement_type nc1{black_knight, c1};
constexpr figurine_placement_type nd1{black_knight, d1};
constexpr figurine_placement_type ne1{black_knight, e1};
constexpr figurine_placement_type nf1{black_knight, f1};
constexpr figurine_placement_type ng1{black_knight, g1};
constexpr figurine_placement_type nh1{black_knight, h1};
constexpr figurine_placement_type Pa8{white_pawn, a8};
constexpr figurine_placement_type Pb8{white_pawn, b8};
constexpr figurine_placement_type Pc8{white_pawn, c8};
constexpr figurine_placement_type Pd8{white_pawn, d8};
constexpr figurine_placement_type Pe8{white_pawn, e8};
constexpr figurine_placement_type Pf8{white_pawn, f8};
constexpr figurine_placement_type Pg8{white_pawn, g8};
constexpr figurine_placement_type Ph8{white_pawn, h8};
constexpr figurine_placement_type Pa7{white_pawn, a7};
constexpr figurine_placement_type Pb7{white_pawn, b7};
constexpr figurine_placement_type Pc7{white_pawn, c7};
constexpr figurine_placement_type Pd7{white_pawn, d7};
constexpr figurine_placement_type Pe7{white_pawn, e7};
constexpr figurine_placement_type Pf7{white_pawn, f7};
constexpr figurine_placement_type Pg7{white_pawn, g7};
constexpr figurine_placement_type Ph7{white_pawn, h7};
constexpr figurine_placement_type Pa6{white_pawn, a6};
constexpr figurine_placement_type Pb6{white_pawn, b6};
constexpr figurine_placement_type Pc6{white_pawn, c6};
constexpr figurine_placement_type Pd6{white_pawn, d6};
constexpr figurine_placement_type Pe6{white_pawn, e6};
constexpr figurine_placement_type Pf6{white_pawn, f6};
constexpr figurine_placement_type Pg6{white_pawn, g6};
constexpr figurine_placement_type Ph6{white_pawn, h6};
constexpr figurine_placement_type Pa5{white_pawn, a5};
constexpr figurine_placement_type Pb5{white_pawn, b5};
constexpr figurine_placement_type Pc5{white_pawn, c5};
constexpr figurine_placement_type Pd5{white_pawn, d5};
constexpr figurine_placement_type Pe5{white_pawn, e5};
constexpr figurine_placement_type Pf5{white_pawn, f5};
constexpr figurine_placement_type Pg5{white_pawn, g5};
constexpr figurine_placement_type Ph5{white_pawn, h5};
constexpr figurine_placement_type Pa4{white_pawn, a4};
constexpr figurine_placement_type Pb4{white_pawn, b4};
constexpr figurine_placement_type Pc4{white_pawn, c4};
constexpr figurine_placement_type Pd4{white_pawn, d4};
constexpr figurine_placement_type Pe4{white_pawn, e4};
constexpr figurine_placement_type Pf4{white_pawn, f4};
constexpr figurine_placement_type Pg4{white_pawn, g4};
constexpr figurine_placement_type Ph4{white_pawn, h4};
constexpr figurine_placement_type Pa3{white_pawn, a3};
constexpr figurine_placement_type Pb3{white_pawn, b3};
constexpr figurine_placement_type Pc3{white_pawn, c3};
constexpr figurine_placement_type Pd3{white_pawn, d3};
constexpr figurine_placement_type Pe3{white_pawn, e3};
constexpr figurine_placement_type Pf3{white_pawn, f3};
constexpr figurine_placement_type Pg3{white_pawn, g3};
constexpr figurine_placement_type Ph3{white_pawn, h3};
constexpr figurine_placement_type Pa2{white_pawn, a2};
constexpr figurine_placement_type Pb2{white_pawn, b2};
constexpr figurine_placement_type Pc2{white_pawn, c2};
constexpr figurine_placement_type Pd2{white_pawn, d2};
constexpr figurine_placement_type Pe2{white_pawn, e2};
constexpr figurine_placement_type Pf2{white_pawn, f2};
constexpr figurine_placement_type Pg2{white_pawn, g2};
constexpr figurine_placement_type Ph2{white_pawn, h2};
constexpr figurine_placement_type Pa1{white_pawn, a1};
constexpr figurine_placement_type Pb1{white_pawn, b1};
constexpr figurine_placement_type Pc1{white_pawn, c1};
constexpr figurine_placement_type Pd1{white_pawn, d1};
constexpr figurine_placement_type Pe1{white_pawn, e1};
constexpr figurine_placement_type Pf1{white_pawn, f1};
constexpr figurine_placement_type Pg1{white_pawn, g1};
constexpr figurine_placement_type Ph1{white_pawn, h1};
constexpr figurine_placement_type pa8{black_pawn, a8};
constexpr figurine_placement_type pb8{black_pawn, b8};
constexpr figurine_placement_type pc8{black_pawn, c8};
constexpr figurine_placement_type pd8{black_pawn, d8};
constexpr figurine_placement_type pe8{black_pawn, e8};
constexpr figurine_placement_type pf8{black_pawn, f8};
constexpr figurine_placement_type pg8{black_pawn, g8};
constexpr figurine_placement_type ph8{black_pawn, h8};
constexpr figurine_placement_type pa7{black_pawn, a7};
constexpr figurine_placement_type pb7{black_pawn, b7};
constexpr figurine_placement_type pc7{black_pawn, c7};
constexpr figurine_placement_type pd7{black_pawn, d7};
constexpr figurine_placement_type pe7{black_pawn, e7};
constexpr figurine_placement_type pf7{black_pawn, f7};
constexpr figurine_placement_type pg7{black_pawn, g7};
constexpr figurine_placement_type ph7{black_pawn, h7};
constexpr figurine_placement_type pa6{black_pawn, a6};
constexpr figurine_placement_type pb6{black_pawn, b6};
constexpr figurine_placement_type pc6{black_pawn, c6};
constexpr figurine_placement_type pd6{black_pawn, d6};
constexpr figurine_placement_type pe6{black_pawn, e6};
constexpr figurine_placement_type pf6{black_pawn, f6};
constexpr figurine_placement_type pg6{black_pawn, g6};
constexpr figurine_placement_type ph6{black_pawn, h6};
constexpr figurine_placement_type pa5{black_pawn, a5};
constexpr figurine_placement_type pb5{black_pawn, b5};
constexpr figurine_placement_type pc5{black_pawn, c5};
constexpr figurine_placement_type pd5{black_pawn, d5};
constexpr figurine_placement_type pe5{black_pawn, e5};
constexpr figurine_placement_type pf5{black_pawn, f5};
constexpr figurine_placement_type pg5{black_pawn, g5};
constexpr figurine_placement_type ph5{black_pawn, h5};
constexpr figurine_placement_type pa4{black_pawn, a4};
constexpr figurine_placement_type pb4{black_pawn, b4};
constexpr figurine_placement_type pc4{black_pawn, c4};
constexpr figurine_placement_type pd4{black_pawn, d4};
constexpr figurine_placement_type pe4{black_pawn, e4};
constexpr figurine_placement_type pf4{black_pawn, f4};
constexpr figurine_placement_type pg4{black_pawn, g4};
constexpr figurine_placement_type ph4{black_pawn, h4};
constexpr figurine_placement_type pa3{black_pawn, a3};
constexpr figurine_placement_type pb3{black_pawn, b3};
constexpr figurine_placement_type pc3{black_pawn, c3};
constexpr figurine_placement_type pd3{black_pawn, d3};
constexpr figurine_placement_type pe3{black_pawn, e3};
constexpr figurine_placement_type pf3{black_pawn, f3};
constexpr figurine_placement_type pg3{black_pawn, g3};
constexpr figurine_placement_type ph3{black_pawn, h3};
constexpr figurine_placement_type pa2{black_pawn, a2};
constexpr figurine_placement_type pb2{black_pawn, b2};
constexpr figurine_placement_type pc2{black_pawn, c2};
constexpr figurine_placement_type pd2{black_pawn, d2};
constexpr figurine_placement_type pe2{black_pawn, e2};
constexpr figurine_placement_type pf2{black_pawn, f2};
constexpr figurine_placement_type pg2{black_pawn, g2};
constexpr figurine_placement_type ph2{black_pawn, h2};
constexpr figurine_placement_type pa1{black_pawn, a1};
constexpr figurine_placement_type pb1{black_pawn, b1};
constexpr figurine_placement_type pc1{black_pawn, c1};
constexpr figurine_placement_type pd1{black_pawn, d1};
constexpr figurine_placement_type pe1{black_pawn, e1};
constexpr figurine_placement_type pf1{black_pawn, f1};
constexpr figurine_placement_type pg1{black_pawn, g1};
constexpr figurine_placement_type ph1{black_pawn, h1};


} // namespace representation


namespace std {


template <> class hash<representation::figurine_placement_type> {
public:
    std::size_t operator()(
        representation::figurine_placement_type const & figurine_placement
    ) const {
        return hash<representation::coordinate_enum>()(figurine_placement.coordinate().as_enum())
         ^ hash<representation::figurine_enum>()(figurine_placement.figurine().as_enum());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_PLACEMENT_TYPE_HPP
