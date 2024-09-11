#include "probing.hpp"


#include <cstdlib>
#include <cstring>
#include <iostream>


unsigned int probing_init(std::string syzygy_path) {
    if (syzygy_path.empty()) {
        syzygy_path = std::getenv("SYZYGY_PATH");
    }
    tb_init(syzygy_path.c_str());
    return TB_LARGEST;
}

static probe_report_type egt_probe(
    tb_position_type const & tb_position,
    unsigned int & tb_probe_root_result,
    unsigned int * per_root_move_results
) {
    // (2) If each side does not have exactly one king, or any pawn is on the
    // first or the last rank, or the side not to move is in check, then we were
    // passed an illegal position.
    if (not each_side_has_exactly_one_king(&tb_position)
     or pawn_on_first_or_last_rank(&tb_position)
     or is_check(&tb_position, false)) {
        return illegal;
    }

    // (3) If there is too much material on the board, then the result will not
    // be known.
    if (TB_LARGEST < tb_pop_count(tb_position.white | tb_position.black)) {
        return unknown;
    }

    // (4) probe the TB:
    tb_probe_root_result = tb_probe_root(tb_position.white, tb_position.black,
     tb_position.kings, tb_position.queens, tb_position.rooks,
     tb_position.bishops, tb_position.knights, tb_position.pawns,
     tb_position.rule50, tb_position.castling, tb_position.en_passant,
     tb_position.white_to_move, per_root_move_results);

    // (5) Output:
    if (tb_probe_root_result == TB_RESULT_FAILED) {
        return unknown;
    } else if (tb_probe_root_result == TB_RESULT_CHECKMATE) {
        return loss;
    } else if (tb_probe_root_result == TB_RESULT_STALEMATE) {
        return draw;
    }
    unsigned int wdl{TB_GET_WDL(tb_probe_root_result)};
    switch (wdl) {
    case 0:
        return loss;
    case 1:
        return blessed_loss;
    case 2:
        return draw;
    case 3:
        return cursed_win;
    case 4:
        return win;
    default:
        std::cerr << "EE: Invalid wdl value " << wdl << " retrieved.\n";
        std::exit(EXIT_FAILURE);
    }
}

static probe_report_type egt_probe(
    std::string const & fen,
    tb_position_type & tb_position,
    unsigned int & tb_probe_root_result,
    unsigned int * per_root_move_results
) {
    // (1) parse the FEN:
    std::optional<tb_position_type> tb_position_opt{parse_fen(fen)};
    if (not tb_position_opt) {
        return invalid;
    }
    tb_position = *tb_position_opt;
    return egt_probe(tb_position, tb_probe_root_result, per_root_move_results);
}

probe_report_type egt_probe(tb_position_type const & tb_position) {
    unsigned int unused;
    return egt_probe(tb_position, unused, nullptr);
}

probe_report_type egt_probe(
    std::string const & fen,
    tb_position_type & tb_position
) {
    unsigned int unused;
    return egt_probe(fen, tb_position, unused, nullptr);
}

probe_report_type egt_probe(std::string const & fen) {
    tb_position_type tb_position;
    return egt_probe(fen, tb_position);
}

probe_report_type egt_probe(
    std::string const & fen,
    tb_position_type & tb_position,
    unsigned int & tb_probe_root_result,
    std::array<unsigned int, TB_MAX_MOVES> & per_root_move_results
) {
    return egt_probe(fen, tb_position, tb_probe_root_result,
     per_root_move_results.data());
}
