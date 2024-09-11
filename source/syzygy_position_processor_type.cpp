#include "syzygy_position_processor_type.hpp"


#include <cstdlib>
#include <iostream>
#include <optional>
#include <type_traits>

#include "representation/bitboard_type.hpp"
#include "representation/canonicalization_enum.hpp"
#include "representation/coordinate_enum.hpp"
#include "representation/coordinates_type.hpp"
#include "representation/figurine_colour_type.hpp"
#include "representation/figurine_placement_type.hpp"
#include "representation/figurine_shape_type.hpp"
#include "representation/perspective_enum.hpp"

#include "paths.hpp"
#include "query_egt.hpp"


using representation::bitboard_type;
using representation::bitboards_type;
using representation::black;
using representation::canonicalization_enum;
using representation::coordinates_type;
using representation::egt_position_type;
using representation::figurine_colour_type;
using representation::figurine_placement_type;
using representation::figurine_placements_type;
using representation::figurine_shape_type;
using representation::material_signature_type;
using representation::pawn;
using representation::perspective_enum;
using representation::no_ep;
using representation::rank_enum;
using representation::white;


probe_report_type syzygy_position_processor_type::query_egt_position(
    egt_position_type const & egt_position
) {
    // Converting from egt_position_type to tb_position_type is relatively
    // expensive.  Therefore, we quickly check for a couple of cases to see if
    // we can avoid performing that processing.  Doing so increases the speed of
    // querying over all positions.

    // A position is invalid when it contains a placement that has two figurines
    // on the same square of the board.
    if (egt_position.placements().coordinates().overlapping()) {
        return invalid;
    }

    // A position is illegal when it contains a placement that has any pawn on
    // either the first or last rank.
    for (figurine_placement_type const & placement: egt_position.placements()) {
        if (pawn == placement.figurine().shape()) {
            rank_enum const rank{just_rank(placement.coordinate().as_enum())};
            if ((rank_enum::rank_8 == rank) or (rank_enum::rank_1 == rank)) {
                //std::cerr << "TT: Attempted to query the illegal egt position "
                // << egt_position << '\n';
                return illegal;
            }
        }
    }

    std::optional<tb_position_type> tb_position_opt{
     egt_position.as_tb_position_opt()};
    // This conversion should never fail, because we already filtered out
    // positions where figurines overlap above.
    assert(tb_position_opt);

    probe_report_type const result{query_egt_directly(*tb_position_opt)};
    // Querying the Syzygy tables via Fathom should never return redundant, and
    // at this point, it should also never return invalid.  However, illegal is
    // still a possible result.
    assert(redundant != result and invalid != result);
    return result;
}

