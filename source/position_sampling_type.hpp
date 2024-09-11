#if !defined(EGT_COMPARISON_POSITION_SAMPLING_TYPE_HPP)
#define EGT_COMPARISON_POSITION_SAMPLING_TYPE_HPP


#include <optional>
#include <random>
#include <utility>

#include "probe_report_type.hpp"
#include "tb_position_type.hpp"

#include "representation/egt_position_type.hpp"
#include "representation/enumerated_material_signatures_type.hpp"


class position_sampling_type {
public:
    position_sampling_type & operator=(position_sampling_type const &)
     = default;
    position_sampling_type & operator=(position_sampling_type &&) = default;
    position_sampling_type(position_sampling_type const &) = default;
    position_sampling_type(position_sampling_type &&) = default;
    ~position_sampling_type() = default;
    position_sampling_type();

    representation::figurine_colour_type draw_colour_to_move_uniformly();
    representation::coordinate_type draw_coordinate_uniformly();
    representation::coordinates_type draw_coordinates_uniformly(
        std::size_t coordinate_count);
    representation::figurine_type draw_non_king_figurine_uniformly();

    representation::egt_position_type
    draw_proposed_egt_entrance_position_uniformly(
        representation::material_signature_type const & material_signature);
    representation::egt_position_type
    draw_proposed_egt_entrance_position_uniformly(
        unsigned int figurine_count);

    std::pair<probe_report_type, representation::egt_position_type>
    draw_egt_entrance_position_uniformly(
        representation::material_signature_type const & material_signature);
    std::pair<probe_report_type, representation::egt_position_type>
    draw_egt_entrance_position_uniformly(
        unsigned int figurine_count);

    std::optional<probe_report_type> sampled_position_is_acceptable(
        representation::egt_position_type const & egt_position);

    epd::operations_type generate_epd_operations(
        probe_report_type const probe_report);

private:
    std::mt19937_64 the_pseudorandom_number_generator;
    std::uniform_int_distribution<unsigned char> the_coordinate_distribution;
    std::uniform_int_distribution<unsigned char>
     the_non_king_figurine_distribution;
    std::uniform_int_distribution<unsigned char> the_colour_to_move_distribution;
};


#endif // EGT_COMPARISON_POSITION_SAMPLING_TYPE_HPP
