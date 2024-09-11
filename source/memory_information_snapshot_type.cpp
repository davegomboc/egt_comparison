#include "memory_information_snapshot_type.hpp"


#include <algorithm>
#include <cassert>
#include <cctype>
#include <charconv>
#include <fstream>
#include <iterator>

#include "primitive_text_operations.hpp"


// Returns the count of nanoseconds since the epoch for the provided timestamp.
std::size_t as_size_t(std::chrono::steady_clock::time_point const & timestamp) {
    return static_cast<std::size_t>(timestamp.time_since_epoch().count());
}


// N.B. /proc/meminfo appears to use kB rather than kiB to mean 2^10, and so on.
std::size_t decode_proc_meminfo_suffix(std::string_view & view_ref) {
    std::size_t result{1};
    if (view_ref.empty() or ('B' != view_ref.back())) {
        return result;
    }
    std::string_view view(view_ref);
    view.remove_suffix(1);
    if (view.empty()) {
        return result;
    }
    if (' ' == view.back()) {
        view.remove_suffix(1);
        view_ref = view;
        return result;
    }
    switch (view.back()) {
    case 'Q':
        result <<= 10;
        [[fallthrough]];
    case 'R':
        result <<= 10;
        [[fallthrough]];
    case 'Y':
        result <<= 10;
        [[fallthrough]];
    case 'Z':
        result <<= 10;
        [[fallthrough]];
    case 'E':
        result <<= 10;
        [[fallthrough]];
    case 'P':
        result <<= 10;
        [[fallthrough]];
    case 'T':
        result <<= 10;
        [[fallthrough]];
    case 'G':
        result <<= 10;
        [[fallthrough]];
    case 'M':
        result <<= 10;
        [[fallthrough]];
    case 'k':
        result <<= 10;
        view.remove_suffix(1);
        break;
    default:
        return result;
    }
    if (view.empty() or (' ' != view.back())) {
        result = 1;
        return result;
    }
    view.remove_suffix(1);
    view_ref = view;
    return result;
}


// Returns the primary key, followed by every other key in lexicographic order.
std::vector<std::string> keys(
    std::unordered_map<std::string, std::size_t> const & memory_information
) {
    std::string const primary_key{
     memory_information_snapshot_type::primary_key()};
    std::vector<std::string> result;
    result.reserve(memory_information.size());
    result.push_back(primary_key);
    [[maybe_unused]] bool found_primary_key{};
    for (auto const & key_and_value: memory_information) {
        if (key_and_value.first == primary_key) {
            found_primary_key = true;
        } else {
            result.push_back(key_and_value.first);
        }
    }
    assert(found_primary_key);
    std::sort(std::next(result.begin()), result.end());
    return result;
}


std::optional<std::pair<std::string, std::size_t>> parse_line_opt(
    std::string_view line
) {
    if (contains(line, '\0')) {
        return std::nullopt;
    }
    std::optional<std::pair<std::string_view, std::string_view>>
     key_value_pair_opt{leftmost_split(line, ':')};
    if (std::nullopt == key_value_pair_opt) {
        return std::nullopt;
    }
    std::string_view value_view{key_value_pair_opt->second};
    trim_whitespace(value_view);
    std::size_t const multiplier{decode_proc_meminfo_suffix(value_view)};
    if (value_view.empty()) {
        return std::nullopt;
    }
    std::string const value_str{value_view};
    char const * const value_as_c_str{value_str.c_str()};
    std::size_t value;
    auto [ptr, errc]{std::from_chars(value_as_c_str,
     value_as_c_str + value_view.size(), value)};
    if (errc != std::errc()) {
        return std::nullopt;
    }
    value *= multiplier;
    std::string_view key_view{key_value_pair_opt->first};
    trim_whitespace(key_view);
    return std::make_pair<std::string, std::size_t>(std::string(key_view), std::move(value));
}


std::optional<std::unordered_map<std::string, std::size_t>> parse_opt(
    std::istream & data_in
) {
    if (not data_in.good()) {
        return std::nullopt;
    }
    std::unordered_map<std::string, std::size_t> keys_and_values;
    std::string line;
    while (std::getline(data_in, line)) {
        std::optional<std::pair<std::string, std::size_t>> parsed_line_opt{
         parse_line_opt(line)};
        if (std::nullopt == parsed_line_opt) {
            return std::nullopt;
        }
        keys_and_values[parsed_line_opt->first] = parsed_line_opt->second;
    }
    if (data_in.eof()) {
        return keys_and_values;
    }
    return std::nullopt;
}


memory_information_snapshot_type::memory_information_snapshot_type()
  : the_timestamp(std::chrono::steady_clock::now()),
    the_contents_of_proc_meminfo(),
    the_memory_information_opt(std::nullopt)
{
    std::ifstream data_in("/proc/meminfo");
    the_contents_of_proc_meminfo = std::string(
     std::istreambuf_iterator<char>{data_in}, {});
}


std::optional<std::vector<std::string>>
memory_information_snapshot_type::keys_opt() {
    parse();
    if (not parsed()) {
        return std::nullopt;
    }
    return keys(*the_memory_information_opt);
}


bool memory_information_snapshot_type::parse() {
    if (parsed()) {
        return false;
    }
    std::string from_proc_meminfo(the_contents_of_proc_meminfo.begin(),
     the_contents_of_proc_meminfo.end());
    std::istringstream data_in_from_proc_meminfo(from_proc_meminfo);
    the_memory_information_opt = parse_opt(data_in_from_proc_meminfo);
    if (std::nullopt == the_memory_information_opt) {
        return true;
    }
    std::string const timestamp_key(primary_key());
    std::size_t const timestamp_value{as_size_t(the_timestamp)};
    (*the_memory_information_opt)[timestamp_key] = timestamp_value;
    return false;
}


bool memory_information_snapshot_type::parsed() {
    return std::nullopt != the_memory_information_opt;
}
