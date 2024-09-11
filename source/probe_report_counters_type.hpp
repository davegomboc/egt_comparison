#if !defined(EGT_COMPARISON_PROBE_REPORT_COUNTERS_TYPE_HPP)
#define EGT_COMPARISON_PROBE_REPORT_COUNTERS_TYPE_HPP


#include <array>
#include <cstddef>
#include <iomanip>
#include <ostream>
#include <type_traits>

#include "epd/side_to_move_type.hpp"

#include "probe_report_enum.hpp"
#include "probe_report_type.hpp"


class probe_report_counters_type {
public:
    probe_report_counters_type & operator=(probe_report_counters_type const &)
     = default;
    probe_report_counters_type & operator=(probe_report_counters_type &&)
     = default;
    explicit probe_report_counters_type(probe_report_counters_type const &)
     = default;
    explicit probe_report_counters_type(probe_report_counters_type &&)
     = default;
    ~probe_report_counters_type() = default;

    probe_report_counters_type() {
        reset();
    }

    void reset() {
        for (std::size_t i{}; i != the_counters.size(); ++i) {
            for (std::size_t j{}; j != the_counters[i].size(); ++j) {
                the_counters[i][j].fill(0);
            }
        }
    }

    std::size_t value(
        epd::side_to_move_type const side,
        bool const en_passant_target_set,
        probe_report_enum const probe_report
    ) const {
        using index_type = std::underlying_type_t<probe_report_enum>;
        index_type const index{static_cast<index_type>(probe_report)};
        bool const black_to_move{side.black_is_to_move()};
        return the_counters[black_to_move][en_passant_target_set][index];
    }

    std::size_t & value(
        epd::side_to_move_type const side,
        bool const en_passant_target_set,
        probe_report_enum const probe_report
    ) {
        using index_type = std::underlying_type_t<probe_report_enum>;
        index_type const index{static_cast<index_type>(probe_report)};
        bool const black_to_move{side.black_is_to_move()};
        return the_counters[black_to_move][en_passant_target_set][index];
    }

    std::size_t value(
        epd::side_to_move_type const side,
        bool const en_passant_target_set,
        probe_report_type const probe_report
    ) const {
        return value(side, en_passant_target_set, probe_report.as_enum());
    }

    std::size_t & value(
        epd::side_to_move_type const side,
        bool const en_passant_target_set,
        probe_report_type const probe_report
    ) {
        return value(side, en_passant_target_set, probe_report.as_enum());
    }

    void increment(
        epd::side_to_move_type const side,
        bool const en_passant_target_set,
        probe_report_enum const probe_report
    ) {
        value(side, en_passant_target_set, probe_report) += 1;
    }

    void increment(
        epd::side_to_move_type const side,
        bool const en_passant_target_set,
        probe_report_type const probe_report
    ) {
        value(side, en_passant_target_set, probe_report) += 1;
    }

    void dump(std::ostream & out) const {
        out << "II: For positions where there is an en passant target.\n";
        out << "II: probe_report   wtm_count   btm_count total_count\n";
        std::size_t total_wtm_count{};
        std::size_t total_btm_count{};
        for (std::underlying_type_t<probe_report_enum> i{}; i != probe_report_enum_limit; ++i) {
            std::size_t const wtm_count{the_counters[0][0][i]};
            total_wtm_count += wtm_count;
            std::size_t const btm_count{the_counters[1][0][i]};
            total_btm_count += btm_count;
            out << "II: " << std::setw(12) << probe_report_descriptions[i]
             << std::setw(12) << wtm_count << std::setw(12) << btm_count
             << std::setw(12) << (btm_count + wtm_count) << '\n';
        }
        out << "II: " << std::setw(12) << "totals" << std::setw(12)
         << total_wtm_count << std::setw(12) << total_btm_count << std::setw(12)
         << (total_btm_count + total_wtm_count) << '\n';

        out << "II: For positions where there is no en passant target.\n";
        out << "II: probe_report   wtm_count   btm_count total_count\n";
        total_wtm_count = 0;
        total_btm_count = 0;
        for (std::underlying_type_t<probe_report_enum> i{}; i != probe_report_enum_limit; ++i) {
            std::size_t const wtm_count{the_counters[0][1][i]};
            total_wtm_count += wtm_count;
            std::size_t const btm_count{the_counters[1][1][i]};
            total_btm_count += btm_count;
            out << "II: " << std::setw(12) << probe_report_descriptions[i]
             << std::setw(12) << wtm_count << std::setw(12) << btm_count
             << std::setw(12) << (btm_count + wtm_count) << '\n';
        }
        out << "II: " << std::setw(12) << "totals" << std::setw(12)
         << total_wtm_count << std::setw(12) << total_btm_count << std::setw(12)
         << (total_btm_count + total_wtm_count) << '\n';

        out << "II: Ignoring whether there is or is no en passant target.\n";
        out << "II: probe_report   wtm_count   btm_count total_count\n";
        total_wtm_count = 0;
        total_btm_count = 0;
        for (std::underlying_type_t<probe_report_enum> i{}; i != probe_report_enum_limit; ++i) {
            std::size_t const wtm_count{the_counters[0][0][i] + the_counters[0][1][i]};
            total_wtm_count += wtm_count;
            std::size_t const btm_count{the_counters[1][0][i] + the_counters[1][1][i]};
            total_btm_count += btm_count;
            out << "II: " << std::setw(12) << probe_report_descriptions[i]
             << std::setw(12) << wtm_count << std::setw(12) << btm_count
             << std::setw(12) << (btm_count + wtm_count) << '\n';
        }
        out << "II: " << std::setw(12) << "totals" << std::setw(12)
         << total_wtm_count << std::setw(12) << total_btm_count << std::setw(12)
         << (total_btm_count + total_wtm_count) << '\n';
}

private:
    // First dimension: black_to_move() (0 <= x <= 1)
    // Second dimension: no_ep != en_passant_target (0 <= x <= 1)
    // Third dimension: probe_report_enum (0 <= x < probe_report_enum_limit)
    std::array<std::array<std::array<std::size_t,
     probe_report_enum_limit>, 2>, 2> the_counters;
};


#endif // EGT_COMPARISON_PROBE_REPORT_COUNTERS_TYPE_HPP
