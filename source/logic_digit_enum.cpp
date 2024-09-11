#include "logic_digit_enum.hpp"


std::optional<logic_digit_enum> read_until_logic_digit_enum_opt(
    std::istream & in
) {
    while (true) {
        auto const peek_ch{in.peek()};
        switch (peek_ch) {
        case EOF:
            return std::nullopt;
        case ' ':
        case '\t':
        case '|':
            in.get();
            continue;
        default:
            std::optional<logic_digit_enum> logic_digit_opt{
             as_logic_digit_enum_opt(peek_ch)};
            if (logic_digit_opt) {
                in.get();
                return logic_digit_opt;
            }
            return std::nullopt;
        }
    }
}