void syzygy_position_processor_type::process_egt_position(
    canonicalization_enum const canonicalization,
    perspective_enum const perspective,
    material_signature_type const & material_signature,
    std::size_t const position_id
) {
    egt_position_type const noncanonicalized_egt_position(material_signature,
     position_id);
    probe_report_type spr_probe_report_of_noncanonicalized_egt_position{
     query_egt_position(noncanonicalized_egt_position)};
    assert(redundant != spr_probe_report_of_noncanonicalized_egt_position);

    std::optional<egt_position_type> canonicalized_egt_position_opt;
    std::optional<probe_report_type>
     spr_probe_report_of_canonicalized_egt_position_opt;
    if (canonicalization == canonicalization_enum::all_symmetries) {
        // A position is acanonical whenever a symmetry can be exploited to
        // avoid having to store a result by looking up another (symmetric)
        // position instead.
        //
        // Effective 2024-01-01, canonicalization occurs prior to checking for
        // invalid or (a subset of) illegal positions rather than afterwards.
        canonicalized_egt_position_opt = noncanonicalized_egt_position;
        canonicalized_egt_position_opt->canonicalize(
         canonicalization_enum::all_symmetries);
        if (noncanonicalized_egt_position == *canonicalized_egt_position_opt) {
            //std::clog << "TT: (" << material_signature << ", " << position_id
            // << ") is canonical.\n";
            spr_probe_report_of_canonicalized_egt_position_opt
             = spr_probe_report_of_noncanonicalized_egt_position;
        } else {
            spr_probe_report_of_canonicalized_egt_position_opt
             = query_egt_position(*canonicalized_egt_position_opt);
            spr_probe_report_of_noncanonicalized_egt_position = redundant;
            //std::clog << "TT: *canonicalized_egt_position_opt = "
            // << *canonicalized_egt_position_opt << ", spr = "
            // << spr_probe_report_of_canonicalized_egt_position_opt
            // ->as_description() << '\n';
            //std::clog << "TT:   noncanonicalized_egt_position = "
            // << noncanonicalized_egt_position << ", spr = "
            // << spr_probe_report_of_noncanonicalized_egt_position
            // .as_description() << '\n';
        }
    } else {
        assert(canonicalization == canonicalization_enum::single_symmetry);
        // TODO: Consider whether we want to set canonicalized_egt_position_opt
        // and spr_probe_report_of_canonicalized_egt_position_opt here.
    }

    probe_report_type finalized_probe_report{
     spr_probe_report_of_noncanonicalized_egt_position};
    if (perspective == perspective_enum::white_player) {
        if (canonicalization == canonicalization_enum::all_symmetries) {
            assert(canonicalized_egt_position_opt);
            if (canonicalized_egt_position_opt->black_to_move()) {
                finalized_probe_report.flip();
            }
        } else {
            if (noncanonicalized_egt_position.black_to_move()) {
                finalized_probe_report.flip();
            }
        }
    }

    record(material_signature, position_id, noncanonicalized_egt_position,
     finalized_probe_report);
    count_processed_egt_position(
     noncanonicalized_egt_position,
     spr_probe_report_of_noncanonicalized_egt_position,
     canonicalized_egt_position_opt,
     spr_probe_report_of_canonicalized_egt_position_opt);
}

