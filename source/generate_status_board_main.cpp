#include "generate_status_board_main.hpp"

#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <optional>
#include <vector>

#include "representation/enumerated_material_signatures_type.hpp"
#include "representation/material_signature_type.hpp"

#include "load_and_save.hpp"

#include "board_generation_helpers.hpp"
#include "egt_format_type.hpp"
#include "status_board_entry_type.hpp"


using representation::enumerated_material_signatures_type;
using representation::material_signature_type;


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cout << "usage: " << executable_name
     << " <minimum_figurine_count (2-5)>"
     " <maximum_figurine_count (minimum_figurine_count-5)>"
     << std::endl;
    std::exit(EXIT_FAILURE);
}


void emit_header(std::ostream & out) {
    out << "mat balance  form  proc/segment    path      disk (B) loads "
     "   memory (B)    node count  F cube count\n";
}

int main(int argc, char * argv[]) {
    unsigned int lowest_figurine_count{2};
    unsigned int highest_figurine_count{2};
    if (3 != argc) {
        print_usage(argv[0]);
    }
    int low_value{std::atoi(argv[1])};
    if ((low_value < 2) or (low_value > 5)) {
        print_usage(argv[0]);
    }
    lowest_figurine_count = static_cast<unsigned int>(low_value);
    int high_value{std::atoi(argv[2])};
    if ((high_value < 2) or (high_value > 5) or (high_value < low_value)) {
        print_usage(argv[0]);
    }
    highest_figurine_count = static_cast<unsigned int>(high_value);

    std::vector<material_signature_type> signatures;
    for (unsigned int figurine_count{lowest_figurine_count}; figurine_count <=
     highest_figurine_count; ++figurine_count) {
        std::vector<material_signature_type> signatures_of_figurine_count(
         material_signatures(figurine_count));
        signatures.reserve(signatures.size()
         + signatures_of_figurine_count.size());
        std::move(signatures_of_figurine_count.begin(),
         signatures_of_figurine_count.end(), std::back_inserter(signatures));
    }

    for (material_signature_type const & signature: signatures) {
        emit_header(std::cout);
        {
            status_board_entry_type const entry(
             std::make_unique<decision_dag_egt_format_type>(), signature);
            entry.emit(std::cout);
        }
        {
            unsigned int const distance_limit{
             ecai_2024_distance_n_merge_plus_make_irredundant_level_to_use(
             signature)};
            std::vector<std::string> const tllm_procs{
             ecai_2024_tllm_procs(signature, distance_limit)};
            std::string const selected_tllm_proc(
             ecai_2024_select_tllm_proc(signature, tllm_procs));
            status_board_entry_type const entry(
             std::make_unique<two_level_logic_minimization_egt_format_type>(),
             signature, selected_tllm_proc);
            entry.emit(std::cout);
        }
        {
            std::string const selected_mtbdd_concretization_algorithm(
             ecai_2024_select_mtbdd_concretization_algorithm(signature,
             ecai_2024_mtbdd_concretization_algorithms()));
            status_board_entry_type const entry(std::make_unique<
             multiterminal_binary_decision_diagram_egt_format_type>(),
             signature, selected_mtbdd_concretization_algorithm);
            entry.emit(std::cout);
        }
        {
            status_board_entry_type const entry(
             std::make_unique<syzygy_wcdbl_egt_format_type>(), signature);
            entry.emit(std::cout);
        }
        {
            status_board_entry_type const entry(
             std::make_unique<probe_report_nybble_egt_format_type>(), signature);
            entry.emit(std::cout);
        }
        std::cout << std::endl;
    }
}
