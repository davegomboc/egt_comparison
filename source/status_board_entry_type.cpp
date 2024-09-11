#include "status_board_entry_type.hpp"


#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ostream>

#include "bdd_from_bits/fixed_decision_diagram_type.hpp"
#include "chesspresso_endgame_table_type.hpp"
#include "cube_list.hpp"
#include "indexed_cube_list.hpp"
#include "load_and_save.hpp"
#include "paths.hpp"


using representation::material_signature_type;


namespace {
    std::error_code const no_error{};
}


using representation::canonicalization_enum;
using representation::material_signature_type;
using representation::perspective_enum;


using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<multiterminal_binary_decision_diagram_egt_format_type>
     egt_format_ptr,
    material_signature_type const & material_signature,
    std::string_view const concretization_algorithm
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature),
    the_proc_opt{concretization_algorithm},
    the_path_opt{bdd_path(material_signature, concretization_algorithm, false)}
{
    std::error_code error_code;
    the_path_exists = the_path_opt and std::filesystem::exists(*the_path_opt,
     error_code);
    if ((no_error == error_code) and the_path_exists) {
        std::filesystem::path xz_path{*the_path_opt};
        xz_path += ".xz";
        bool const xz_path_exists{std::filesystem::exists(xz_path, error_code)};
        if ((no_error == error_code) and xz_path_exists) {
            the_bytes_on_disk_opt = std::filesystem::file_size(xz_path,
             error_code);
        } else {
            the_bytes_on_disk_opt = std::filesystem::file_size(*the_path_opt,
             error_code);
            std::cerr << "WW: Reporting bytes on disk for non-xz-compressed "
             << *the_path_opt << '\n';
        }
        fixed_decision_diagram_type bdd;
        [[maybe_unused]] unsigned int const root_node{
         bdd.add_nodes_from_file(the_path_opt->c_str())};
        // The BDD code bails if the load fails.
        the_path_loads = true;
        the_bytes_in_memory_opt = bdd.in_use_byte_count();
        the_node_count_opt = bdd.terminal_node_count()
         + bdd.nonterminal_node_count();
    } else {
        if (the_path_opt) {
            std::cerr << "WW: " << *the_path_opt << " does not exist.\n";
        } else {
            std::cerr << "II: " << extension()
             << ": the_path_opt = std::nullopt.\n";
        }
    }
}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<probe_report_nybble_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature),
    the_path_opt{probe_report_nybble_path(canonicalization_enum::all_symmetries,
     perspective_enum::white_player, material_signature, false)}
{
    std::error_code error_code;
    the_path_exists = the_path_opt and std::filesystem::exists(*the_path_opt,
     error_code);
    if ((no_error == error_code) and the_path_exists) {
        // It was decided to not report the xz-compressed size for
        // this method in the paper because by design, the .wpr files
        // are memory-mapped for use.
        the_bytes_on_disk_opt = std::filesystem::file_size(*the_path_opt,
         error_code);
    } else {
        if (the_path_opt) {
            std::cerr << "WW: " << *the_path_opt << " does not exist.\n";
        } else {
            std::cerr << "II: " << extension()
             << ": the_path_opt = std::nullopt.\n";
        }
    }
}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<decision_dag_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature),
    the_path_opt{decision_dag_path_opt(material_signature.as_string())}
{
    std::error_code error_code;
    the_path_exists = the_path_opt and std::filesystem::exists(*the_path_opt,
     error_code);
    if ((no_error == error_code) and the_path_exists) {
        std::filesystem::path xz_path{*the_path_opt};
        xz_path += ".xz";
        bool const xz_path_exists{std::filesystem::exists(xz_path, error_code)};
        if ((no_error == error_code) and xz_path_exists) {
            the_bytes_on_disk_opt = std::filesystem::file_size(xz_path,
             error_code);
        } else {
            the_bytes_on_disk_opt = std::filesystem::file_size(*the_path_opt,
             error_code);
            std::cerr << "WW: Reporting bytes on disk for non-xz-compressed "
             << *the_path_opt << '\n';
        }
        auto const decision_dag_opt(load_ddag_opt<32, 8, 27>(*the_path_opt));
        the_path_loads = decision_dag_opt.has_value();
        if (the_path_loads) {
            the_bytes_in_memory_opt = decision_dag_opt->nbytes();
            the_node_count_opt = decision_dag_opt->nnodes();
            the_F_cube_count_opt = decision_dag_opt->ncubes();
        }
    } else {
        if (the_path_opt) {
            std::cerr << "WW: " << *the_path_opt << " does not exist.\n";
        } else {
            std::cerr << "II: " << extension()
             << ": the_path_opt = std::nullopt.\n";
        }
    }
}


