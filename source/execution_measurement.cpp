#include "execution_measurement.hpp"


#include <fstream>
#include <string_view>


std::optional<std::size_t> value_from_proc_meminfo_opt(
    std::string const desired_prefix,
    std::string const desired_suffix
) {
    std::ifstream memory_info("/proc/meminfo");
    std::string line;
    while (std::getline(memory_info, line)) {
        if (0 != line.rfind(desired_prefix, 0)) {
            continue;
        }
        std::string_view view(line);
        view.remove_prefix(desired_prefix.size());
        if (desired_suffix != view.substr(view.size() - desired_suffix.size(),
         desired_suffix.size())) {
            return std::nullopt;
        }
        view.remove_suffix(desired_suffix.size());
        line = view;
        std::size_t bytes{static_cast<std::size_t>(atoll(line.c_str()))};
        return bytes;
    }
    return std::nullopt;
}


std::optional<std::size_t> available_bytes_opt() {
    std::optional<std::size_t> available_kB_opt{
     value_from_proc_meminfo_opt("MemAvailable:", " kB")};
    if (std::nullopt == available_kB_opt) {
        return std::nullopt;
    }
    return 1024 * *available_kB_opt;
}


std::optional<std::size_t> total_bytes_opt() {
    std::optional<std::size_t> total_kB_opt{
     value_from_proc_meminfo_opt("MemTotal:", " kB")};
    if (std::nullopt == total_kB_opt) {
        return std::nullopt;
    }
    return 1024 * *total_kB_opt;
}
