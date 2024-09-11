#if !defined(EGT_COMPARISON_EPD_BOARD_CONTENTS_TYPE_HPP)
#define EGT_COMPARISON_EPD_BOARD_CONTENTS_TYPE_HPP


#include <array>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <vector>


namespace epd {


class board_contents_type {
public:
    board_contents_type & operator=(
     board_contents_type const &) = default;
    board_contents_type & operator=(board_contents_type &&) = default;
    board_contents_type(board_contents_type const &) = default;
    board_contents_type(board_contents_type &&) = default;
    ~board_contents_type() = default;

    board_contents_type();

    static std::optional<board_contents_type> create(
        std::string_view const board_contents);

    bool empty() const;

    // a8 = 0, b8 = 1, ..., h8 = 7, a7 = 8, ..., h1 = 63.
    char operator[](unsigned char) const;

    /*
     * Places a char (which must be one of "1kKqQrRbBnNpP") onto the specified
     * square on the board.
     */
    void set_square_content(unsigned char index, char content);

    std::string as_epd_fen(bool avoid_empty_square_compaction = false) const;

private:
    board_contents_type(std::array<char, 64> const board_contents);

    std::array<char, 64> the_board_contents;
};


std::ostream & operator<<(
    std::ostream & out,
    board_contents_type const & board_contents);


} // namespace epd


#endif // EGT_COMPARISON_EPD_BOARD_CONTENTS_TYPE_HPP
