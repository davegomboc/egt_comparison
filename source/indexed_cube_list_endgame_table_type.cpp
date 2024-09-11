#include "indexed_cube_list_endgame_table_type.hpp"


#include <cassert>

#include "representation/canonicalization_enum.hpp"
#include "representation/cover_enum.hpp"
#include "representation/cover_type.hpp"
#include "representation/cube_type.hpp"
#include "representation/logic_digits_type.hpp"
#include "bit_region_type.hpp"
#include "cube_binary_inputs.hpp"


using representation::canonicalization_enum;
using representation::cover_enum;
using representation::cube_type;
using representation::egt_position_type;
using representation::logic_digits_type;
using representation::material_signature_type;

using cube_binary_inputs_type = cube_binary_inputs_tt<32>;


indexed_cube_list_endgame_table_type::indexed_cube_list_endgame_table_type(
    indexed_cube_list_type && cubes
) : endgame_table_type(canonicalization_enum::all_symmetries),
    the_endgame_data(cubes) {}


std::unique_ptr<indexed_cube_list_endgame_table_type>
indexed_cube_list_endgame_table_type::create_ptr(std::istream & in) {
    std::optional<indexed_cube_list_type> indexed_cube_list_opt{
     indexed_cube_list_type::create_opt(in)};
    if (not indexed_cube_list_opt) {
        return nullptr;
    }
    return std::make_unique<indexed_cube_list_endgame_table_type>(
     std::move(*indexed_cube_list_opt));
}


std::unique_ptr<indexed_cube_list_endgame_table_type>
indexed_cube_list_endgame_table_type::create_ptr(
    std::filesystem::path const & path
) {
    std::optional<indexed_cube_list_type> indexed_cube_list_opt{
     indexed_cube_list_type::create_opt(path)};
    if (not indexed_cube_list_opt) {
        return nullptr;
    }
    return std::make_unique<indexed_cube_list_endgame_table_type>(
     std::move(*indexed_cube_list_opt));
}


std::unique_ptr<indexed_cube_list_endgame_table_type>
indexed_cube_list_endgame_table_type::create_ptr(
    material_signature_type const & material_signature,
    std::string_view const proc
) {
    std::optional<std::filesystem::path> const path_opt{
     cube_list_path_opt(material_signature.as_string(), proc, true)};
    if (not path_opt) {
        return nullptr;
    }
    return create_ptr(*path_opt);
}


std::unique_ptr<indexed_cube_list_endgame_table_type>
indexed_cube_list_endgame_table_type::create_ptr(
    material_signature_type const & material_signature
) {
    unsigned int const di_level{
     ecai_2024_distance_n_merge_plus_make_irredundant_level_to_use(
     material_signature)};
    std::vector<std::string> const tllm_procs{
     ecai_2024_tllm_procs(material_signature, di_level)};
    std::string const selected_proc{
     ecai_2024_select_tllm_proc(material_signature, tllm_procs)};
    return create_ptr(material_signature, selected_proc);
}


// Assumes third last bit is wwin, second last bit is wdrw, last bit is wlos.
// The probe_report_type returned is to be interpreted from White's perspective.
probe_report_type indexed_cube_list_endgame_table_type::probe_directly_impl(
    egt_position_type const & canonical_position
) const {
    logic_digits_type const input_as_pcn{canonical_position.as_logic_digits()};
    cube_type probe_cube(prepare_cube_for_cover(input_as_pcn,
     cover_enum::function, redundant));
    bit_region_type<cube_type::uint_type const> cregion{
     probe_cube.const_region()};
    assert(1 == cregion.uint_count());
    std::uint64_t probe_uint64{cregion.uint_ref(0)};
    cube_binary_inputs_type const direct_cube{
     cube_binary_inputs_type(probe_uint64)};

    std::optional<unsigned char> const lookup_result_opt{
     the_endgame_data.lookup(direct_cube)};
    if (lookup_result_opt) {
        switch (*lookup_result_opt) {
        case 0b100:
            return win;
        case 0b110:
            return cursed_win;
        case 0b010:
            return draw;
        case 0b011:
            return blessed_loss;
        case 0b001:
            return loss;
        default:
            break;
        }
    }
    return unknown;
}
