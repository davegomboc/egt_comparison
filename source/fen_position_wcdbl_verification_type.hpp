#if !defined (EGT_COMPARISON_FEN_POSITION_WCDBL_VERIFICATION_TYPE_HPP)
#define EGT_COMPARISON_FEN_POSITION_WCDBL_VERIFICATION_TYPE_HPP


#include <ostream>
#include <string_view>
#include <vector>

#include "probe_report_type.hpp"


// FEN = Forsyth-Edwards Notation (see the Portable Game Notation standard).
// WCDBL = win / cursed win / draw / blessed loss / loss
struct fen_position_wcdbl_verification_type {
    std::string_view fen_position;
    // Probe reports are specified according to the white player's perspective.
    probe_report_type expected_wpr;
};


std::ostream & operator<<(
    std::ostream & data_out,
    fen_position_wcdbl_verification_type const & test
);


std::vector<fen_position_wcdbl_verification_type>
 illegal_wcdbl_test_positions();
std::vector<fen_position_wcdbl_verification_type> legal_wcdbl_test_positions();
std::vector<fen_position_wcdbl_verification_type> wcdbl_test_positions();


#endif // EGT_COMPARISON_FEN_POSITION_WCDBL_VERIFICATION_TYPE_HPP
