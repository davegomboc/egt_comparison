#if !defined(FATHOM_HPP)
#define FATHOM_HPP


#include <array>
#include <string>

#include "probe_report_type.hpp"
#include "tb_position_type.hpp"

#include "tbprobe.h"


/*
 * Displays Fathom's comprehensive output.
 */
void display_fathom_output(
    std::string const & fen,
    tb_position_type & tb_position,
    probe_report_type probe_report,
    unsigned int tb_probe_root_result,
    std::array<unsigned int, TB_MAX_MOVES> const & per_root_move_results);


#endif // FATHOM_HPP