std::vector<std::string> split_syzygy_path_by_colons(
    std::string_view const path
) {
    std::vector<std::string> result;
    std::size_t segment_first_index{};
    while (true) {
        if (segment_first_index == path.size()) {
            break;
        }
        std::size_t const colon_location{path.find(':', segment_first_index)};
        bool const end_reached{std::string_view::npos == colon_location};
        std::size_t const segment_past_last_index{
         end_reached ? path.size() : colon_location};
        std::size_t const segment_size{segment_past_last_index - segment_first_index};
        if (0 < segment_size) {
            result.emplace_back(path.substr(segment_first_index, segment_size));
        }
        if (end_reached) {
            break;
        }
        segment_first_index = 1 + segment_past_last_index;
    }
    return result;
}


std::optional<std::filesystem::path> syzygy_table_location_opt(
    std::vector<std::string> const candidate_path_prefixes,
    std::string const path_suffix
) {
    std::error_code error_code;
    for (auto const & candidate_path_prefix: candidate_path_prefixes) {
        assert(not candidate_path_prefix.empty());
        std::filesystem::path candidate_path{candidate_path_prefix};
        candidate_path /= path_suffix;
        bool const table_location_found{
         std::filesystem::exists(candidate_path, error_code)};
        if ((no_error == error_code) and table_location_found) {
            return candidate_path;
        }
    }
    return std::nullopt;
}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<syzygy_wcdbl_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature)
{
    std::error_code error_code;
    std::string path_suffix{material_signature.as_syzygy_string()};
    path_suffix += '.';
    path_suffix += the_egt_format_ptr->extension();
    char const * const syzygy_tb_path_as_c_str{std::getenv("SYZYGY_PATH")};
    if (syzygy_tb_path_as_c_str) {
        std::vector<std::string> const candidate_path_prefixes{
         split_syzygy_path_by_colons(syzygy_tb_path_as_c_str)};
        the_path_opt = syzygy_table_location_opt(candidate_path_prefixes, path_suffix);
        the_path_exists = the_path_opt and std::filesystem::exists(*the_path_opt,
         error_code);
        if ((no_error == error_code) and the_path_exists) {
            // It was decided to not report the xz-compressed size for
            // this method in the paper because by design, the .rtbw
            // files are memory-mapped for use.
            the_bytes_on_disk_opt = std::filesystem::file_size(*the_path_opt,
             error_code);
        } else {
            if (the_path_opt) {
                // We don't want to print a warning for KvK: Syzygy doesn't bother
                // to have that data file (though it does have KBvK and KNvK, the
                // positions of which are also always drawn).
                if ("KvK" != material_signature.as_syzygy_string()) {
                    std::cerr << "WW: " << *the_path_opt << " does not exist.\n";
                }
            } else {
                std::cerr << "II: " << extension()
                 << ": the_path_opt = std::nullopt.\n";
            }
        }
    } else {
        std::cerr << "WW: SYZYGY_PATH environment variable not found.\n";
    }
}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<syzygy_dtzm_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature)
{
    std::error_code error_code;
    std::string path_suffix{material_signature.as_syzygy_string()};
    path_suffix += '.';
    path_suffix += the_egt_format_ptr->extension();
    char const * const syzygy_tb_path_as_c_str{std::getenv("SYZYGY_PATH")};
    if (syzygy_tb_path_as_c_str) {
        std::vector<std::string> const candidate_path_prefixes{
         split_syzygy_path_by_colons(syzygy_tb_path_as_c_str)};
        the_path_opt = syzygy_table_location_opt(candidate_path_prefixes, path_suffix);
        the_path_exists = the_path_opt and std::filesystem::exists(*the_path_opt,
         error_code);
        if ((no_error == error_code) and the_path_exists) {
            // It was decided to not report the xz-compressed size for
            // this method in the paper because by design, the .rtbz
            // files are memory-mapped for use.
            the_bytes_on_disk_opt = std::filesystem::file_size(*the_path_opt,
             error_code);
        } else {
            if (the_path_opt) {
                // We don't want to print a warning for KvK: Syzygy doesn't bother
                // to have that data file (though it does have KBvK and KNvK, the
                // positions of which are also always drawn).
                if ("KvK" != material_signature.as_syzygy_string()) {
                    std::cerr << "WW: " << *the_path_opt << " does not exist.\n";
                }
            } else {
                std::cerr << "II: " << extension()
                 << ": the_path_opt = std::nullopt.\n";
            }
        }
    } else {
        std::cerr << "WW: SYZYGY_PATH environment variable not found.\n";
    }
}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<cube_list_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature,
    std::string_view const proc
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature),
    the_proc_opt{not proc.empty() ? std::optional<std::string>(proc)
     : std::nullopt},
    the_path_opt{cube_list_path_opt(material_signature.as_string(), proc, false)}
{
    std::error_code error_code;
    the_path_exists = the_path_opt and std::filesystem::exists(*the_path_opt,
     error_code);
    if ((no_error == error_code) and the_path_exists) {
        std::filesystem::path xz_path{*the_path_opt};
        xz_path += ".xz";
        bool const xz_path_exists{std::filesystem::exists(xz_path, error_code)};
        if ((no_error == error_code) and xz_path_exists) {
            the_bytes_on_disk_opt = std::filesystem::file_size(xz_path,
             error_code);
        } else {
            the_bytes_on_disk_opt = std::filesystem::file_size(*the_path_opt,
             error_code);
            std::cerr << "WW: Reporting bytes on disk for non-xz-compressed "
             << *the_path_opt << '\n';
        }
        auto const cube_list_opt{cube_list_type::create_opt(*the_path_opt)};
        the_path_loads = cube_list_opt.has_value();
        if (the_path_loads) {
            the_bytes_in_memory_opt = cube_list_opt->nbytes();
            the_F_cube_count_opt = cube_list_opt->ncubes();
        }
    } else {
        if (the_path_opt) {
            std::cerr << "WW: " << *the_path_opt << " does not exist.\n";
        } else {
            std::cerr << "II: " << extension()
             << ": the_path_opt = std::nullopt.\n";
        }
    }
}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<cube_list_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature,
    unsigned int const merge_distance,
    std::string_view const proc_suffix
) : status_board_entry_type(std::move(egt_format_ptr),
    material_signature, std::string("D")
     + as_minimum_two_digit_string(merge_distance)
     + (proc_suffix.empty() ? "" : std::string("_")
     + std::string(proc_suffix))) {}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<two_level_logic_minimization_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature,
    std::string_view const proc
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature),
    the_proc_opt(proc)
{
    status_board_entry_type clst_entry(
     std::make_unique<cube_list_egt_format_type>(), material_signature, proc);
    status_board_entry_type icl_entry(
     std::make_unique<indexed_cube_list_egt_format_type>(),
     material_signature, proc);
    the_path_opt = clst_entry.path();
    the_path_exists = clst_entry.path_exists() and icl_entry.path_exists();
    the_path_loads = clst_entry.path_loads() and icl_entry.path_loads();
    the_bytes_on_disk_opt = clst_entry.bytes_on_disk();
    the_bytes_in_memory_opt = icl_entry.bytes_in_memory_opt();
    the_node_count_opt = clst_entry.node_count_opt();
    the_F_cube_count_opt = clst_entry.f_cube_count_opt();
}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<indexed_cube_list_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature,
    std::string_view const proc
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature),
    the_proc_opt{not proc.empty() ? std::optional<std::string>(proc)
     : std::nullopt},
    the_path_opt{cube_list_path_opt(material_signature.as_string(), proc, true)}
{
    std::error_code error_code;
    the_path_exists = the_path_opt and std::filesystem::exists(*the_path_opt,
     error_code);
    if ((no_error == error_code) and the_path_exists) {
        std::filesystem::path xz_path{*the_path_opt};
        xz_path += ".xz";
        bool const xz_path_exists{std::filesystem::exists(*the_path_opt,
         error_code)};
        if ((no_error == error_code) and xz_path_exists) {
            the_bytes_on_disk_opt = std::filesystem::file_size(xz_path,
             error_code);
        } else {
            the_bytes_on_disk_opt = std::filesystem::file_size(*the_path_opt,
             error_code);
            std::cerr << "WW: Reporting bytes on disk for non-xz-compressed "
             << *the_path_opt << '\n';
        }
        auto const indexed_cube_list_opt{
         indexed_cube_list_type::create_opt(*the_path_opt)};
        if (indexed_cube_list_opt.has_value()) {
            the_path_loads = true;
            the_bytes_in_memory_opt = indexed_cube_list_opt->nbytes();
            the_F_cube_count_opt = indexed_cube_list_opt->ncubes();
        }
    } else {
        if (the_path_opt) {
            std::cerr << "WW: " << *the_path_opt << " does not exist.\n";
        } else {
            std::cerr << "II: " << extension()
             << ": the_path_opt = std::nullopt.\n";
        }
    }
}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<indexed_cube_list_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature,
    unsigned int const merge_distance,
    std::string_view const proc_suffix
) : status_board_entry_type(std::move(egt_format_ptr),
    material_signature, std::string("D")
     + as_minimum_two_digit_string(merge_distance)
     + (proc_suffix.empty() ? "" : std::string("_")
     + std::string(proc_suffix))) {}


