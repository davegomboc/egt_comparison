#include "probe_report_nybble_endgame_table_type.hpp"


#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <system_error>

#include <fcntl.h>

#include "representation/canonicalization_enum.hpp"
#include "representation/perspective_enum.hpp"
#include "hex_display.hpp"
#include "paths.hpp"
#include "primitive_text_operations.hpp"


using representation::canonicalization_enum;
using representation::egt_position_type;
using representation::material_signature_type;
using representation::perspective_enum;


probe_report_nybble_endgame_table_type::probe_report_nybble_endgame_table_type(
    canonicalization_enum const canonicalization_mode,
    std::size_t input_bit_pair_count,
    memory_mapped_file_type && mapped_endgame_table
) : endgame_table_type(canonicalization_mode),
    the_colour_to_move_bit_mask((1ull << (input_bit_pair_count)) >> 1),
    the_mapped_endgame_table(std::move(mapped_endgame_table)) {}


std::unique_ptr<probe_report_nybble_endgame_table_type>
probe_report_nybble_endgame_table_type::create_ptr(
    std::filesystem::path const & path,
    canonicalization_enum const canonicalization_mode,
    material_signature_type const & material_signature
) {
    std::optional<memory_mapped_file_type> egt_opt{
     memory_mapped_file_type::map_file_into_memory(path, O_LARGEFILE,
     O_RDONLY, PROT_READ, MAP_NORESERVE | MAP_SHARED_VALIDATE)};
    if (not egt_opt) {
        return nullptr;
    }
    probe_report_nybble_endgame_table_type egt(canonicalization_mode,
     material_signature.cube_schema().binary_variable_count(),
     std::move(*egt_opt));
    return std::make_unique<probe_report_nybble_endgame_table_type>(
     std::move(egt));
}


std::unique_ptr<probe_report_nybble_endgame_table_type>
probe_report_nybble_endgame_table_type::create_ptr(
    std::filesystem::path const & path
) {
    std::string const stem(path.stem());
    canonicalization_enum canonicalization_mode{
     canonicalization_enum::all_symmetries};
    std::string material_balance;
    if (starts_with(stem, "single_symmetry.")) {
        canonicalization_mode = canonicalization_enum::single_symmetry;
        material_balance = stem.substr(16 /* past the prefix */);
    } else {
        material_balance = stem;
    }
    std::optional<material_signature_type> material_signature_opt{
     material_signature_type::create_opt(material_balance)};
    if (not material_signature_opt) {
        return nullptr;
    }
    return create_ptr(path, canonicalization_mode, *material_signature_opt);
}


std::unique_ptr<probe_report_nybble_endgame_table_type>
probe_report_nybble_endgame_table_type::create_ptr(
    canonicalization_enum const canonicalization_mode,
    material_signature_type const & material_signature
) {
    bool const uncompressed{};
    std::filesystem::path const table_path(probe_report_nybble_path(
     canonicalization_mode, perspective_enum::white_player, material_signature,
     uncompressed));
    return create_ptr(table_path);
}


probe_report_type
probe_report_nybble_endgame_table_type::stored_wpr_probe_report(
    std::size_t const position_id
) const {
    std::size_t const index{position_id & ~the_colour_to_move_bit_mask};
    assert(not the_mapped_endgame_table.empty());
    std::byte const wpr_probe_reports_byte{the_mapped_endgame_table[index]};
    bool const use_high_nybble{index != position_id};
    std::byte const nybble_of_interest{use_high_nybble
     ? (wpr_probe_reports_byte >> 4)
     : (wpr_probe_reports_byte & std::byte(0b1111))};
    probe_report_type const result{
     static_cast<probe_report_enum>(nybble_of_interest)};
    return result;
}


// The probe_report_type returned is to be interpreted from White's perspective.
probe_report_type probe_report_nybble_endgame_table_type::stored_wpr_probe_report(
    egt_position_type const & egt_position
) const {
    std::size_t const position_id{egt_position.as_position_id()};
    return stored_wpr_probe_report(position_id);
}


// The probe_report_type returned is to be interpreted from White's perspective.
probe_report_type probe_report_nybble_endgame_table_type::probe_directly_impl(
    egt_position_type const & egt_position
) const {
    return stored_wpr_probe_report(egt_position);
}
