#include "epd/castling_rights_type.hpp"


#include "primitive_bit_operations.hpp"


namespace epd {


std::optional<castling_rights_type> castling_rights_type::create(
    std::string_view const castling_rights_sv
) {
    if ("-" == castling_rights_sv) {
        return castling_rights_type(UINT8_C(0));
    }
    if (castling_rights_sv.empty() or (4 < castling_rights_sv.size())) {
        return std::nullopt;
    }
    std::uint8_t castling_rights{};
    for (std::size_t i{}; i != castling_rights_sv.size(); ++i) {
        char const ch{castling_rights_sv[i]};
        switch (ch) {
        case 'K':
            castling_rights |= white_OO;
            break;
        case 'Q':
            castling_rights |= white_OOO;
            break;
        case 'k':
            castling_rights |= black_OO;
            break;
        case 'q':
            castling_rights |= black_OOO;
            break;
        default:
            return std::nullopt;
        }
    }
    return castling_rights_type(castling_rights);
}


std::string castling_rights_type::as_epd_fen() const {
    std::string result;
    if (white_kingside()) result += 'K';
    if (white_queenside()) result += 'Q';
    if (black_kingside()) result += 'k';
    if (black_queenside()) result += 'q';
    if (result.empty()) result += '-';
    return result;
}


void castling_rights_type::black_kingside(bool const allowed) {
    if (allowed) {
        the_castling_rights |= black_OO;
    } else {
        the_castling_rights &= invert<std::uint8_t>(black_OO);
    }
}


void castling_rights_type::black_queenside(bool const allowed) {
    if (allowed) {
        the_castling_rights |= black_OOO;
    } else {
        the_castling_rights &= invert<std::uint8_t>(black_OOO);
    }
}


void castling_rights_type::white_kingside(bool const allowed) {
    if (allowed) {
        the_castling_rights |= white_OO;
    } else {
        the_castling_rights &= invert<std::uint8_t>(white_OO);
    }
}


void castling_rights_type::white_queenside(bool const allowed) {
    if (allowed) {
        the_castling_rights |= white_OOO;
    } else {
        the_castling_rights &= invert<std::uint8_t>(white_OOO);
    }
}


void castling_rights_type::flip_colours() {
    the_castling_rights =
     static_cast<std::uint8_t>((the_castling_rights & 0b0011) << 2)
     | static_cast<std::uint8_t>((the_castling_rights & 0b1100) >> 2);
}


std::ostream & operator<<(
    std::ostream & out,
    castling_rights_type const & castling_rights
) {
    return out << castling_rights.as_epd_fen();
}


} // namespace epd
