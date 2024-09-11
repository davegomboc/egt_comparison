#include "epd/extended_position_description_type.hpp"


#include <ostream>


#include "primitive_text_operations.hpp"


namespace epd {


std::optional<board_contents_type>
extract_board_contents_opt(std::string_view & epd_text) {
    trim_whitespace_prefix(epd_text);
    std::size_t const board_contents_end{epd_text.find(' ')};
    if (std::string_view::npos == board_contents_end) {
        epd_text.remove_prefix(epd_text.size());
        return std::nullopt;
    }
    std::string_view const board_contents_text{
     epd_text.substr(0, board_contents_end)};
    epd_text.remove_prefix(1 + board_contents_end);
    return board_contents_type::create(board_contents_text);
}


std::optional<side_to_move_type>
extract_side_to_move_opt(std::string_view & remaining_epd_text) {
    trim_whitespace_prefix(remaining_epd_text);
    std::size_t const side_to_move_end{remaining_epd_text.find(' ')};
    if (std::string_view::npos == side_to_move_end) {
        remaining_epd_text.remove_prefix(remaining_epd_text.size());
        return std::nullopt;
    }
    std::string_view const side_to_move_text{
     remaining_epd_text.substr(0, side_to_move_end)};
    remaining_epd_text.remove_prefix(1 + side_to_move_end);
    return side_to_move_type::create(side_to_move_text);
}


std::optional<castling_rights_type>
extract_castling_rights_opt(std::string_view & remaining_epd_text) {
    trim_whitespace_prefix(remaining_epd_text);
    std::size_t const castling_rights_end{remaining_epd_text.find(' ')};
    if (std::string_view::npos == castling_rights_end) {
        remaining_epd_text.remove_prefix(remaining_epd_text.size());
        return std::nullopt;
    }
    std::string_view const castling_rights_text{
     remaining_epd_text.substr(0, castling_rights_end)};
    remaining_epd_text.remove_prefix(1 + castling_rights_end);
    return castling_rights_type::create(castling_rights_text);
}


std::optional<en_passant_target_type>
extract_en_passant_target_opt(std::string_view & remaining_epd_text) {
    trim_whitespace_prefix(remaining_epd_text);
    std::size_t en_passant_target_end{remaining_epd_text.find(' ')};
    if (std::string_view::npos == en_passant_target_end) {
        en_passant_target_end = remaining_epd_text.size();
    }
    std::string_view const en_passant_target_text{
     remaining_epd_text.substr(0, en_passant_target_end)};
    remaining_epd_text.remove_prefix(en_passant_target_end);
    return en_passant_target_type::create(en_passant_target_text);
}


extended_position_description_type::extended_position_description_type()
  : the_board_contents(),
    the_side_to_move(),
    the_castling_rights(),
    the_en_passant_target(),
    the_operations{} {}


extended_position_description_type::extended_position_description_type(
    board_contents_type const & board_contents,
    side_to_move_type const side_to_move,
    castling_rights_type const & castling_rights,
    en_passant_target_type const en_passant_target,
    operations_type const & operations
) : the_board_contents(board_contents),
    the_side_to_move(side_to_move),
    the_castling_rights(castling_rights),
    the_en_passant_target(en_passant_target),
    the_operations(operations) {}


std::optional<extended_position_description_type>
extended_position_description_type::create(std::string_view epd_text) {
    std::optional<board_contents_type> board_contents_opt{
     extract_board_contents_opt(epd_text)};
    if (not board_contents_opt) {
        return std::nullopt;
    }

    std::optional<side_to_move_type> side_to_move_opt{
     extract_side_to_move_opt(epd_text)};
    if (not side_to_move_opt) {
        return std::nullopt;
    }

    std::optional<castling_rights_type> castling_rights_opt{
     extract_castling_rights_opt(epd_text)};
    if (not castling_rights_opt) {
        return std::nullopt;
    }

    std::optional<en_passant_target_type> en_passant_target_opt{
     extract_en_passant_target_opt(epd_text)};
    if (not en_passant_target_opt) {
        return std::nullopt;
    }

    std::optional<operations_type> operations_opt;
    if (epd_text.empty()) {
        operations_opt = operations_type();
    } else {
        epd_text.remove_prefix(1);
        operations_opt = operations_type::create(epd_text);
        if (not operations_opt) {
            return std::nullopt;
        }
    }

    return extended_position_description_type(*board_contents_opt,
     *side_to_move_opt, *castling_rights_opt, *en_passant_target_opt,
     *operations_opt);
}


std::optional<extended_position_description_type>
extended_position_description_type::create_from_fen(std::string_view fen_text) {
    std::optional<board_contents_type> board_contents_opt{
     extract_board_contents_opt(fen_text)};
    if (not board_contents_opt) {
        return std::nullopt;
    }

    std::optional<side_to_move_type> side_to_move_opt{
     extract_side_to_move_opt(fen_text)};
    if (not side_to_move_opt) {
        return std::nullopt;
    }

    std::optional<castling_rights_type> castling_rights_opt{
     extract_castling_rights_opt(fen_text)};
    if (not castling_rights_opt) {
        return std::nullopt;
    }

    std::optional<en_passant_target_type> en_passant_target_opt{
     extract_en_passant_target_opt(fen_text)};
    if (not en_passant_target_opt) {
        return std::nullopt;
    }

    trim_whitespace_prefix(fen_text);
    if (fen_text.empty()) {
        return std::nullopt;
    }
    std::optional<operations_type> operations_opt;
    operations_opt = operations_type::create_from_fen(fen_text);
    if (not operations_opt) {
        return std::nullopt;
    }

    return extended_position_description_type(*board_contents_opt,
     *side_to_move_opt, *castling_rights_opt, *en_passant_target_opt,
     *operations_opt);
}


std::string extended_position_description_type::as_epd() const {
    std::string result{the_board_contents.as_epd_fen()};
    result += ' ';
    result += the_side_to_move.as_char();
    result += ' ';
    result += the_castling_rights.as_epd_fen();
    result += ' ';
    result += the_en_passant_target.as_epd_fen();
    if (not the_operations.empty()) {
        result += ' ';
        result += the_operations.as_epd();
    }
    return result;
}


std::optional<std::string>
extended_position_description_type::as_fen_opt() const {
    if (the_operations.empty()) {
        return std::nullopt;
    }
    std::optional<std::string> operations_opt{the_operations.as_fen_opt()};
    if (not operations_opt) {
        return std::nullopt;
    }
    std::string result{the_board_contents.as_epd_fen()};
    result += ' ';
    result += the_side_to_move.as_char();
    result += ' ';
    result += the_castling_rights.as_epd_fen();
    result += ' ';
    result += the_en_passant_target.as_epd_fen();
    result += ' ';
    result += *operations_opt;
    return result;
}


std::ostream & operator<<(
    std::ostream & out,
    extended_position_description_type const & extended_position_description
) {
    return out << extended_position_description.as_epd();
}


} // namespace epd
