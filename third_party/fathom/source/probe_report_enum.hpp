#if !defined(PROBE_REPORT_ENUM_HPP)
#define PROBE_REPORT_ENUM_HPP


#include <array>
#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <type_traits>

#include "epd/operand_type.hpp"
#include "epd/operands_type.hpp"
#include "epd/operation_type.hpp"


enum class probe_report_enum : unsigned char {
    unknown,
    loss,
    blessed_loss,
    draw,
    cursed_win,
    win,
    invalid_position_description,
    illegal_position,
    redundant_position,
    limit
};

auto constexpr probe_report_enum_limit{
 std::underlying_type_t<probe_report_enum>(probe_report_enum::limit)};

std::array<char, probe_report_enum_limit> constexpr probe_report_chars = {
    'u', 'l', 'b', 'd', 'c', 'w', 'x', 'i', 'r',
};

std::array<std::string_view, probe_report_enum_limit> constexpr
probe_report_descriptions = {
    "unknown",
    "loss",
    "blessed_loss",
    "draw",
    "cursed_win",
    "win",
    "invalid",
    "illegal",
    "redundant",
};

std::array<std::string_view, probe_report_enum_limit> constexpr
wtm_pgn_header_game_results = {
    "*", "0-1", "1/2-1/2", "1/2-1/2", "1/2-1/2", "1-0", "*", "*", "*",
};

std::array<std::string_view, probe_report_enum_limit> constexpr
btm_pgn_header_game_results = {
    "*", "1-0", "1/2-1/2", "1/2-1/2", "1/2-1/2", "0-1", "*", "*", "*",
};

std::array<std::string_view, probe_report_enum_limit> constexpr
wtm_pgn_body_game_results = {
    "* {unknown}",
    "0-1",
    "1/2-1/2 {1/4-3/4}",
    "1/2-1/2",
    "1/2-1/2 {3/4-1/4}",
    "1-0",
    "* {invalid}",
    "* {illegal}",
    "* {redundant}",
};

std::array<std::string_view, probe_report_enum_limit> constexpr
btm_pgn_body_game_results = {
    "* {unknown}",
    "1-0",
    "1/2-1/2 {3/4-1/4}",
    "1/2-1/2",
    "1/2-1/2 {1/4-3/4}",
    "0-1",
    "* {invalid}",
    "* {illegal}",
    "* {redundant}",
};


constexpr bool valid(probe_report_enum const probe_report) {
    return probe_report < probe_report_enum::limit;
}


constexpr auto as_index(probe_report_enum const probe_report) {
    assert(valid(probe_report));
    return std::underlying_type_t<probe_report_enum>(probe_report);
}


constexpr char as_char(probe_report_enum const probe_report) {
    assert(valid(probe_report));
    return probe_report_chars[as_index(probe_report)];
}


constexpr std::string_view as_description(
    probe_report_enum const probe_report
) {
    assert(valid(probe_report));
    return probe_report_descriptions[as_index(probe_report)];
}


constexpr std::optional<probe_report_enum> to_probe_report_enum_opt(
    char const ch
) {
    for (std::size_t i{}; i != probe_report_enum_limit; ++i) {
        if (ch == probe_report_chars[i]) {
            return probe_report_enum(i);
        }
    }
    return std::nullopt;
}


constexpr std::optional<probe_report_enum> to_probe_report_enum_opt(
    std::string_view const description
) {
    for (std::size_t i{}; i != probe_report_enum_limit; ++i) {
        if (description == probe_report_descriptions[i]) {
            return probe_report_enum(i);
        }
    }
    return std::nullopt;
}


std::optional<probe_report_enum> to_probe_report_enum_opt(
    epd::operation_type const game_theoretic_value_operation);


constexpr std::string_view to_pgn_result(
    probe_report_enum const probe_report,
    bool const white_to_move,
    bool const pgn_header
) {
    assert(valid(probe_report));
    auto const index{as_index(probe_report)};
    if (white_to_move) {
        if (pgn_header) {
            return wtm_pgn_header_game_results[index];
        } else {
            return wtm_pgn_body_game_results[index];
        }
    }
    if (pgn_header) {
        return btm_pgn_header_game_results[index];
    } else {
        return btm_pgn_body_game_results[index];
    }
}


// Returns the equivalent probe report from the other player's perspective.
constexpr probe_report_enum flip(probe_report_enum const probe_report) {
    switch (probe_report) {
    case probe_report_enum::win:
        return probe_report_enum::loss;
    case probe_report_enum::loss:
        return probe_report_enum::win;
    case probe_report_enum::cursed_win:
        return probe_report_enum::blessed_loss;
    case probe_report_enum::blessed_loss:
        return probe_report_enum::cursed_win;
    case probe_report_enum::unknown:
    case probe_report_enum::draw:
    case probe_report_enum::invalid_position_description:
    case probe_report_enum::illegal_position:
    case probe_report_enum::redundant_position:
    case probe_report_enum::limit:
    default:
        return probe_report;
    }
}


std::ostream & operator<<(std::ostream & out, probe_report_enum probe_report);


#endif // PROBE_REPORT_ENUM_HPP
