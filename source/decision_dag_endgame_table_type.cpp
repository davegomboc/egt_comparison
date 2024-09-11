#include "decision_dag_endgame_table_type.hpp"


#include <cassert>

#include "representation/canonicalization_enum.hpp"
#include "representation/cover_enum.hpp"
#include "representation/cover_type.hpp"
#include "representation/cube_type.hpp"
#include "representation/logic_digits_type.hpp"
#include "bit_region_type.hpp"
#include "cube_binary_inputs.hpp"
#include "paths.hpp"


using representation::canonicalization_enum;
using representation::cover_enum;
using representation::cube_type;
using representation::egt_position_type;
using representation::logic_digits_type;
using representation::material_signature_type;

using cube_binary_inputs_type = cube_binary_inputs_tt<32>;


decision_dag_endgame_table_type::decision_dag_endgame_table_type(
    decision_dag_type && decision_dag
) : endgame_table_type(canonicalization_enum::all_symmetries),
    the_endgame_data(decision_dag) {}


std::unique_ptr<decision_dag_endgame_table_type>
decision_dag_endgame_table_type::create_ptr(std::istream & in) {
    return std::make_unique<decision_dag_endgame_table_type>(
     decision_dag_type(in));
}


std::unique_ptr<decision_dag_endgame_table_type>
decision_dag_endgame_table_type::create_ptr(
    std::filesystem::path const & path
) {
    std::optional<decision_dag_type> decision_dag_opt{
     load_ddag_opt<32, 8, 27>(path)};
    if (not decision_dag_opt) {
        return nullptr;
    }
    return std::make_unique<decision_dag_endgame_table_type>(
     std::move(*decision_dag_opt));
}


std::unique_ptr<decision_dag_endgame_table_type>
decision_dag_endgame_table_type::create_ptr(
    material_signature_type const & material_signature
) {
    return create_ptr(decision_dag_path(material_signature, false));
}


// Assumes third last bit is wwin, second last bit is wdrw, last bit is wlos.
// The probe_report_type returned is to be interpreted from White's perspective.
probe_report_type decision_dag_endgame_table_type::probe_directly_impl(
    egt_position_type const & position
) const {
    logic_digits_type const input_as_pcn{position.as_logic_digits()};
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
