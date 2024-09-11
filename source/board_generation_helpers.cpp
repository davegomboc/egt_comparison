#include "board_generation_helpers.hpp"


#include <cassert>
#include <cstddef>
#include <string>
#include <vector>

#include "representation/enumerated_material_signatures_type.hpp"
#include "representation/material_signature_type.hpp"

#include "status_board_entry_type.hpp"


using representation::enumerated_material_signatures_type;
using representation::material_signature_type;


std::vector<material_signature_type> material_signatures(
    unsigned int highest_figurine_count
) {
    enumerated_material_signatures_type ems(highest_figurine_count);
    return ems.material_signatures(highest_figurine_count);
}


unsigned int highest_distance_n_merge_plus_make_irredundant_processed(
    material_signature_type const & material_signature
) {
    std::string const balance{material_signature.as_string()};
    switch (material_signature.size()) {
    case 2:
        // K_vs_k.
        return 3;
    case 3:
        // All higher values have the same .clst file contents as the
        // value being returned.
        if (balance == "KP_vs_k") return 9;
        if (balance == "KQ_vs_k") return 11;
        if (balance == "KR_vs_k") return 8;
        // KB_vs_k and KN_vs_k remain.
        return 2;
    case 4:
        return 9;
    case 5:
        // We stopped at 5 due to computational resource constraints.
        return 5;
    default:
        return 0;
    }
}


unsigned int loops_processed(
    material_signature_type const & material_signature
) {
    switch (material_signature.size()) {
    case 2:
        return 1;
    case 3:
        return 1;
    case 4:
        return 1;
    case 5:
        return 1;
    default:
        return 0;
    }
}


bool trailing_expand_available(material_signature_type const &) {
    return false;
}


bool trailing_expand_and_make_irredundant_available(
    material_signature_type const &
) {
    return true;
}


std::string ecai_2024_proc(
    [[maybe_unused]] material_signature_type const & material_signature,
    unsigned int const distance_n_merge_plus_make_irredundant_level_completed,
    bool const loop_completed,
    bool const post_loop_expansion_and_make_irredundant_completed,
    bool const post_reduce_completed
) {
    assert(loop_completed
     or not post_loop_expansion_and_make_irredundant_completed);
    assert(post_loop_expansion_and_make_irredundant_completed
     or not post_reduce_completed);
    std::string result;
    if (0 == distance_n_merge_plus_make_irredundant_level_completed) {
        return result;
    }
    if (not loop_completed) {
        result = "D";
    }
    result += as_minimum_two_digit_string(
     distance_n_merge_plus_make_irredundant_level_completed);
    result += (loop_completed ? 'L' : 'I');
    if (post_reduce_completed) {
        result += "+Q";
    } else if (post_loop_expansion_and_make_irredundant_completed) {
        result += "+X+I";
    }
    return result;
}


unsigned int ecai_2024_distance_n_merge_plus_make_irredundant_level_to_use(
    material_signature_type const & material_signature
) {
    std::size_t const figurine_count{material_signature.as_figurines().size()};
    switch (figurine_count) {
    case 2:
        return 3;
    case 3:
        return 11;
    case 4:
        return 9;
    case 5:
        return 5;
    default:
        ;
    }
    return 0;
}


std::vector<std::string> ecai_2024_tllm_procs(
    material_signature_type const & material_signature,
    unsigned int const distance_n_merge_plus_make_irredundant_level_to_use
) {
    unsigned int const limit{distance_n_merge_plus_make_irredundant_level_to_use};
    std::vector<std::string> result;
    for (unsigned int level{}; level <= limit; ++level) {
        result.push_back(ecai_2024_proc(material_signature, level, false, false,
         false));
    }
    result.push_back(ecai_2024_proc(material_signature, limit, true, false,
     false));
    result.push_back(ecai_2024_proc(material_signature, limit, true, true,
     false));
    result.push_back(ecai_2024_proc(material_signature, limit, true, true,
     true));
    return result;
}


std::string ecai_2024_select_tllm_proc(
    material_signature_type const & material_signature,
    std::vector<std::string> const & tllm_procs
) {
    std::string result;
    std::uintmax_t bytes_on_disk{std::numeric_limits<std::uintmax_t>::max()};
    for (std::string const & proc: tllm_procs) {
        status_board_entry_type const icl_entry(std::make_unique<
         indexed_cube_list_egt_format_type>(), material_signature, proc);
        if (not icl_entry.path_exists() or not icl_entry.path_loads()) {
            continue;
        }
        status_board_entry_type clst_entry(std::make_unique<
         cube_list_egt_format_type>(), material_signature, proc);
        if (not clst_entry.path_exists() or not clst_entry.path_loads()) {
            continue;
        }
        if (clst_entry.bytes_on_disk() < bytes_on_disk) {
            bytes_on_disk = clst_entry.bytes_on_disk();
            result = proc;
        }
    }
    return result;
}


std::vector<std::string> ecai_2024_mtbdd_concretization_algorithms() {
    return {"tsm", "osm", "restr", "none"};
}


std::string ecai_2024_select_mtbdd_concretization_algorithm(
    representation::material_signature_type const & material_signature,
    std::vector<std::string> const & concretization_algorithms
) {
    std::string result;
    std::uintmax_t bytes_on_disk{std::numeric_limits<std::uintmax_t>::max()};
    for (std::string const & concretization_algorithm: concretization_algorithms) {
        status_board_entry_type const entry(
         std::make_unique<multiterminal_binary_decision_diagram_egt_format_type>(),
         material_signature, concretization_algorithm);
        if (not entry.path_exists() or not entry.path_loads()) {
            continue;
        }
        if (entry.bytes_on_disk() < bytes_on_disk) {
            bytes_on_disk = entry.bytes_on_disk();
            result = concretization_algorithm;
        }
    }
    return result;
}
