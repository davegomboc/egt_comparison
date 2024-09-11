#if !defined(EGT_COMPARISON_ARGUMENT_PARSING_HPP)
#define EGT_COMPARISON_ARGUMENT_PARSING_HPP


#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "representation/material_signature_type.hpp"


std::optional<bool> as_bool_opt(std::string_view);

std::optional<bool> as_perspective_of_white_player_opt(std::string_view);

std::vector<representation::material_signature_type>
material_signatures_to_process(int argc, char *argv[]);

std::string remove_potential_prefix(
    std::string_view input,
    std::string_view potential_prefix);


#endif // EGT_COMPARISON_ARGUMENT_PARSING_HPP
