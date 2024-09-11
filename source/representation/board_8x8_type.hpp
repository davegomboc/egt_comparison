#if !defined(EGT_COMPARISON_REPRESENTATION_BOARD_8x8_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_BOARD_8x8_TYPE_HPP


#include <array>
#include <optional>
#include <string>
#include <string_view>

#include "epd/board_contents_type.hpp"

#include "representation/figurine_type.hpp"
#include "representation/figurine_placements_type.hpp"


namespace representation {


class board_8x8_type {
public:
    board_8x8_type() {}
    board_8x8_type(epd::board_contents_type const & board_contents);
    board_8x8_type(figurine_placements_type const & placements);

    static std::optional<board_8x8_type> create_opt(
        std::string_view board_contents_as_epd_fen_text);

    bool empty() const;
    figurine_placements_type as_placements() const;
    std::string as_renderable_string(bool use_utf8) const;

private:
    std::array<std::array<std::optional<figurine_type>, 8>, 8> the_board;
};


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_BOARD_8x8_TYPE_HPP
