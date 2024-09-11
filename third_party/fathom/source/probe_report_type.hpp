#if !defined(PROBE_REPORT_TYPE_HPP)
#define PROBE_REPORT_TYPE_HPP


#include <iosfwd>
#include <optional>
#include <string_view>

#include "epd/operation_type.hpp"

#include "probe_report_enum.hpp"


class probe_report_type {
public:
    probe_report_type & operator=(probe_report_type const &) = default;
    probe_report_type & operator=(probe_report_type &&) = default;
    constexpr probe_report_type(probe_report_type const &) = default;
    constexpr probe_report_type(probe_report_type &&) = default;
    ~probe_report_type() = default;

    constexpr explicit probe_report_type(
        probe_report_enum const probe_report = probe_report_enum::unknown
    ) : the_probe_report(probe_report) {
        assert(valid(probe_report));
    }
    constexpr explicit probe_report_type(unsigned int const probe_report)
      : the_probe_report(static_cast<probe_report_enum>(probe_report)) {
        assert(static_cast<std::size_t>(probe_report)
         <= probe_report_enum_limit);
        assert(valid(static_cast<probe_report_enum>(probe_report)));
    }

    constexpr std::optional<probe_report_type> create(char const ch) {
        std::optional<probe_report_enum> as_enum_opt{
         to_probe_report_enum_opt(ch)};
        if (not as_enum_opt) return std::nullopt;
        return probe_report_type(*as_enum_opt);
    }

    static std::optional<probe_report_type> create(
        epd::operation_type const game_theoretic_value_operation);

    constexpr probe_report_enum as_enum() const { return the_probe_report; }

    constexpr auto as_index() const {
        return ::as_index(the_probe_report);
    }

    constexpr char as_char() const {
        return ::as_char(the_probe_report);
    }

    constexpr std::string_view as_description() const {
        return ::as_description(the_probe_report);
    }

    constexpr std::string_view as_pgn_result(
        bool const white_to_move,
        bool const pgn_header
    ) const {
        return to_pgn_result(the_probe_report, white_to_move, pgn_header);
    }

    // Update the probe report to be from the other player's perspective.
    void flip() {
        the_probe_report = ::flip(the_probe_report);
    }

private:
    probe_report_enum the_probe_report;
};


constexpr std::optional<probe_report_type>
to_probe_report_type_opt(char const ch) {
    std::optional<probe_report_enum> probe_report_enum_opt{
     to_probe_report_enum_opt(ch)};
    if (std::nullopt == probe_report_enum_opt) {
        return std::nullopt;
    }
    return probe_report_type(*probe_report_enum_opt);
}


constexpr std::optional<probe_report_type>
to_probe_report_type_opt(std::string_view const view) {
    // If there's just one character in the view, then process it as such.
    if (1 == view.size()) {
        return to_probe_report_type_opt(view[0]);
    }
    std::optional<probe_report_enum> probe_report_enum_opt{
     to_probe_report_enum_opt(view)};
    if (std::nullopt == probe_report_enum_opt) {
        return std::nullopt;
    }
    return probe_report_type(*probe_report_enum_opt);
}


std::ostream & operator<<(std::ostream & out, probe_report_type probe_report);


constexpr bool operator==(
    probe_report_type const lhs,
    probe_report_type const rhs
) {
    return lhs.as_enum() == rhs.as_enum();
}


constexpr bool operator!=(
    probe_report_type const lhs,
    probe_report_type const rhs
) {
    return not (lhs == rhs);
}


constexpr probe_report_type blessed_loss{probe_report_enum::blessed_loss};
constexpr probe_report_type cursed_win{probe_report_enum::cursed_win};
constexpr probe_report_type draw{probe_report_enum::draw};
constexpr probe_report_type illegal{probe_report_enum::illegal_position};
constexpr probe_report_type invalid{
 probe_report_enum::invalid_position_description};
constexpr probe_report_type loss{probe_report_enum::loss};
constexpr probe_report_type redundant{probe_report_enum::redundant_position};
constexpr probe_report_type unknown{probe_report_enum::unknown};
constexpr probe_report_type win{probe_report_enum::win};


#endif // PROBE_REPORT_TYPE
