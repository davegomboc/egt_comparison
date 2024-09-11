#if !defined(EGT_COMPARISON_PATHS_HPP)
#define EGT_COMPARISON_PATHS_HPP


#include <filesystem>
#include <optional>
#include <string_view>

#include "representation/canonicalization_enum.hpp"
#include "representation/figurine_enum.hpp"
#include "representation/material_signature_type.hpp"
#include "representation/perspective_enum.hpp"
#include "probe_report_type.hpp"


std::filesystem::path bdd_path_prefix();
std::filesystem::path decision_dag_path_prefix();
std::filesystem::path probe_report_nybble_path_prefix();
std::filesystem::path chesspresso_path_prefix();

std::string material_signature_infix(
    bool want_figurine_count_subdirectory,
    representation::canonicalization_enum canonicalization,
    representation::material_signature_type const & material_signature);

std::filesystem::path bdd_path_suffix(
    representation::material_signature_type const & material_signature,
    std::string_view const concretization_algorithm,
    bool xz_compressed);

std::filesystem::path decision_dag_path_suffix(
    representation::material_signature_type const & material_signature,
    bool xz_compressed);

std::filesystem::path chesspresso_path_suffix(
    representation::material_signature_type const & material_signature,
    std::optional<std::string_view const> const cover_spec_opt,
    bool binary_format,
    bool xz_compressed);

std::filesystem::path probe_report_nybble_path_suffix(
    representation::canonicalization_enum canonicalization,
    representation::perspective_enum perspective,
    representation::material_signature_type const & material_signature,
    bool xz_compressed);

std::filesystem::path bdd_path(
    representation::material_signature_type const & material_signature,
    std::string_view const concretization_algorithm,
    bool xz_compressed);

std::filesystem::path decision_dag_path(
    representation::material_signature_type const & material_signature,
    bool xz_compressed);

std::filesystem::path chesspresso_path(
    std::optional<std::filesystem::path> const subdirectory_opt,
    representation::material_signature_type const & material_signature,
    std::optional<std::string_view const> const cover_spec_opt,
    bool binary_format,
    bool xz_compressed);

std::filesystem::path probe_report_nybble_path(
    representation::canonicalization_enum canonicalization,
    representation::perspective_enum perspective,
    representation::material_signature_type const & material_signature,
    bool xz_compressed);


#endif // EGT_COMPARISON_PATHS_HPP
