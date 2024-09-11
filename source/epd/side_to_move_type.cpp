#include "epd/side_to_move_type.hpp"


#include <ostream>


namespace epd {


std::optional<side_to_move_type>
side_to_move_type::create(char const side_to_move_char) {
    switch (side_to_move_char) {
    case black_to_move_char:
        return side_to_move_type(true);
    case white_to_move_char:
        return side_to_move_type(false);
    default:
        return std::nullopt;
    }
}


std::optional<side_to_move_type>
side_to_move_type::create(std::string_view const side_to_move) {
    if (1 != side_to_move.size()) {
        return std::nullopt;
    }
    return side_to_move_type::create(side_to_move[0]);
}


void side_to_move_type::flip() {
    the_player_of_the_black_pieces_is_next_to_move
     = not the_player_of_the_black_pieces_is_next_to_move;
}


std::ostream & operator<<(
    std::ostream & out,
    side_to_move_type const & side_to_move
) {
    return out.put(side_to_move.as_char());
}


} // namespace epd
