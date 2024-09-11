#include "fathom_argv_processing.hpp"


#include <getopt.h>

#include <cstdlib>
#include <iostream>
#include <string>


void print_help(std::ostream & out, char const * const prog) {
    out << "\nusage: " << prog
     << " [--help] [--path=PATH] [--test] FEN\n\n" << R"(
WHERE:
        FEN
                The FEN position to probe, delimited by double quotation marks
                (e.g., "8/8/2q1K1k1/8/8/8/8/8 b - - 0 1").
        --help
                Print this helpful message.
        --path=PATH
                Set the tablebase PATH string.
        --test
                Minimizes output: useful for scripts.

DESCRIPTION:
        This program is a stand-alone Syzygy tablebase probe tool.  The
        program takes as input a FEN string representation of a chess
        position and outputs a PGN representation of the probe result.

        In addition to the standard fields, the output PGN represents
        the following information:
        - Result: "1-0" (white wins), "1/2-1/2" (draw), or "0-1" (black
          wins)
        - Report: the single character that would be reported by the test mode.
        - The Win-Draw-Loss (WDL) value for the next move: "Win",
          "Draw", "Loss", "CursedWin" (win but 50-move draw) or
          "BlessedLoss" (loss but 50-move draw)
        - The Distance-To-Zero (DTZ) value (in plies) for the next move
        - WinningMoves: All winning moves.
        - CursedWinningMoves: All moves leading to theoretically won positions
          that are drawn in over-the-board play due to the 50-move rule.
        - DrawingMoves: All theoretically drawing moves.
        - BlessedLosingMoves: All moves leading to theoretically lost positions
          that are drawn in over-the-board play due to the 50-move rule.
        - LosingMoves: All losing moves.

        The PGN contains a "pseudo-principal variation" of Syzygy vs. Syzygy for
        the input position.  Each pseudo-PV move is rational with respect to
        preserving the WDL value.  The pseudo-PV does not represent the shortest
        mate nor the most natural human moves.
)" << std::endl;
}


static constexpr int option_help{0};
static constexpr int option_path{1};
static constexpr int option_test{2};


static constexpr option long_options[] =
{
    {"help", 0, nullptr, option_help},
    {"path", 1, nullptr, option_path},
    {"test", 0, nullptr, option_test},
    {nullptr, 0, nullptr, 0}
};


std::pair<bool, prober_type const>
shared_setup_for_mains(int argc, char * * argv) {
    std::string path;
    bool display_comprehensive_output{true};
    while (true) {
        int index;
        int opt{getopt_long(argc, argv, "", long_options, &index)};
        if (opt < 0) break;
        switch (opt) {
        case option_path:
            path = optarg;
            break;
        case option_test:
            display_comprehensive_output = false;
            break;
        case option_help:
        default:
            print_help(std::cout, argv[0]);
            std::exit(EXIT_SUCCESS);
        }
    }
    std::optional<prober_type const> prober_opt{
     prober_type::create_once_opt(path)};
    if (std::nullopt == prober_opt) {
        std::cerr << "EE: Unable to initialize prober.  (Bad path?)\n";
        std::exit(EXIT_FAILURE);
    }
    return std::make_pair(display_comprehensive_output, *prober_opt);
}
