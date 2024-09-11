#if !defined(EGT_COMPARISON_MEMORY_INFORMATION_SNAPSHOT_TYPE_HPP)
#define EGT_COMPARISON_MEMORY_INFORMATION_SNAPSHOT_TYPE_HPP


#include <chrono>
#include <cstddef>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <unordered_map>
#include <vector>

#include "default_init_type.hpp"


std::size_t as_size_t(
    std::chrono::steady_clock::time_point const & time_point);


std::size_t decode_proc_meminfo_suffix(std::string_view & view_ref);


std::vector<std::string> keys(
    std::unordered_map<std::string, std::size_t> const & memory_information);


std::optional<std::pair<std::string, std::size_t>> parse_line_opt(
    std::string_view line);


std::optional<std::unordered_map<std::string, std::size_t>> parse_opt(
    std::istream & data_in);


class memory_information_snapshot_type {
public:
    memory_information_snapshot_type();

    bool empty() const {
        return the_contents_of_proc_meminfo.empty();
    }

    // Returns true iff a prior attempt at parsing succeeded.
    bool parsed();

    // Returns true iff an error occurred.
    bool parse();

    // When /proc/meminfo is parseable, returns the primary key, followed by
    // every other key in lexicographic order.  Otherwise, returns std::nullopt.
    std::optional<std::vector<std::string>> keys_opt();

    std::optional<std::unordered_map<std::string, std::size_t>> &
    parsed_data_opt() {
        return the_memory_information_opt;
    }

    static std::string_view const primary_key() {
        return the_primary_key;
    }

private:
    static std::string_view constexpr the_primary_key{"SteadyClockNow"};

    std::optional<std::pair<std::string, std::size_t>> parse_line(std::string);

    std::chrono::time_point<std::chrono::steady_clock> the_timestamp;

    std::string the_contents_of_proc_meminfo;

    std::optional<std::unordered_map<std::string, std::size_t>>
     the_memory_information_opt;
};


#endif // EGT_COMPARISON_MEMORY_INFORMATION_SNAPSHOT_TYPE_HPP
