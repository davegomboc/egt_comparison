#if !defined(EGT_COMPARISON_EXECUTION_MEASUREMENT_HPP)
#define EGT_COMPARISON_EXECUTION_MEASUREMENT_HPP


#include <chrono>
#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <utility>


template <typename callable_type, typename ... argument_types>
inline auto measure_execution_time(
    callable_type && callable,
    argument_types && ... arguments
) {
    auto const start = std::chrono::steady_clock::now();
    std::invoke(std::forward<decltype(callable)>(callable),
     std::forward<argument_types>(arguments)...);
    auto const stop = std::chrono::steady_clock::now();
    return std::make_pair(start, stop);
}


std::optional<std::size_t> value_from_proc_meminfo_opt(
    std::string const desired_prefix,
    std::string const desired_suffix);


std::optional<std::size_t> available_bytes_opt();

std::optional<std::size_t> total_bytes_opt();


#endif // EGT_COMPARISON_EXECUTION_MEASUREMENT_HPP
