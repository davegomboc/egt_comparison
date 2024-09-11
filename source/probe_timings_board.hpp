#ifndef EGT_COMPARISON_EGT_PROBE_TIMINGS_BOARD_HPP
#define EGT_COMPARISON_EGT_PROBE_TIMINGS_BOARD_HPP


#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>

#include "egt_format_type.hpp"


void determine_and_emit_probe_timing_board_entries(
    std::filesystem::path const input_path,
    std::string const machine,
    egt_format_type const * egt_format_ptr,
    std::size_t const epd_line_count);


#endif // EGT_COMPARISON_EGT_PROBE_TIMINGS_BOARD_HPP
