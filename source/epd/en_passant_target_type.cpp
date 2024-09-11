#include "epd/en_passant_target_type.hpp"


#include <cassert>


namespace epd {


std::optional<en_passant_target_type>
en_passant_target_type::create(
    char const file,
    char const rank
) {
    if (('a' > file) or ('h' < file)) {
        return std::nullopt;
    }
    if (('a' == file) and ('8' == rank)) {
        return en_passant_target_type();
    }
    if (('3' != rank) and ('6' != rank)) {
        return std::nullopt;
    }
    return en_passant_target_type(
     static_cast<unsigned char>(file - 'a' + 8 * ('8' - rank)));
}


std::optional<en_passant_target_type>
en_passant_target_type::create(
    std::string_view const en_passant_target
) {
    if ("-" == en_passant_target) {
        return en_passant_target_type();
    }
    if (2 != en_passant_target.size()) {
        return std::nullopt;
    }
    return en_passant_target_type::create(en_passant_target[0],
     en_passant_target[1]);
}


std::string en_passant_target_type::as_epd_fen() const {
    if (not specified()) {
        return "-";
    }
    std::string result(2, '\0');
    result[0] = file_as_char();
    result[1] = rank_as_char();
    return result;
}


void en_passant_target_type::clear() {
    the_en_passant_target = 0;
}


void en_passant_target_type::double_pawn_push(
    std::uint8_t const file,
    bool const played_by_black
) {
    if (file < 8) {
        the_en_passant_target = static_cast<std::uint8_t>(
         file + (played_by_black ? UINT8_C(16) : UINT8_C(40)));
    }
}


void en_passant_target_type::double_pawn_push(
    char const file,
    bool const played_by_black
) {
    double_pawn_push(static_cast<std::uint8_t>(file - 'a'), played_by_black);
}


std::ostream & operator<<(
    std::ostream & out,
    en_passant_target_type const & en_passant_target
) {
    return out << en_passant_target.as_epd_fen();
}


} // namespace epd
