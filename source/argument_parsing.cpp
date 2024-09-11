#include "argument_parsing.hpp"


#include <cstdlib>
#include <iostream>


using representation::material_signature_type;


std::optional<bool> as_bool_opt(std::string_view const input) {
    if (("true" == input) or ("TRUE" == input) or ("1" == input)) {
        return true;
    }
    if (("false" == input) or ("FALSE" == input) or ("0" == input)) {
        return false;
    }
    return std::nullopt;
}


std::optional<bool> as_perspective_of_white_player_opt(
    std::string_view const input
) {
    if (("wpr" == input) or ("white_player" == input)) {
        return true;
    }
    if (("spr" == input) or ("side_to_move" == input)) {
        return false;
    }
    return std::nullopt;
}


std::vector<material_signature_type>
material_signatures_to_process(int argc, char *argv[]) {
    std::vector<material_signature_type> result;
    for (int i{1}; i != argc; ++i) {
        auto material_signature_opt{material_signature_type::create_opt(argv[i])};
        if (material_signature_opt) {
            result.emplace_back(*material_signature_opt);
        } else {
            std::cerr << "EE: " << argv[i]
             << " is not a valid material signature." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return result;
}


std::string remove_potential_prefix(
    std::string_view const input,
    std::string_view const potential_prefix
) {
    std::string result{input};
    if (0 == result.find(potential_prefix)) {
        result = result.substr(potential_prefix.size());
    }
    return result;
}
