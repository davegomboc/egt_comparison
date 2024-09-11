#include "fathom.hpp"

#include <getopt.h>

#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string_view>

#include "tbprobe.h"

#include "fathom_argv_processing.hpp"
#include "probe_report_type.hpp"
#include "probing.hpp"
#include "tb_position_type.hpp"


/*
 * Print a pseudo-principal variation for the given position.
 */
static void print_PV(std::ostream & out, tb_position_type * pos) {
    tb_position_type temp{*pos};
    std::putchar('\n');
    bool first{true};
    bool check{};
    if (not pos->white_to_move) {
        first = false;
        out << pos->full_move_count << "...";
    }
    while (true) {
        unsigned int move{tb_probe_root(pos->white, pos->black, pos->kings,
         pos->queens, pos->rooks, pos->bishops, pos->knights, pos->pawns,
         pos->rule50, pos->castling, pos->en_passant, pos->white_to_move,
         nullptr)};
        if (move == TB_RESULT_FAILED) {
            out << "{TB probe failed}\n";
            break;
        }
        if (move == TB_RESULT_CHECKMATE) {
            out << "# " << loss.as_pgn_result(pos->white_to_move, false)
             << '\n';
            break;
        }
        if (check) out.put('+');
        if (move == TB_RESULT_STALEMATE) {
            out << draw.as_pgn_result(pos->white_to_move, false) << '\n';
        } else if (pos->rule50 >= 100) {
            // TODO: Use cursed_win or blessed_loss, as appropriate.
            out << draw.as_pgn_result(pos->white_to_move, false) << '\n';
            break;
        }

        char str[32];
        move_to_str(pos, move, str);
        if (not first) out.put(' ');
        first = false;
        if (pos->white_to_move) {
            out << pos->full_move_count << ". ";
        }
        out << str;
        do_move(pos, move);
        check = is_check(pos, true);
    }
    // restore to root position
    *pos = temp;
}

/*
 * Print a list of moves that match the WDL value.
 */
static void print_moves(
    std::ostream & out,
    tb_position_type & tb_position,
    std::array<unsigned int, TB_MAX_MOVES> const & per_root_move_results,
    unsigned int wdl
) {
    bool move_previously_emitted{};
    for (std::size_t i{}; per_root_move_results[i] != TB_RESULT_FAILED; ++i) {
        if (TB_GET_WDL(per_root_move_results[i]) != wdl) continue;
        if (move_previously_emitted) out << ", ";
        move_previously_emitted = true;
        char str[32];
        move_to_str(&tb_position, per_root_move_results[i], str);
        out << str;
    }
}

