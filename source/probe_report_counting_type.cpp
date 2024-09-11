#include "probe_report_counting_type.hpp"


#include <cassert>

#include "representation/figurine_colour_type.hpp"


probe_report_counting_type::probe_report_counting_type(
    std::vector<configuration_enum> const & desired_configurations
) {
    for (auto const configuration: desired_configurations) {
        assert(configuration < limit);
        the_counters_opt_configurations[configuration]
         = probe_report_counters_type{};
    }
}


bool probe_report_counting_type::tracking(
    configuration_enum const configuration
) const {
    assert(configuration < limit);
    return std::nullopt != the_counters_opt_configurations[configuration];
}


std::string probe_report_counting_type::key_name_prefix(
    configuration_enum const configuration
) const {
    std::string result{"count_"};
    if (not (configuration & 0b10)) {
        result += "non";
    }
    result += "canonicalized_";
    if (configuration & 0b01) {
        result += "white_player";
    } else {
        result += "side_to_move";
    }
    result += "_perspective_report";
    return result;
}


void probe_report_counting_type::count_processed_egt_position(
    std::optional<representation::egt_position_type> const &
     noncanonicalized_egt_position_opt,
    std::optional<probe_report_type> const
     spr_probe_report_of_noncanonicalized_egt_position_opt,
    std::optional<representation::egt_position_type> const &
     canonicalized_egt_position_opt,
    std::optional<probe_report_type> const
     spr_probe_report_of_canonicalized_egt_position_opt
) {
    assert(
     (not tracking(single_symmetry_spr) and
      not tracking(single_symmetry_wpr)) or
     (noncanonicalized_egt_position_opt and
      spr_probe_report_of_noncanonicalized_egt_position_opt));
    assert(
     (not tracking(all_symmetries_spr) and
      not tracking(all_symmetries_wpr)) or
     (canonicalized_egt_position_opt and
      spr_probe_report_of_canonicalized_egt_position_opt));

    std::optional<probe_report_type> wpr_probe_report_of_noncanonicalized_egt_position_opt;
    if (tracking(single_symmetry_spr) and (noncanonicalized_egt_position_opt)) {
        assert(spr_probe_report_of_noncanonicalized_egt_position_opt);
        wpr_probe_report_of_noncanonicalized_egt_position_opt
         = spr_probe_report_of_noncanonicalized_egt_position_opt;
        if (noncanonicalized_egt_position_opt->black_to_move()) {
            wpr_probe_report_of_noncanonicalized_egt_position_opt->flip();
        }
    }
    std::optional<probe_report_type> wpr_probe_report_of_canonicalized_egt_position_opt;
    if (tracking(all_symmetries_spr) and (canonicalized_egt_position_opt)) {
        assert(spr_probe_report_of_canonicalized_egt_position_opt);
        wpr_probe_report_of_canonicalized_egt_position_opt
         = spr_probe_report_of_canonicalized_egt_position_opt;
        if (canonicalized_egt_position_opt->black_to_move()) {
            wpr_probe_report_of_canonicalized_egt_position_opt->flip();
        }
    }

    using representation::no_ep;
    if (tracking(single_symmetry_spr)) {
        the_counters_opt_configurations[single_symmetry_spr]->increment(
         noncanonicalized_egt_position_opt->side_to_move(),
         no_ep == noncanonicalized_egt_position_opt->en_passant_target(),
         *spr_probe_report_of_noncanonicalized_egt_position_opt);
    }
    if (tracking(single_symmetry_wpr)) {
        the_counters_opt_configurations[single_symmetry_wpr]->increment(
         noncanonicalized_egt_position_opt->side_to_move(),
         no_ep == noncanonicalized_egt_position_opt->en_passant_target(),
         *wpr_probe_report_of_noncanonicalized_egt_position_opt);
    }
    if (tracking(all_symmetries_spr)) {
        the_counters_opt_configurations[all_symmetries_spr]->increment(
         canonicalized_egt_position_opt->side_to_move(),
         no_ep == canonicalized_egt_position_opt->en_passant_target(),
         *spr_probe_report_of_canonicalized_egt_position_opt);
    }
    if (tracking(all_symmetries_wpr)) {
        the_counters_opt_configurations[all_symmetries_wpr]->increment(
         canonicalized_egt_position_opt->side_to_move(),
         no_ep == canonicalized_egt_position_opt->en_passant_target(),
         *wpr_probe_report_of_canonicalized_egt_position_opt);
    }
}


