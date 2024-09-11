#include "endgame_table_type.hpp"


#include <ostream>


#include "representation/canonicalization_enum.hpp"


using representation::canonicalization_enum;
using representation::egt_position_type;


endgame_table_type::endgame_table_type(
    canonicalization_enum const canonicalization_mode
) : the_canonicalization_mode(canonicalization_mode) {}


canonicalization_enum endgame_table_type::canonicalization_mode() const {
    return the_canonicalization_mode;
}


probe_report_type endgame_table_type::probe_after_canonicalizing(
    egt_position_type const & position
) const {
    egt_position_type const canonical_position{position.canonical_position(
     the_canonicalization_mode)};
    probe_report_type result{probe_directly_impl(canonical_position)};
    bool const side_to_move_changed{position.white_to_move()
     != canonical_position.white_to_move()};
    if (side_to_move_changed) {
        result.flip();
    }
    return result;
}


// This primarily exists so that clang++ will emit vtables for this class in
// this translation unit only, instead of all of them.  (See -Wweak-vtables
// documentation for details.)
void endgame_table_type::vtable_pinning_method() const {}


std::ostream & operator<<(std::ostream & out, endgame_table_type const &) {
    return out << "[endgame_table_type]";
}
