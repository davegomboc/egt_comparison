#ifndef EGT_COMPARISON_MAKE_DECISION_DAG_HPP
#define EGT_COMPARISON_MAKE_DECISION_DAG_HPP


#include <cstdlib>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "cube_list.hpp"
#include "load_and_save.hpp"


using ddag_type = ddag<32, 8, 27>;


template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, std::size_t INDEXBITS>
auto make_decision_dag(cube_list_tt<MAXINBITS, MAXOUTBITS> const & cl) {
    return ddag<MAXINBITS, MAXOUTBITS, INDEXBITS>(cl);
}


/*
   If the serialized decision DAG is already present on disk, loads it.
   Otherwise, generates it, saves it to disk, then loads it.

   Returns the requested decision DAG.
*/
template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, std::size_t INDEXBITS>
auto get_decision_dag(
    std::string_view const material_balance,
    std::optional<std::filesystem::path> const & archive_path_opt
     = std::nullopt,
    std::optional<std::filesystem::path> const & extraction_script_path_opt
     = std::nullopt
) {
    // Determine which directory should contain the requested decision DAG by
    // the time this routine exits successfully.
    std::optional<std::filesystem::path> decision_dags_dir_path_opt{
     decision_dags_directory_path_opt(material_balance)};
    if (not decision_dags_dir_path_opt) {
        std::cerr << "EE: Unable to find where decision DAGs could and should "
         "be persisted.\n";
        std::exit(EXIT_FAILURE);
    }

    // If it is not already present, construct and save the decision DAG.
    std::filesystem::path ddag_path(data_file_path(
     *decision_dags_dir_path_opt, data_category_enum::ddag, material_balance,
     ""));
    if (not std::filesystem::exists(ddag_path)) {
        auto data(load_chesspresso_F_plb_from_archive<MAXINBITS, MAXOUTBITS>(
         material_balance, 3, archive_path_opt, extraction_script_path_opt));
        ddag<MAXINBITS, MAXOUTBITS, INDEXBITS> dt(data);
        save_ddag(ddag_path, dt);
    }
    std::optional<ddag_type> ddag_opt(load_ddag_opt<32, 8, 27>(ddag_path));
    if (not ddag_opt) {
        std::cerr << "EE: Attempt to load from " << ddag_path
         << " was unsuccessful.\n";
        std::exit(EXIT_FAILURE);
    }
    return std::move(*ddag_opt);
}


#endif // EGT_COMPARISON_MAKE_DECISION_DAG_HPP
