#if !defined(PROBING_HPP)
#define PROBING_HPP


#include <array>
#include <filesystem>
#include <optional>
#include <string>

#include "probe_report_type.hpp"
#include "tb_position_type.hpp"

#include "tbprobe.h"


// Returns the value of TB_LARGEST (e.g., 5 for 5-figurine tables).
// The return value will be zero when initialization fails.
unsigned int probing_init(std::string syzygy_path = "");


/*
 * Probe the chess endgame table.  Fastest.
 */
probe_report_type egt_probe(tb_position_type const & tb_position);


/*
 * Probe the chess endgame table.  Exposes the tb_position_type set from the
 * specified FEN string, which is useful (amongst other reasons) if one needs
 * to know which player was to move.
 */
probe_report_type egt_probe(
    std::string const & fen,
    tb_position_type & tb_position);


/*
 * Probe the chess endgame table, given only a FEN string.
 */
probe_report_type egt_probe(std::string const & fen);


/*
 * Probe the chess endgame table.  Collects and returns extra information in
 * order to support Fathom's comprehensive output.
 */
probe_report_type egt_probe(
    std::string const & fen,
    tb_position_type & tb_position,
    unsigned int & tb_probe_root_result,
    std::array<unsigned int, TB_MAX_MOVES> & per_root_move_results);


#endif // PROBING_HPP