std::optional<std::vector<std::pair<std::string, std::string>>>
probe_report_counting_type::as_key_value_pairs_opt(
    configuration_enum const configuration
) const {
    std::optional<probe_report_counters_type> const & counters_opt{
     the_counters_opt_configurations[configuration]};
    if (not counters_opt) {
        return std::nullopt;
    }
    std::vector<std::pair<std::string, std::string>> key_value_pairs;
    std::string const key_prefix{key_name_prefix(configuration)};
    std::size_t total_wtm_count{};
    std::size_t total_btm_count{};
    for (std::underlying_type_t<probe_report_enum> i{};
     i != ::probe_report_enum_limit; ++i) {
        std::string all_key{key_prefix + "_for_"};
        all_key += probe_report_descriptions[i];
        std::string const wtm_key{all_key + "_where_white_is_to_move"};
        std::string const btm_key{all_key + "_where_black_is_to_move"};
        std::size_t const wtm_count{counters_opt->value(
         representation::white.as_side_to_move(), false, probe_report_enum(i))
         + counters_opt->value(representation::white.as_side_to_move(), false,
         probe_report_enum(i))};
        total_wtm_count += wtm_count;
        std::size_t const btm_count{counters_opt->value(
         representation::black.as_side_to_move(), false, probe_report_enum(i))
         + counters_opt->value(representation::black.as_side_to_move(), true,
         probe_report_enum(i))};
        total_btm_count += btm_count;
        key_value_pairs.push_back(std::make_pair(wtm_key,
         std::to_string(wtm_count)));
        key_value_pairs.push_back(std::make_pair(btm_key,
         std::to_string(btm_count)));
        key_value_pairs.push_back(std::make_pair(all_key,
         std::to_string(btm_count + wtm_count)));
    }
    std::string const total_wtm_key{key_prefix + "_where_white_is_to_move"};
    std::string const total_btm_key{key_prefix + "_where_black_is_to_move"};
    key_value_pairs.push_back(std::make_pair(total_wtm_key,
     std::to_string(total_wtm_count)));
    key_value_pairs.push_back(std::make_pair(total_btm_key,
     std::to_string(total_btm_count)));
    key_value_pairs.push_back(std::make_pair(key_prefix,
     std::to_string(total_btm_count + total_wtm_count)));
    return key_value_pairs;
}


std::vector<std::pair<std::string, std::string>>
probe_report_counting_type::as_key_value_pairs() const {
    std::vector<std::pair<std::string, std::string>> result;
    for (unsigned char i{}; i != limit; ++i) {
        auto key_value_pairs_opt{as_key_value_pairs_opt(configuration_enum(i))};
        if (key_value_pairs_opt) {
            result.insert(result.end(), key_value_pairs_opt->cbegin(),
             key_value_pairs_opt->cend());
        }
    }
    return result;
}


void probe_report_counting_type::dump(
    std::ostream & out,
    configuration_enum const configuration
) const {
    std::optional<probe_report_counters_type> const & counters_opt{
     the_counters_opt_configurations[configuration]};
    assert(counters_opt);

    if (configuration & 0b10) {
        if (configuration & 0b01) {
            out << "II: Canonicalized, using probe reports from "
             "white's perspective.\n";
        } else {
            out << "II: Canonicalized, using probe reports from "
             "the side to move's perspective.\n";
        }
    } else {
        if (configuration & 0b01) {
            out << "II: Noncanonicalized, using probe reports from "
             "white's perspective.\n";
        } else {
            out << "II: Noncanonicalized, using probe reports from "
             "the side to move's perspective.\n";
        }
    }
    counters_opt->dump(out);
}


void probe_report_counting_type::dump(std::ostream & out) const {
    for (unsigned char i{}; i != limit; ++i) {
        configuration_enum const configuration{i};
        if (tracking(configuration)) {
            dump(out, configuration);
        }
    }
}
