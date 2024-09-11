#include "board_8x8_type.hpp"


#include <cstdlib>

#include "representation/figurine_enum.hpp"
#include "representation/figurine_type.hpp"
#include "representation/figurine_placement_type.hpp"


namespace representation {


board_8x8_type::board_8x8_type(
    epd::board_contents_type const & board_contents
) : board_8x8_type() {
    for (unsigned char square{}; 64 != square; ++square) {
        unsigned char const file_index{static_cast<unsigned char>(square
         & static_cast<unsigned char>(file_enum::file_mask))};
        unsigned char const rank_bits{static_cast<unsigned char>(square
         & static_cast<unsigned char>(rank_enum::rank_mask))};
        unsigned char const rank_index{static_cast<unsigned char>(
         7 - static_cast<unsigned char>(rank_bits >> 3))};
        char const ch{board_contents[square]};
        switch (ch) {
        case 'p':
            the_board[file_index][rank_index] = black_pawn;
            break;
        case 'P':
            the_board[file_index][rank_index] = white_pawn;
            break;
        case 'r':
            the_board[file_index][rank_index] = black_rook;
            break;
        case 'R':
            the_board[file_index][rank_index] = white_rook;
            break;
        case 'b':
            the_board[file_index][rank_index] = black_bishop;
            break;
        case 'B':
            the_board[file_index][rank_index] = white_bishop;
            break;
        case 'n':
            the_board[file_index][rank_index] = black_knight;
            break;
        case 'N':
            the_board[file_index][rank_index] = white_knight;
            break;
        case 'q':
            the_board[file_index][rank_index] = black_queen;
            break;
        case 'Q':
            the_board[file_index][rank_index] = white_queen;
            break;
        case 'k':
            the_board[file_index][rank_index] = black_king;
            break;
        case 'K':
            the_board[file_index][rank_index] = white_king;
            break;
        case '1':
        default:
            break;
        }
    }
}


board_8x8_type::board_8x8_type(
    figurine_placements_type const & placements
) : board_8x8_type() {
    for (figurine_placement_type const & placement: placements) {
        coordinate_type const coordinate{placement.coordinate()};
        file_enum const file{just_file(coordinate.as_enum())};
        rank_enum const rank{just_rank(coordinate.as_enum())};
        unsigned int file_index{static_cast<unsigned int>(file)};
        unsigned int rank_index{7 - (static_cast<unsigned int>(rank) >> 3)};
        the_board[file_index][rank_index] = placement.figurine();
    }
}


std::optional<board_8x8_type>
board_8x8_type::create_opt(std::string_view board_contents_as_epd_fen_text) {
    std::optional<epd::board_contents_type> const board_contents_opt{
        epd::board_contents_type::create(board_contents_as_epd_fen_text)};
    if (not board_contents_opt) {
        return std::nullopt;
    }
    return board_8x8_type(*board_contents_opt);
}


bool board_8x8_type::empty() const {
    for (unsigned int rank_index{}; rank_index != 8; ++rank_index) {
        for (unsigned int file_index{}; file_index != 8; ++file_index) {
            if (the_board[file_index][rank_index]) {
                return false;
            }
        }
    }
    return true;
}


figurine_placements_type board_8x8_type::as_placements() const {
    std::vector<figurine_placement_type> placements;
    for (unsigned int rank_index{}; rank_index != 8; ++rank_index) {
        for (unsigned int file_index{}; file_index != 8; ++file_index) {
            std::optional<figurine_type> figurine_opt{
             the_board[file_index][rank_index]};
            if (figurine_opt) {
                file_enum const file{static_cast<unsigned char>(file_index)};
                rank_enum const rank{
                 static_cast<unsigned char>((7 - rank_index) << 3)};
                coordinate_type const coordinate(file, rank);
                placements.emplace_back(*figurine_opt, coordinate);
            }
        }
    }
    return figurine_placements_type(std::move(placements));
}


std::string board_8x8_type::as_renderable_string(bool const use_utf8) const {
    std::string result;
    for (unsigned int rank_index{7}; rank_index + 1 != 0; --rank_index) {
        result += "DD: ";
        for (unsigned int file_index{}; file_index != 8; ++file_index) {
            bool const light_square{
             static_cast<bool>((rank_index + file_index) % 2)};
            result += as_board_diagram_square(as_optional_enum(
             the_board[file_index][rank_index]), light_square, use_utf8);
        }
        result.push_back('\n');
    }
    return result;
}


} // namespace representation
