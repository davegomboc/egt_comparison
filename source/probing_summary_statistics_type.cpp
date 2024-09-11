#include "probing_summary_statistics_type.hpp"


#include <cassert>
#include <iomanip>
#include <ostream>


std::optional<long double>
probing_summary_statistics_type::duration_mean_opt() const {
    if (count() < 1) {
        return std::nullopt;
    }
    return sum_of_durations() / count();
}


std::optional<long double>
probing_summary_statistics_type::duration_sample_standard_deviation_opt()
const {
    if (count() < 2) {
        return std::nullopt;
    }
    long double const squared_sum_of_durations{
     sum_of_durations() * sum_of_durations()};
    long double const numerator{
     sum_of_squared_durations() - squared_sum_of_durations / count()};
    return std::sqrt(numerator / count() - 1.0L);
}


void probing_summary_statistics_type::include(
    std::chrono::duration<long double, std::nano> const probe_duration
) {
    // TODO: Use a big integer type so that adding one always changes the value.
    assert(the_duration_count < the_duration_count + 1);
    ++the_duration_count;
    long double nanoseconds{probe_duration.count()};
    the_sum_of_durations += nanoseconds;
    the_sum_of_squared_durations += nanoseconds * nanoseconds;
}


void probing_summary_statistics_type::emit(
    std::ostream & output_stream,
    bool const sufficient_statistics_only
) const {
    std::ostream out(output_stream.rdbuf());
    out << std::scientific
        << std::setprecision(std::numeric_limits<long double>::max_digits10)
        << "duration_count = " << count()
        << "\nsum_of_durations = " << sum_of_durations()
        << "\nsum_of_squared_durations = " << sum_of_squared_durations()
        << "\n";
    if (not sufficient_statistics_only) {
        std::optional<long double> mean_opt{duration_mean_opt()};
        out << "duration_mean = ";
        if (mean_opt) {
            out << *mean_opt;
        } else {
            out << "n/a";
        }
        out << '\n';
        std::optional<long double> deviation_opt{
         duration_sample_standard_deviation_opt()};
        out << "duration_sample_standard_deviation = ";
        if (deviation_opt) {
            out << *deviation_opt;
        } else {
            out << "n/a";
        }
        out << '\n';
    }
}


std::ostream & operator<<(
    std::ostream & output_stream,
    probing_summary_statistics_type const & stats
) {
    std::ostream out(output_stream.rdbuf());
    out << std::scientific
        << std::setprecision(std::numeric_limits<long double>::max_digits10)
        << "(n = " << stats.count() << ", sum(x) = " << stats.sum_of_durations()
        << ", sum(x^2) = " << stats.sum_of_squared_durations() << ")\n";
    return output_stream;
}
