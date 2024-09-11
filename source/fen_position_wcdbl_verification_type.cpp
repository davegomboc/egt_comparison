#include "fen_position_wcdbl_verification_type.hpp"


#include <iterator>


std::ostream & operator<<(
    std::ostream & data_out,
    fen_position_wcdbl_verification_type const & test
) {
    return data_out << "{\"" << test.fen_position << "\", "
     << test.expected_wpr.as_description() << "}";
}


std::vector<fen_position_wcdbl_verification_type> legal_wcdbl_test_positions() {
    return std::vector<fen_position_wcdbl_verification_type>{
        // K_vs_k
        fen_position_wcdbl_verification_type{
         "k7/8/8/8/8/8/8/7K w - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "3k4/8/8/8/8/4K3/8/8 b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "6K1/8/8/8/2k5/8/8/8 w - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/8/2k5/8/8/5K2/8/8 b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/8/3k4/8/4K3/8/8/8 w - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/8/8/2K5/8/8/6k1/8 b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/8/8/5k2/8/6K1/8/8 b - - 0 1", draw},

        // KB_vs_k
        fen_position_wcdbl_verification_type{
         "8/8/2B1K1k1/8/8/8/8/8 w - - 0 1", draw},

        // KN_vs_k
        fen_position_wcdbl_verification_type{
         "8/8/8/4K3/3n4/4k3/8/8 b - - 0 1", draw},

        // KP_vs_k
        fen_position_wcdbl_verification_type{
         "k7/P7/K7/8/8/8/8/8 w - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "k7/P7/K7/8/8/8/8/8 b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "4k3/4P3/4K3/8/8/8/8/8 w - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "4k3/4P3/4K3/8/8/8/8/8 b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/7p/K1k5/8/8/8/8/8 w - - 0 1", loss},
        fen_position_wcdbl_verification_type{
         "8/7p/K1k5/8/8/8/8/8 b - - 0 1", loss},
        fen_position_wcdbl_verification_type{
         "8/8/8/1P6/8/k7/8/1K6 w - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "8/8/8/1P6/8/k7/8/1K6 b - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "8/8/8/8/1P6/k7/8/1K6 w - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "8/8/8/8/1P6/k7/8/1K6 b - - 0 1", draw},

        // KQ_vs_k
        fen_position_wcdbl_verification_type{
         "K7/8/8/8/8/8/5Q2/7k w - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "K7/8/8/8/8/8/5Q2/7k b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "K7/8/8/8/8/8/6Q1/7k b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "7k/8/8/8/8/8/1q6/K7 w - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "7k/6q1/8/8/8/8/8/K7 w - - 0 1", loss},
        fen_position_wcdbl_verification_type{
         "8/8/2Qk1K2/8/8/8/8/8 b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/8/2q1K1k1/8/8/8/8/8 w - - 0 1", loss},
        fen_position_wcdbl_verification_type{
         "8/8/2Q1K1k1/8/8/8/8/8 w - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "8/8/2Q1K1k1/8/8/8/8/8 b - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "8/8/2Qk1K2/8/8/8/8/8 b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/8/8/8/8/5K2/6Q1/7k b - - 0 1", win},

        // KR_vs_k
        fen_position_wcdbl_verification_type{
         "7k/8/8/8/8/8/8/R6K w - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "8/8/8/K3k3/1r6/8/8/8 w - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/8/5r2/4k3/8/8/6K1/8 w - - 0 1", loss},
        fen_position_wcdbl_verification_type{
         "8/8/8/4k3/3R4/2K5/8/8 b - - 0 1", win},
        fen_position_wcdbl_verification_type{
         "8/8/8/8/2k4K/3R4/8/8 b - - 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/8/5K2/8/5k2/5r2/8/8 b - - 0 1", loss},

        // KN_vs_kp
        fen_position_wcdbl_verification_type{
         "8/4P3/8/6K1/1n6/8/3k4/8 b - - 0 1", win},

        // KP_vs_kp
        fen_position_wcdbl_verification_type{
         "8/8/1K6/4kPp1/8/8/8/8 w - g6 0 1", draw},
        fen_position_wcdbl_verification_type{
         "8/6k1/8/2pP4/8/8/8/K7 w - c6 0 1", win},
        fen_position_wcdbl_verification_type{
         "8/8/8/8/3Pp3/1K6/8/k7 b - d3 0 1", draw},
        fen_position_wcdbl_verification_type{
         "7k/8/8/8/4pP2/8/1K6/8 b - f3 0 1", loss},
        fen_position_wcdbl_verification_type{
         "5k2/8/8/6Pp/1K6/8/8/8 w - h6 0 1", draw},
        fen_position_wcdbl_verification_type{
         "1k6/8/8/7K/8/7p/5P2/8 w - - 0 1", loss},
    };
}


std::vector<fen_position_wcdbl_verification_type> illegal_wcdbl_test_positions() {
    return std::vector<fen_position_wcdbl_verification_type>{
        // K_vs_k
        fen_position_wcdbl_verification_type{
         "8/8/8/3k4/4K3/8/8/8 w - - 0 1", illegal},
        fen_position_wcdbl_verification_type{
         "8/8/8/3k4/4K3/8/8/8 b - - 0 1", illegal},
        fen_position_wcdbl_verification_type{
         "8/8/3k4/2K5/8/8/8/8 w - - 0 1", illegal},
        fen_position_wcdbl_verification_type{
         "8/8/3k4/2K5/8/8/8/8 b - - 0 1", illegal},

        // KQ_vs_k
        fen_position_wcdbl_verification_type{
         "8/8/2Q1k1K1/8/8/8/8/8 w - - 0 1", illegal},
        fen_position_wcdbl_verification_type{
         "8/8/2q1K1k1/8/8/8/8/8 b - - 0 1", illegal},
        fen_position_wcdbl_verification_type{
         "K7/8/8/8/8/8/6Q1/7k w - - 0 1", illegal},
        fen_position_wcdbl_verification_type{
         "8/8/8/8/8/5K2/6Q1/7k w - - 0 1", illegal},
    };
}


std::vector<fen_position_wcdbl_verification_type> wcdbl_test_positions() {
    std::vector<fen_position_wcdbl_verification_type> illegal_positions{
     illegal_wcdbl_test_positions()};
    std::vector<fen_position_wcdbl_verification_type> result{
     legal_wcdbl_test_positions()};
    result.insert(result.end(),
     std::make_move_iterator(illegal_positions.begin()),
     std::make_move_iterator(illegal_positions.end()));
    return result;
}
