#include "epd/board_contents_type.hpp"


#include <algorithm>
#include <cassert>
#include <cctype>


namespace epd {


board_contents_type::board_contents_type() {
    the_board_contents.fill('1');
}


board_contents_type::board_contents_type(std::array<char, 64> board_contents)
  : the_board_contents(board_contents) {}


std::optional<board_contents_type> board_contents_type::create(
    std::string_view board_contents
) {
    std::array<char, 64> board_chars;
    unsigned char board_contents_index{};
    unsigned char board_chars_index{};
    while (64 != board_chars_index) {
        char const ch{board_contents[board_contents_index]};
        switch (ch) {
        case '8':
            if (56 < board_chars_index) {
                return std::nullopt;
            }
            board_chars[board_chars_index++] = '1';
            [[fallthrough]];
        case '7':
            if (57 < board_chars_index) {
                return std::nullopt;
            }
            board_chars[board_chars_index++] = '1';
            [[fallthrough]];
        case '6':
            if (58 < board_chars_index) {
                return std::nullopt;
            }
            board_chars[board_chars_index++] = '1';
            [[fallthrough]];
        case '5':
            if (59 < board_chars_index) {
                return std::nullopt;
            }
            board_chars[board_chars_index++] = '1';
            [[fallthrough]];
        case '4':
            if (60 < board_chars_index) {
                return std::nullopt;
            }
            board_chars[board_chars_index++] = '1';
            [[fallthrough]];
        case '3':
            if (61 < board_chars_index) {
                return std::nullopt;
            }
            board_chars[board_chars_index++] = '1';
            [[fallthrough]];
        case '2':
            if (62 < board_chars_index) {
                return std::nullopt;
            }
            board_chars[board_chars_index++] = '1';
            [[fallthrough]];
        case '1':
            board_chars[board_chars_index++] = '1';
            board_contents.remove_prefix(1);
            break;
        case 'k':
        case 'q':
        case 'r':
        case 'b':
        case 'n':
        case 'p':
        case 'K':
        case 'Q':
        case 'R':
        case 'B':
        case 'N':
        case 'P':
            board_chars[board_chars_index++] = ch;
            board_contents.remove_prefix(1);
            break;
        case '/':
            if (0 != (board_chars_index & 0x07)) {
                return std::nullopt;
            }
            board_contents.remove_prefix(1);
            break;
        default:
            return std::nullopt;
        }
    }
    if (not board_contents.empty()) {
        return std::nullopt;
    }
    return board_contents_type(board_chars);
}


bool board_contents_type::empty() const {
    std::array<char, 64>::const_iterator iter{the_board_contents.begin()};
    while (iter != the_board_contents.end()) {
        if ('1' != *iter) {
            return false;
        }
        ++iter;
    }
    return true;
}


char board_contents_type::operator[](unsigned char index) const {
    assert(index < 64);
    return the_board_contents[index];
}


void board_contents_type::set_square_content(
    unsigned char const index,
    char const content
) {
    static std::string const valid_square_content{"1pPrRbBnNqQkK"};
    if ((index < 64) and (std::string::npos !=
     valid_square_content.find(content))) {
        the_board_contents[index] = content;
    }
}


std::string board_contents_type::as_epd_fen(
    bool const avoid_empty_square_compaction
) const {
    std::string result;
    for (unsigned char i{}; i != 64; ++i) {
        if ((0 != i) and (0 == (i & 0x07))) result += '/';
        if (not avoid_empty_square_compaction and (0 != i) and
         ('1' == the_board_contents[i]) and
         (std::isdigit(result.back()))) {
            ++*result.rbegin();
        } else {
            result += the_board_contents[i];
        }
    }
    return result;
}


std::ostream & operator<<(
    std::ostream & out,
    board_contents_type const & board_contents
) {
    return out << board_contents.as_epd_fen();
}


} // namespace epd
