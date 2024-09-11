#if !defined(EGT_COMPARISON_PROBE_REPORT_COUNTING_TYPE_HPP)
#define EGT_COMPARISON_PROBE_REPORT_COUNTING_TYPE_HPP


#include <array>
#include <optional>
#include <utility>
#include <vector>

#include "probe_report_type.hpp"
#include "probe_report_counters_type.hpp"

#include "representation/egt_position_type.hpp"


class probe_report_counting_type {
public:
    // spr means probe report is from perspective of side to move
    // wpr means probe report is from perspective of white player
    enum configuration_enum: unsigned char {
        single_symmetry_spr = 0,
        single_symmetry_wpr = 1,
        all_symmetries_spr = 2,
        all_symmetries_wpr = 3,
        limit = 4,
    };

    probe_report_counting_type(
        std::vector<configuration_enum> const & desired_configurations = {});

    // The probe reports provided for counting must be from the perspective of
    // the side to move, which is exactly what Fathom already returns.
    void count_processed_egt_position(
        std::optional<representation::egt_position_type> const &
         noncanonicalized_egt_position_opt,
        std::optional<probe_report_type> const
         spr_probe_report_of_noncanonicalized_egt_position_opt,
        std::optional<representation::egt_position_type> const &
         canonicalized_egt_position_opt,
        std::optional<probe_report_type> const
         spr_probe_report_of_canonicalized_egt_position_opt
    );

    std::vector<std::pair<std::string, std::string>> as_key_value_pairs() const;
    std::optional<std::vector<std::pair<std::string, std::string>>>
     as_key_value_pairs_opt(configuration_enum configuration) const;

    void dump(std::ostream & out) const;
    void dump(std::ostream & out, configuration_enum configuration) const;

    bool tracking(configuration_enum) const;

private:
    std::string key_name_prefix(configuration_enum configuration) const;

    std::array<std::optional<probe_report_counters_type>, 4>
     the_counters_opt_configurations;
};


#endif // EGT_COMPARISON_PROBE_REPORT_COUNTING_TYPE_HPP
