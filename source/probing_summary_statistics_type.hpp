#ifndef EGT_COMPARISON_PROBING_SUMMARY_STATISTICS_TYPE_HPP
#define EGT_COMPARISON_PROBING_SUMMARY_STATISTICS_TYPE_HPP


#include <chrono>
#include <cmath>
#include <iosfwd>
#include <limits>
#include <optional>


class probing_summary_statistics_type {
public:
    long double count() const {
        return the_duration_count;
    }
    long double sum_of_durations() const {
        return the_sum_of_durations;
    }
    long double sum_of_squared_durations() const {
        return the_sum_of_squared_durations;
    }
    std::optional<long double> duration_mean_opt() const;
    std::optional<long double> duration_sample_standard_deviation_opt() const;

    void include(
        std::chrono::duration<long double, std::nano> const probe_duration);

    void emit(std::ostream & out, bool sufficient_statistics_only) const;

private:
    long double the_duration_count{0.0L};
    long double the_sum_of_durations{0.0L};
    long double the_sum_of_squared_durations{0.0L};
};


std::ostream & operator<<(
    std::ostream & out,
    probing_summary_statistics_type const & stats);


#endif // EGT_COMPARISON_PROBING_SUMMARY_STATISTICS_TYPE_HPP
