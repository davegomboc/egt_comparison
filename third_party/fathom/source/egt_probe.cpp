#include <getopt.h>

#include <iostream>
#include <string>

#include "fathom_argv_processing.hpp"
#include "probe_report_type.hpp"
#include "probing.hpp"
#include "tb_position_type.hpp"

#include "tbprobe.h"


int main(int argc, char * * argv) {
    auto [display_comprehensive_output, prober]{
     shared_setup_for_mains(argc, argv)};
    std::string fen;
    tb_position_type tb_position;
    // Avoid issuance of -Wmaybe-uninitialzed.
    tb_position.white_to_move = false;
    while (true) {
        std::getline(std::cin, fen);
        if (not std::cin) break;
        probe_report_type probe_report{prober.probe(fen, tb_position)};
        if (display_comprehensive_output) {
            std::cout << probe_report.as_pgn_result(tb_position.white_to_move,
             false) << std::endl;
        } else {
            std::cout.put(probe_report.as_char());
        }
    }
    return EXIT_SUCCESS;
}