void display_fathom_output(
    std::string const & fen,
    tb_position_type & tb_position,
    probe_report_type const probe_report,
    unsigned int const tb_probe_root_result,
    std::array<unsigned int, TB_MAX_MOVES> const & per_root_move_results
) {
    switch (probe_report.as_enum()) {
    case probe_report_enum::loss:
    case probe_report_enum::blessed_loss:
    case probe_report_enum::draw:
    case probe_report_enum::cursed_win:
    case probe_report_enum::win:
        break;
    case probe_report_enum::unknown:
    case probe_report_enum::illegal_position:
    case probe_report_enum::invalid_position_description:
    case probe_report_enum::redundant_position:
        std::cerr << "EE: " << probe_report.as_description() << '\n';
        return;
    case probe_report_enum::limit:
    default:
        assert(false);
        std::cerr << "EE: Internal error: probe report has bogus value.\n";
	std::exit(EXIT_FAILURE);
    }

    std::cout << "[Event \"\"]\n"
     "[Site \"\"]\n"
     "[Date \"??\"]\n"
     "[Round \"-\"]\n"
     "[White \"Syzygy\"]\n"
     "[Black \"Syzygy\"]\n"
     "[Result \"" << probe_report.as_pgn_result(tb_position.white_to_move, true)
     << "\"]\n"
     "[FEN \"" << fen << "\"]\n"
     "[WCDBL \"" << probe_report.as_char() << "\"]\n"
     "[DTZ \"" << TB_GET_DTZ(tb_probe_root_result) << "\"]\n"
     "[WinningMoves \"";
    print_moves(std::cout, tb_position, per_root_move_results, TB_WIN);
    std::cout << "\"]\n[CursedWinningMoves \"";
    print_moves(std::cout, tb_position, per_root_move_results, TB_CURSED_WIN);
    std::cout << "\"]\n[DrawingMoves \"";
    print_moves(std::cout, tb_position, per_root_move_results, TB_DRAW);
    std::cout << "\"]\n[BlessedLosingMoves \"";
    print_moves(std::cout, tb_position, per_root_move_results, TB_BLESSED_LOSS);
    std::cout << "\"]\n[LosingMoves \"";
    print_moves(std::cout, tb_position, per_root_move_results, TB_LOSS);
    std::cout << "\"]\n";
    print_PV(std::cout, &tb_position);

    TbRootMoves moves;
    int dtz_result{tb_probe_root_dtz(tb_position.white, tb_position.black,
     tb_position.kings, tb_position.queens, tb_position.rooks,
     tb_position.bishops, tb_position.knights, tb_position.pawns,
     tb_position.rule50, tb_position.castling, tb_position.en_passant,
     tb_position.white_to_move, false, true, &moves)};
    if (0 == dtz_result) {
        std::cerr << "EE: DTZ probe failed.\n";
        return;
    }
    std::cout << moves.size << " moves returned from DTZ probe.\n";
    char str[20];
    for (unsigned int i{}; i < moves.size; ++i) {
        TbRootMove * m = &(moves.moves[i]);
        move_parts_to_str(&tb_position, TB_MOVE_FROM(m->move),
         TB_MOVE_TO(m->move), TB_MOVE_PROMOTES(m->move), str);
        std::cout << str << "\trank = " << std::setw(6) << m->tbRank
         << "\tscore = " << std::setw(6) << m->tbScore << '\n';
    }
    int wdl_result{tb_probe_root_wdl(tb_position.white, tb_position.black,
     tb_position.kings, tb_position.queens, tb_position.rooks,
     tb_position.bishops, tb_position.knights, tb_position.pawns,
     tb_position.rule50, tb_position.castling, tb_position.en_passant,
     tb_position.white_to_move, true, &moves)};
    if (0 == wdl_result) {
        std::cerr << "EE: WDL probe failed.\n";
        return;
    }
    std::cout << moves.size << " moves returned from WDL probe.\n";
    for (unsigned int i{}; i < moves.size; ++i) {
        TbRootMove * m = &(moves.moves[i]);
        move_parts_to_str(&tb_position, TB_MOVE_FROM(m->move),
         TB_MOVE_TO(m->move), TB_MOVE_PROMOTES(m->move), str);
        std::cout << str << "\trank = " << std::setw(6) << m->tbRank
         << "\tscore = " << std::setw(6) << m->tbScore << '\n';
    }
}

/*
 * Fathom's main().
 */
int main(int argc, char * * argv) {
    auto [display_comprehensive_output, prober]{
     shared_setup_for_mains(argc, argv)};
    if (optind != argc - 1) {
        print_help(std::cout, argv[0]);
        return EXIT_SUCCESS;
    }
    std::string const fen{argv[optind]};
    tb_position_type tb_position;
    unsigned int tb_probe_root_result{};
    std::array<unsigned int, TB_MAX_MOVES> per_move_root_results;
    probe_report_type probe_report{prober.probe(fen, tb_position,
     tb_probe_root_result, per_move_root_results)};

    if (display_comprehensive_output) {
        display_fathom_output(fen, tb_position, probe_report,
         tb_probe_root_result, per_move_root_results);
    } else {
        std::cout << probe_report.as_pgn_result(tb_position.white_to_move,
         false) << std::endl;
    }
    return EXIT_SUCCESS;
}
