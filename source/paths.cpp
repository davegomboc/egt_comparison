#include "paths.hpp"


#include <cassert>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <sstream>

#include "environment_variable_access.hpp"


using representation::canonicalization_enum;
using representation::figurine_colour_enum;
using representation::material_signature_type;
using representation::perspective_enum;


std::filesystem::path defaulted_path(
    std::string_view const environment_variable_name,
    std::filesystem::path const defaulted_subdirectory_of_home
) {
    std::optional<std::string> table_path_opt{
     get_environment_variable_opt(environment_variable_name)};
    if (table_path_opt) {
        return std::filesystem::path(*table_path_opt);
    }
    std::optional<std::filesystem::path> home_opt{get_home_directory_opt()};
    if (not home_opt) {
        std::cerr << "EE: Unable to obtain home directory.\n";
        std::exit(EXIT_FAILURE);
    }
    return *home_opt / defaulted_subdirectory_of_home;
}


std::filesystem::path bdd_path_prefix() {
    std::filesystem::path result(defaulted_path("EGT_DIR", "/"));
    result /= "decision_diagrams";
    return result;
}


std::filesystem::path decision_dag_path_prefix() {
    std::filesystem::path result(defaulted_path("EGT_DIR", "/"));
    result /= "decision_dags";
    return result;
}


std::filesystem::path probe_report_nybble_path_prefix() {
    std::filesystem::path result(defaulted_path("EGT_DIR", "/"));
    result /= "probe_report_nybble";
    return result;
}


std::filesystem::path chesspresso_path_prefix() {
    std::filesystem::path result(defaulted_path("EGT_DIR", "/"));
    result /= "chesspresso";
    return result;
}


std::string material_signature_infix(
    bool const want_figurine_count_subdirectory,
    canonicalization_enum const canonicalization,
    material_signature_type const & material_signature
) {
    std::ostringstream stream;
    if (want_figurine_count_subdirectory) {
        stream << material_signature.size() << '/';
    }
    if (canonicalization != canonicalization_enum::all_symmetries) {
        stream << representation::as_string_view(canonicalization);
        stream.put('.');
    }
    stream << material_signature.as_string();
    return stream.str();
}


std::filesystem::path probe_report_nybble_path_suffix(
    canonicalization_enum const canonicalization,
    perspective_enum const perspective,
    material_signature_type const & material_signature,
    bool const xz_compressed
) {
    assert(valid(canonicalization));
    assert(valid(perspective));
    std::ostringstream suffix_stream;
    suffix_stream
     << material_signature_infix(true, canonicalization, material_signature)
     << '.' << ((perspective_enum::side_to_move == perspective) ? 's' : 'w')
     << "pr";
    if (xz_compressed) {
        suffix_stream << ".xz";
    }
    return std::filesystem::path(suffix_stream.str());
}


std::filesystem::path bdd_path_suffix(
    material_signature_type const & material_signature,
    std::string_view const concretization_algorithm,
    bool const xz_compressed
) {
    std::ostringstream suffix_stream;
    suffix_stream << material_signature.size() << '/'
     << material_signature.as_string() << "." << concretization_algorithm
     << ".mtbdd";
    if (xz_compressed) {
        suffix_stream << ".xz";
    }
    return std::filesystem::path(suffix_stream.str());
}


std::filesystem::path decision_dag_path_suffix(
    material_signature_type const & material_signature,
    bool const xz_compressed
) {
    std::ostringstream suffix_stream;
    suffix_stream << material_signature.size() << '/'
     << material_signature.as_string() << ".dtr";
    if (xz_compressed) {
        suffix_stream << ".xz";
    }
    return std::filesystem::path(suffix_stream.str());
}


std::filesystem::path chesspresso_path_suffix(
    material_signature_type const & material_signature,
    std::optional<std::string_view const> const cover_spec_opt,
    bool const binary_format,
    bool const xz_compressed
) {
    std::ostringstream suffix_stream;
    suffix_stream << material_signature.size() << '/'
     << material_signature.as_string();
    if (cover_spec_opt) {
        suffix_stream.put('.');
        suffix_stream << *cover_spec_opt;
    }
    suffix_stream << ".pl";
    if (binary_format) {
        suffix_stream.put('b');
    } else {
        suffix_stream.put('a');
    }
    if (xz_compressed) {
        suffix_stream << ".xz";
    }
    return std::filesystem::path(suffix_stream.str());
}


std::filesystem::path bdd_path(
    material_signature_type const & material_signature,
    std::string_view const concretization_algorithm,
    bool const xz_compressed
) {
    std::filesystem::path result(bdd_path_prefix());
    result /= bdd_path_suffix(material_signature, concretization_algorithm,
     xz_compressed);
    return result;
}


std::filesystem::path decision_dag_path(
    material_signature_type const & material_signature,
    bool const xz_compressed
) {
    std::filesystem::path result(decision_dag_path_prefix());
    result /= decision_dag_path_suffix(material_signature, xz_compressed);
    return result;
}


std::filesystem::path probe_report_nybble_path(
    canonicalization_enum const canonicalization,
    perspective_enum const perspective,
    material_signature_type const & material_signature,
    bool const xz_compressed
) {
    std::filesystem::path result{probe_report_nybble_path_prefix()};
    result /= probe_report_nybble_path_suffix(canonicalization, perspective,
     material_signature, xz_compressed);
    return result;
}

std::filesystem::path chesspresso_path(
    std::optional<std::filesystem::path> const subdirectory_opt,
    material_signature_type const & material_signature,
    std::optional<std::string_view const> const cover_spec_opt,
    bool const binary_format,
    bool const xz_compressed
) {
    std::filesystem::path result(chesspresso_path_prefix());
    if (subdirectory_opt) {
        result /= *subdirectory_opt;
    }
    result /= chesspresso_path_suffix(material_signature, cover_spec_opt,
     binary_format, xz_compressed);
    return result;
}
