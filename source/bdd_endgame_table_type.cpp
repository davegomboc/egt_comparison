#include "bdd_endgame_table_type.hpp"


#include <cassert>

#include "bit_region_type.hpp"

#include "representation/canonicalization_enum.hpp"
#include "representation/cover_enum.hpp"
#include "representation/cover_type.hpp"
#include "representation/cube_type.hpp"
#include "representation/logic_digits_type.hpp"

#include "board_generation_helpers.hpp"
#include "paths.hpp"


using representation::canonicalization_enum;
using representation::cover_enum;
using representation::cube_type;
using representation::egt_position_type;
using representation::logic_digits_type;
using representation::material_signature_type;


bdd_endgame_table_type::bdd_endgame_table_type()
  : endgame_table_type(canonicalization_enum::all_symmetries) {}


std::unique_ptr<bdd_endgame_table_type>
bdd_endgame_table_type::create_ptr(std::istream & /*in*/) {
    // TODO: Implement.
    return nullptr;
}


std::unique_ptr<bdd_endgame_table_type>
bdd_endgame_table_type::create_ptr(std::filesystem::path const & path) {
    std::unique_ptr<bdd_endgame_table_type> result{
     std::unique_ptr<bdd_endgame_table_type>(new bdd_endgame_table_type())};
    result->the_bdd_endgame_table_root_node_id
     = result->the_bdd_endgame_table.add_nodes_from_file(path.c_str());
    result->the_bdd_endgame_table.done_building();
    return result;
}


std::unique_ptr<bdd_endgame_table_type>
bdd_endgame_table_type::create_ptr(
    material_signature_type const & material_signature,
    std::string_view const concretization_algorithm
) {
    return create_ptr(bdd_path(material_signature, concretization_algorithm,
     false));
}


std::unique_ptr<bdd_endgame_table_type>
bdd_endgame_table_type::create_ptr(
    material_signature_type const & material_signature
) {
    std::string const selected_mtbdd_concretization_algorithm{
     ecai_2024_select_mtbdd_concretization_algorithm(material_signature,
     ecai_2024_mtbdd_concretization_algorithms())};
    return create_ptr(material_signature,
     selected_mtbdd_concretization_algorithm);
}


// The probe_report_type returned is to be interpreted from White's perspective.
probe_report_type bdd_endgame_table_type::probe_directly_impl(
    egt_position_type const & position
) const {
    std::vector<unsigned char> bdd_probe_vector{
     position.as_mtbdd_uchar_vector_id()};
    unsigned int evaluate_result{the_bdd_endgame_table.evaluate(
     the_bdd_endgame_table_root_node_id, bdd_probe_vector)};
    switch (evaluate_result) {
    case 0b001:
        return win;
    case 0b011:
        return cursed_win;
    case 0b010:
        return draw;
    case 0b110:
        return blessed_loss;
    case 0b100:
        return loss;
    default:
        break;
    }
    return unknown;
}


// This primarily exists so that clang++ will emit vtables for this class in
// this translation unit only, instead of all of them.  (See -Wweak-vtables
// documentation for details.)
void bdd_endgame_table_type::vtable_pinning_method() const {}
