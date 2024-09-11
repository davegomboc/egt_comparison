#if !defined(EGT_COMPARISON_EPD_SIDE_TO_MOVE_TYPE_HPP)
#define EGT_COMPARISON_EPD_SIDE_TO_MOVE_TYPE_HPP


#include <iosfwd>
#include <optional>
#include <string_view>


namespace epd {


class side_to_move_type {
public:
    static constexpr char black_to_move_char = 'b';
    static constexpr char white_to_move_char = 'w';

    side_to_move_type & operator=(side_to_move_type const &) = default;
    side_to_move_type & operator=(side_to_move_type &&) = default;
    constexpr side_to_move_type(side_to_move_type const &) = default;
    constexpr side_to_move_type(side_to_move_type &&) = default;
    ~side_to_move_type() = default;

    constexpr explicit side_to_move_type(bool const black_has_the_move = false)
      : the_player_of_the_black_pieces_is_next_to_move{
         static_cast<unsigned char>(black_has_the_move)} {}

    static std::optional<side_to_move_type> create(char side_to_move_char);
    static std::optional<side_to_move_type> create(
        std::string_view const side_to_move);

    constexpr bool black_is_to_move() const {
        return not white_is_to_move();
    }
    constexpr bool white_is_to_move() const {
        return 0 == the_player_of_the_black_pieces_is_next_to_move;
    }

    constexpr char as_char() const {
        return black_is_to_move() ? black_to_move_char : white_to_move_char;
    }

    void flip();

private:
    unsigned char the_player_of_the_black_pieces_is_next_to_move;
};


std::ostream & operator<<(
    std::ostream & out,
    side_to_move_type const & side_to_move);


} // namespace epd


#endif // EGT_COMPARISON_EPD_SIDE_TO_MOVE_TYPE_HPP
