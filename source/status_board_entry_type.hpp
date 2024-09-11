#ifndef EGT_COMPARISON_STATUS_BOARD_ENTRY_HPP
#define EGT_COMPARISON_STATUS_BOARD_ENTRY_HPP


#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

#include "representation/material_signature_type.hpp"

#include "egt_format_type.hpp"


inline std::string as_minimum_two_digit_string(unsigned int const value) {
    if (value < 10) {
        return std::string("0") + std::to_string(value);
    }
    return std::to_string(value);
}


class status_board_entry_type {
public:
    status_board_entry_type(
     std::unique_ptr<multiterminal_binary_decision_diagram_egt_format_type>,
     representation::material_signature_type const & signature,
     std::string_view const concretization_algorithm);

    status_board_entry_type(
     std::unique_ptr<probe_report_nybble_egt_format_type>,
     representation::material_signature_type const & signature);

    status_board_entry_type(
     std::unique_ptr<decision_dag_egt_format_type>,
     representation::material_signature_type const & signature);

    status_board_entry_type(
     std::unique_ptr<syzygy_dtzm_egt_format_type>,
     representation::material_signature_type const & signature);

    status_board_entry_type(
     std::unique_ptr<syzygy_wcdbl_egt_format_type>,
     representation::material_signature_type const & signature);

    status_board_entry_type(
     std::unique_ptr<cube_list_egt_format_type>,
     representation::material_signature_type const & signature,
     std::string_view const proc);

    status_board_entry_type(
     std::unique_ptr<cube_list_egt_format_type>,
     representation::material_signature_type const & signature,
     unsigned int merge_distance,
     std::string_view const proc_suffix);

    status_board_entry_type(
     std::unique_ptr<indexed_cube_list_egt_format_type>,
     representation::material_signature_type const & signature,
     std::string_view const proc);

    status_board_entry_type(
     std::unique_ptr<indexed_cube_list_egt_format_type>,
     representation::material_signature_type const & signature,
     unsigned int merge_distance,
     std::string_view const proc_suffix);

    status_board_entry_type(
     std::unique_ptr<two_level_logic_minimization_egt_format_type>,
     representation::material_signature_type const & signature,
     std::string_view const proc);

    status_board_entry_type(
     std::unique_ptr<chesspresso_binary_egt_format_type>,
     representation::material_signature_type const & signature,
     std::optional<std::string_view> const path_segment_opt);

    std::string extension() const {
        return the_egt_format_ptr->extension();
    }

    std::filesystem::path path() const {
        return (the_path_opt ? *the_path_opt : std::filesystem::path());
    }

    std::string proc() const {
        return (the_proc_opt ? *the_proc_opt : "ERROR");
    }

    bool path_exists() const {
       return the_path_exists;
    }

    bool path_loads() const {
       return the_path_loads;
    }

    std::uintmax_t bytes_on_disk() const {
        return (the_bytes_on_disk_opt ? *the_bytes_on_disk_opt
         : std::numeric_limits<std::uintmax_t>::max());
    }

    std::optional<std::size_t> bytes_in_memory_opt() const {
        return the_bytes_in_memory_opt;
    }

    std::optional<std::size_t> node_count_opt() const {
        return the_node_count_opt;
    }

    std::optional<std::size_t> f_cube_count_opt() const {
        return the_F_cube_count_opt;
    }

    void emit(std::ostream & out) const;
    void emit_header(std::ostream & out) const;

private:
    std::unique_ptr<egt_format_type> the_egt_format_ptr;
    representation::material_signature_type the_material_signature;
    std::optional<std::string> the_proc_opt;
    std::optional<std::filesystem::path> the_path_opt;
    bool the_path_exists;
    bool the_path_loads;
    std::optional<std::uintmax_t> the_bytes_on_disk_opt;
    std::optional<std::size_t> the_bytes_in_memory_opt;
    std::optional<std::size_t> the_node_count_opt;
    std::optional<std::size_t> the_F_cube_count_opt;
};


#endif // EGT_COMPARISON_STATUS_BOARD_ENTRY_HPP
