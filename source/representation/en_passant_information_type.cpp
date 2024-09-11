#include "representation/en_passant_information_type.hpp"


#include "representation/coordinate_enum.hpp"


namespace representation {


en_passant_information_type::en_passant_information_type(
    coordinate_type const en_passant_target
) {
    coordinate_enum const coordinate(en_passant_target.as_enum());
    the_en_passant_target_file = just_file(coordinate);
    the_en_passant_target_rank = just_rank(coordinate);
    assert((rank_enum::rank_3 == the_en_passant_target_rank)
     or (rank_enum::rank_6 == the_en_passant_target_rank));
}


std::optional<en_passant_information_type> en_passant_information_type::create_opt(
    coordinate_type const en_passant_target_square
) {
    rank_enum const target_rank{just_rank(en_passant_target_square.as_enum())};
    if ((rank_enum::rank_3 != target_rank) and (rank_enum::rank_6 != target_rank)) {
        return std::nullopt;
    }
    return en_passant_information_type(en_passant_target_square);
}


} // namespace representation