void syzygy_position_processor_type::process_table(
    canonicalization_enum const canonicalization,
    perspective_enum const perspective,
    material_signature_type const & material_signature
) {
    if (canonicalization == canonicalization_enum::all_symmetries) {
        if (perspective == perspective_enum::white_player) {
            the_counters_opt = probe_report_counting_type({
             probe_report_counting_type::single_symmetry_spr,
             probe_report_counting_type::single_symmetry_wpr,
             probe_report_counting_type::all_symmetries_spr,
             probe_report_counting_type::all_symmetries_wpr});
        } else if (perspective == perspective_enum::side_to_move) {
            the_counters_opt = probe_report_counting_type({
             probe_report_counting_type::single_symmetry_spr,
             probe_report_counting_type::all_symmetries_spr});
        } else {
            std::cerr << "EE: Provided perspective was not understood.\n";
            std::exit(EXIT_FAILURE);
        }
    } else if (canonicalization == canonicalization_enum::single_symmetry) {
        if (perspective == perspective_enum::white_player) {
            the_counters_opt = probe_report_counting_type({
             probe_report_counting_type::single_symmetry_spr,
             probe_report_counting_type::all_symmetries_wpr});
        } else if (perspective == perspective_enum::side_to_move) {
            the_counters_opt = probe_report_counting_type({
             probe_report_counting_type::single_symmetry_spr});
        } else {
            std::cerr << "EE: Provided perspective was not understood.\n";
            std::exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "EE: Provided canonicalization was not understood.\n";
        std::exit(EXIT_FAILURE);
    }
    create_metadata_file(canonicalization, perspective, material_signature);
    std::size_t const wtm_position_count(
     material_signature.cube_schema().single_side_to_move_position_count());
    on_process_table_commencement(canonicalization, perspective,
     material_signature);
    auto processing_start_time{std::chrono::steady_clock::now()};
    for (std::size_t wtm_position_id{}; wtm_position_id != wtm_position_count;
     ++wtm_position_id) {
        process_egt_position(canonicalization, perspective, material_signature,
         wtm_position_id);
        std::size_t const btm_position_id(wtm_position_id + wtm_position_count);
        process_egt_position(canonicalization, perspective, material_signature,
         btm_position_id);
    }
    auto processing_stop_time{std::chrono::steady_clock::now()};
    the_processing_duration = std::chrono::duration_cast<std::chrono::seconds>(
     processing_stop_time - processing_start_time);
    record_syzygy_position_processor_computed_metadata();
    on_process_table_completion(canonicalization, perspective,
     material_signature);
}

void syzygy_position_processor_type::process_tables(
    canonicalization_enum const canonicalization,
    perspective_enum const perspective,
    std::vector<material_signature_type> const & material_signatures,
    bool const also_do_postprocessing
) {
    for (material_signature_type const & material_signature:
     material_signatures) {
        process_table(canonicalization, perspective, material_signature);
        if (also_do_postprocessing) {
            on_postprocess_table(canonicalization, perspective,
             material_signature);
        }
    }
}

void syzygy_position_processor_type::dump_probe_report_counters(
    std::ostream & out
) const {
    assert(the_counters_opt);
    the_counters_opt->dump(out);
}

std::error_code syzygy_position_processor_type::ensure_output_parent_path_exists(
    perspective_enum const perspective,
    material_signature_type const & material_signature
) const {
    std::filesystem::path path(output_path_prefix(perspective));
    path /= std::to_string(material_signature.size());
    std::error_code result;
    std::filesystem::create_directories(path, result);
    return result;
}

void syzygy_position_processor_type::count_processed_egt_position(
    std::optional<egt_position_type> const & noncanonicalized_egt_position_opt,
    std::optional<probe_report_type> const
     spr_probe_report_of_noncanonicalized_egt_position_opt,
    std::optional<egt_position_type> const & canonicalized_egt_position_opt,
    std::optional<probe_report_type> const
     spr_probe_report_of_canonicalized_egt_position_opt
) {
    assert(the_counters_opt);
    the_counters_opt->count_processed_egt_position(
     noncanonicalized_egt_position_opt,
     spr_probe_report_of_noncanonicalized_egt_position_opt,
     canonicalized_egt_position_opt,
     spr_probe_report_of_canonicalized_egt_position_opt);
}

void syzygy_position_processor_type::record_additional_metadata(
    std::vector<std::pair<std::string, std::string>> const & key_value_pairs
) const {
    assert(nullptr != the_metadata_ptr);
    for (auto const & [key, value]: key_value_pairs) {
        *the_metadata_ptr << key << " = " << value << '\n';
    }
    the_metadata_ptr->flush();
}

void syzygy_position_processor_type::create_metadata_file(
    canonicalization_enum const canonicalization,
    perspective_enum const perspective,
    material_signature_type const & material_signature
) {
    std::filesystem::path metadata_file_path(probe_report_nybble_path(canonicalization, perspective, material_signature, false));
    metadata_file_path /= std::to_string(material_signature.size());
    metadata_file_path /= "metadata.";
    metadata_file_path += name();
    metadata_file_path += ".";
    metadata_file_path += material_signature_infix(false, canonicalization,
     material_signature);
    metadata_file_path += ".txt";
    the_metadata_ptr = std::make_unique<std::ofstream>(metadata_file_path);
    std::vector<std::pair<std::string, std::string>> key_value_pairs;
    key_value_pairs.push_back(std::make_pair("name", name()));
    key_value_pairs.push_back(std::make_pair("canonicalization",
     std::string(representation::as_string_view(canonicalization))));
    key_value_pairs.push_back(std::make_pair("material_signature",
     material_signature.as_string()));
    record_additional_metadata(key_value_pairs);
}

void syzygy_position_processor_type::
record_syzygy_position_processor_computed_metadata() const {
    std::vector<std::pair<std::string, std::string>> key_value_pairs{
     the_counters_opt->as_key_value_pairs()};
    record_additional_metadata(key_value_pairs);
}

// This primarily exists so that clang++ will emit vtables for this class in
// this translation unit only, instead of all of them.  (See -Wweak-vtables
// documentation for details.)
void syzygy_position_processor_type::vtable_pinning_method() const {}