status_board_entry_type::status_board_entry_type(
    std::unique_ptr<chesspresso_binary_egt_format_type> egt_format_ptr,
    material_signature_type const & material_signature,
    std::optional<std::string_view> const path_segment_opt
) : the_egt_format_ptr{std::move(egt_format_ptr)},
    the_material_signature(material_signature),
    the_proc_opt{path_segment_opt},
    the_path_opt{chesspresso_path(path_segment_opt, material_signature,
     std::nullopt, true, false)}
{
    std::error_code error_code;
    the_path_exists = the_path_opt and std::filesystem::exists(*the_path_opt,
     error_code);
    if ((no_error != error_code) or (not the_path_exists)) {
        if (the_path_opt) {
            std::cerr << "WW: " << *the_path_opt << " does not exist.\n";
            std::cerr << "WW: error_code = " << error_code << ".\n";
        } else {
            std::cerr << "II: " << extension()
             << ": the_path_opt = std::nullopt.\n";
        }
        return;
    }

    std::filesystem::path xz_path(*the_path_opt);
    xz_path += ".xz";
    bool const xz_path_exists{std::filesystem::exists(xz_path, error_code)};
    if ((no_error != error_code) or (not xz_path_exists)) {
        std::cerr << "II: xz_path = " << xz_path << ".\n";
        std::cerr << "EE: error_code = " << error_code << ".\n";
        return;
    }

    the_bytes_on_disk_opt = std::filesystem::file_size(xz_path, error_code);
    std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
     read_chesspresso_endgame_table(xz_path, 2)};
    the_path_loads = (nullptr != table_ptr);
    if (the_path_loads) {
        // TODO: Determine how much memory it occupies when loaded.
        // the_bytes_in_memory_opt = ?
        the_F_cube_count_opt = table_ptr->cover(
         representation::cover_enum::function).size();
    }
}


void status_board_entry_type::emit(std::ostream & out) const {
    bool const loaded{the_egt_format_ptr->use_requires_loading_into_memory()};
    out << std::setw(11) << the_material_signature
        << std::setw(6) << the_egt_format_ptr->as_string()
        << std::setw(14) << (the_proc_opt ? *the_proc_opt : "")
        << std::setw(8) << (the_path_exists ? "exists" : "missing")
        << std::setw(14) << (the_bytes_on_disk_opt ?
                             std::to_string(*the_bytes_on_disk_opt) : "n/a")
        << std::setw(6) << (loaded ? (the_path_loads ?
                            "okay" : "error") : "n/a")
        << std::setw(14) << (the_bytes_in_memory_opt ?
                             std::to_string(*the_bytes_in_memory_opt) : "n/a")
        << std::setw(14) << (the_node_count_opt ?
                             std::to_string(*the_node_count_opt) : "n/a")
        << std::setw(14) << (the_F_cube_count_opt ?
                             std::to_string(*the_F_cube_count_opt) : "n/a")
        << std::endl;
}
