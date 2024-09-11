#include "position_sampling_type.hpp"


#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <tuple>

#include "query_egt.hpp"
#include "representation/coordinates_type.hpp"
#include "representation/figurines_type.hpp"
#include "representation/figurine_placements_type.hpp"
#include "representation/material_signature_type.hpp"


using representation::bitboards_type;
using representation::coordinate_type;
using representation::coordinates_type;
using representation::egt_position_type;
using representation::figurine_type;
using representation::figurine_colour_type;
using representation::figurines_type;
using representation::figurine_placements_type;
using representation::material_signature_type;
using representation::the_maximum_figurine_count;
using representation::the_minimum_figurine_count;


position_sampling_type::position_sampling_type()
  : the_coordinate_distribution(
     std::uniform_int_distribution<unsigned char>(0, 63)),
    the_non_king_figurine_distribution(
     std::uniform_int_distribution<unsigned char>(0, 9)),
    the_colour_to_move_distribution(
     std::uniform_int_distribution<unsigned char>(0, 1)) {}


figurine_colour_type
position_sampling_type::draw_colour_to_move_uniformly() {
    return figurine_colour_type(8 * the_colour_to_move_distribution(
     the_pseudorandom_number_generator));
}

coordinate_type position_sampling_type::draw_coordinate_uniformly() {
    return coordinate_type(the_coordinate_distribution(
     the_pseudorandom_number_generator));
}

coordinates_type
position_sampling_type::draw_coordinates_uniformly(
    std::size_t const coordinate_count
) {
    coordinates_type result{};
    for (std::size_t i{}; i != coordinate_count; ++i) {
        result.push_back(draw_coordinate_uniformly());
    }
    return result;
}

figurine_type position_sampling_type::draw_non_king_figurine_uniformly() {
    switch (the_non_king_figurine_distribution(
     the_pseudorandom_number_generator)) {
    case 0:
        return representation::black_queen;
    case 1:
        return representation::black_rook;
    case 2:
        return representation::black_bishop;
    case 3:
        return representation::black_knight;
    case 4:
        return representation::black_pawn;
    case 5:
        return representation::white_queen;
    case 6:
        return representation::white_rook;
    case 7:
        return representation::white_bishop;
    case 8:
        return representation::white_knight;
    case 9:
        return representation::white_pawn;
    default:
        assert(false);
        __builtin_unreachable();
    }
}

egt_position_type
position_sampling_type::draw_proposed_egt_entrance_position_uniformly(
    material_signature_type const & material_signature
) {
    // Establish the figurine placements.
    figurine_placements_type const placements(material_signature,
     draw_coordinates_uniformly(material_signature.size()));

    // Establish the egt_position_type.  Note that the decoding of illegal pawn
    // placements permits positions with en passant captures to be generated.
    egt_position_type result(draw_colour_to_move_uniformly(), placements,
     representation::no_ep);
    (void)result.decode_illegal_pawn_placement();

    // TODO: Support positions where castling is allowed.
    return result;
}

egt_position_type
position_sampling_type::draw_proposed_egt_entrance_position_uniformly(
    unsigned int const figurine_count
) {
    if ((figurine_count < 2) or (5 < figurine_count)) {
        std::cerr << "EE: Unsupported figurine_count.\n";
        std::exit(EXIT_FAILURE);
    }
    figurines_type figurines{representation::white_king,
     representation::black_king};
    while (figurine_count != figurines.size()) {
        figurines.push_back(draw_non_king_figurine_uniformly());
    }
    std::optional<material_signature_type> material_signature_opt(
     material_signature_type::create_opt(figurines));
    assert(material_signature_opt);
    return draw_proposed_egt_entrance_position_uniformly(
     *material_signature_opt);
}

std::pair<probe_report_type, egt_position_type>
position_sampling_type::draw_egt_entrance_position_uniformly(
    material_signature_type const & material_signature
) {
    while (true) {
        egt_position_type egt_position{
         draw_proposed_egt_entrance_position_uniformly(material_signature)};
        std::optional<probe_report_type> probe_report_opt{
         sampled_position_is_acceptable(egt_position)};
        if (probe_report_opt) {
            return std::make_pair(std::move(*probe_report_opt),
             std::move(egt_position));
        }
    }
}

std::pair<probe_report_type, egt_position_type>
position_sampling_type::draw_egt_entrance_position_uniformly(
    unsigned int const figurine_count
) {
    while (true) {
        egt_position_type egt_position{
         draw_proposed_egt_entrance_position_uniformly(figurine_count)};
        std::optional<probe_report_type> probe_report_opt{
         sampled_position_is_acceptable(egt_position)};
        if (probe_report_opt) {
            return std::make_pair(std::move(*probe_report_opt),
             std::move(egt_position));
        }
    }
}

std::optional<probe_report_type>
position_sampling_type::sampled_position_is_acceptable(
    egt_position_type const & egt_position
) {
    std::optional<tb_position_type> tb_position_opt{
     egt_position.as_tb_position_opt()};
    if (std::nullopt == tb_position_opt) {
        return std::nullopt;
    }

    probe_report_type const probe_report{query_egt_directly(*tb_position_opt)};
    switch (probe_report.as_enum()) {
    case probe_report_enum::loss:
    case probe_report_enum::blessed_loss:
    case probe_report_enum::draw:
    case probe_report_enum::cursed_win:
    case probe_report_enum::win:
        return probe_report;
    case probe_report_enum::illegal_position:
        return std::nullopt;
    case probe_report_enum::invalid_position_description:
        std::cerr << "EE: TB probe should not have returned invalid.\n";
        return std::nullopt;
    case probe_report_enum::redundant_position:
        std::cerr << "EE: TB probe should not have returned redundant.\n";
        return std::nullopt;
    case probe_report_enum::unknown:
        std::cerr << "EE: TB probe should not have returned unknown.\n";
        return std::nullopt;
    case probe_report_enum::limit:
    default:
        std::cerr << "EE: TB probe returned a completely bogus value!\n";
        return std::nullopt;
    }
}

epd::operations_type position_sampling_type::generate_epd_operations(
    probe_report_type const probe_report
) {
    assert(probe_report == draw or probe_report == win or probe_report == loss
     or probe_report == blessed_loss or probe_report == cursed_win);
    std::string temp{"GTV "};
    temp.push_back(probe_report.as_char());
    temp += "; hmvc 0; fmvn 1;";
    std::optional<epd::operations_type> operations_opt{
     epd::operations_type::create(temp)};
    assert(operations_opt);
    return *operations_opt;
}
