#ifndef EGT_COMPARISON_BOARD_GENERATION_HELPERS_HPP
#define EGT_COMPARISON_BOARD_GENERATION_HELPERS_HPP


#include <string>
#include <vector>

#include "representation/material_signature_type.hpp"


std::vector<representation::material_signature_type> material_signatures(
    unsigned int highest_figurine_count);

unsigned int highest_distance_n_merge_plus_make_irredundant_processed(
    representation::material_signature_type const & signature);

unsigned int loops_processed(
    representation::material_signature_type const & signature);

bool trailing_expand_available(
    representation::material_signature_type const & signature);

bool trailing_expand_and_make_irredundant_available(
    representation::material_signature_type const & signature);

std::string ecai_2024_proc(
    representation::material_signature_type const & signature,
    unsigned int distance_n_merge_plus_make_irredundant_level_completed,
    bool loop_completed,
    bool post_expansion_make_irredundant_completed,
    bool post_reduce_completed);

unsigned int ecai_2024_distance_n_merge_plus_make_irredundant_level_to_use(
    representation::material_signature_type const & signature);

std::vector<std::string> ecai_2024_tllm_procs(
    representation::material_signature_type const & signature,
    unsigned int distance_n_merge_plus_make_irredundant_level_to_use);

std::string ecai_2024_select_tllm_proc(
    representation::material_signature_type const & signature,
    std::vector<std::string> const & tllm_procs);

std::vector<std::string> ecai_2024_mtbdd_concretization_algorithms();

std::string ecai_2024_select_mtbdd_concretization_algorithm(
    representation::material_signature_type const & signature,
    std::vector<std::string> const & concretization_algorithms);


#endif // EGT_COMPARISON_BOARD_GENERATION_HELPERS_HPP
