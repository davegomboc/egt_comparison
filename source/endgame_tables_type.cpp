#include "endgame_tables_type.hpp"


#include <cstdlib>
#include <iostream>

#include "representation/canonicalization_enum.hpp"

#include "bdd_endgame_table_type.hpp"
#include "chesspresso_endgame_table_type.hpp"
#include "cube_list_endgame_table_type.hpp"
#include "decision_dag_endgame_table_type.hpp"
#include "indexed_cube_list_endgame_table_type.hpp"
#include "probe_report_nybble_endgame_table_type.hpp"


using representation::as_canonicalization_enum_opt;
using representation::as_string_view;
using representation::canonicalization_enum;
using representation::material_signature_type;


endgame_tables_type::endgame_tables_type(
    unsigned int const highest_figurine_count,
    egt_format_type const & egt_format,
    bool const exit_on_load_failure
) : the_enumerated_material_signatures(highest_figurine_count) {
    assert((2 <= highest_figurine_count) and (highest_figurine_count <= 9));
    for (unsigned int figurine_count{2};
     figurine_count <= highest_figurine_count; ++figurine_count) {
        for (auto const & material_signature:
         the_enumerated_material_signatures.material_signatures(
         figurine_count)) {
            if (exit_on_load_failure) {
                load_or_exit(egt_format, material_signature);
            } else {
                load_or_warn(egt_format, material_signature);
            }
        }
    }
}


endgame_tables_type::endgame_tables_type(
    unsigned int const highest_figurine_count,
    egt_format_type const & egt_format,
    std::string const & version_specifier,
    bool const exit_on_load_failure
) : the_enumerated_material_signatures(highest_figurine_count) {
    assert((2 <= highest_figurine_count) and (highest_figurine_count <= 9));
    for (unsigned int figurine_count{2};
     figurine_count <= highest_figurine_count; ++figurine_count) {
        for (auto const & material_signature:
         the_enumerated_material_signatures.material_signatures(
         figurine_count)) {
            if (exit_on_load_failure) {
                load_or_exit(egt_format, material_signature, version_specifier);
            } else {
                load_or_warn(egt_format, material_signature, version_specifier);
            }
        }
    }
}


endgame_table_type const * endgame_tables_type::get(
    material_signature_type const & material_signature
) const {
    auto iter{the_egt_ptrs.find(material_signature)};
    if (the_egt_ptrs.end() == iter) {
        return nullptr;
    }
    return iter->second.get();
}


std::unique_ptr<endgame_table_type> endgame_tables_type::load(
    std::filesystem::path const & egt_path
) {
    std::filesystem::path const extension(egt_path.extension());
    if (".clst" == extension) {
        return cube_list_endgame_table_type::create_ptr(egt_path);
    } else if (".dtr" == extension) {
        return decision_dag_endgame_table_type::create_ptr(egt_path);
    } else if (".icl" == extension) {
        return indexed_cube_list_endgame_table_type::create_ptr(egt_path);
    } else if (".mtbdd" == extension) {
        return bdd_endgame_table_type::create_ptr(egt_path);
    } else if (".plb" == extension) {
        return chesspresso_endgame_table_type::create_ptr(egt_path, 1);
    } else if (".wpr" == extension) {
        return probe_report_nybble_endgame_table_type::create_ptr(egt_path);
    }

    // The extension was not recognized.
    return nullptr;
}


std::unique_ptr<endgame_table_type> endgame_tables_type::load(
    egt_format_type const & egt_format,
    material_signature_type const & material_signature
) {
    std::string extension{"."};
    extension += egt_format.as_string();
    if (".clst" == extension) {
        return cube_list_endgame_table_type::create_ptr(material_signature);
    } else if (".dtr" == extension) {
        return decision_dag_endgame_table_type::create_ptr(material_signature);
    } else if (".icl" == extension) {
        return indexed_cube_list_endgame_table_type::create_ptr(
         material_signature);
    } else if (".mtbdd" == extension) {
        return bdd_endgame_table_type::create_ptr(material_signature);
    } else if (".plb" == extension) {
        return chesspresso_endgame_table_type::create_ptr(material_signature);
    } else if (".wpr" == extension) {
        return probe_report_nybble_endgame_table_type::create_ptr(
         canonicalization_enum::all_symmetries, material_signature);
    }

    // The extension was not recognized.
    return nullptr;
}


std::unique_ptr<endgame_table_type> endgame_tables_type::load(
    egt_format_type const & egt_format,
    material_signature_type const & material_signature,
    std::string_view const version_specifier
) {
    std::string extension{"."};
    extension += egt_format.as_string();
    if (".clst" == extension) {
        return cube_list_endgame_table_type::create_ptr(material_signature,
         version_specifier);
    } else if (".dtr" == extension) {
        return decision_dag_endgame_table_type::create_ptr(material_signature);
    } else if (".icl" == extension) {
        return indexed_cube_list_endgame_table_type::create_ptr(
         material_signature, version_specifier);
    } else if (".mtbdd" == extension) {
        return bdd_endgame_table_type::create_ptr(material_signature,
         version_specifier);
    } else if (".plb" == extension) {
        return chesspresso_endgame_table_type::create_ptr(material_signature);
    } else if (".wpr" == extension) {
        std::optional<canonicalization_enum> const canonicalization_mode_opt{
         as_canonicalization_enum_opt(version_specifier)};
        canonicalization_enum const canonicalization_mode{
         canonicalization_mode_opt ? *canonicalization_mode_opt
         : canonicalization_enum::all_symmetries};
        return probe_report_nybble_endgame_table_type::create_ptr(
         canonicalization_mode, material_signature);
    }

    // The extension was not recognized.
    return nullptr;
}


void endgame_tables_type::load_or_exit(
    egt_format_type const & egt_format,
    material_signature_type const & material_signature
) {
    std::unique_ptr<endgame_table_type> egt_ptr{
     load(egt_format, material_signature)};
    bool const load_failed{nullptr == egt_ptr};
    if (load_failed) {
        std::cerr << "EE: An attempt to load " << material_signature << " for "
         << egt_format.as_string() << " failed.\n";
        std::exit(EXIT_FAILURE);
    }
    the_egt_ptrs[material_signature] = std::move(egt_ptr);
}


void endgame_tables_type::load_or_exit(
    egt_format_type const & egt_format,
    material_signature_type const & material_signature,
    std::string_view const version_specifier
) {
    std::unique_ptr<endgame_table_type> egt_ptr{
     load(egt_format, material_signature, version_specifier)};
    bool const load_failed{nullptr == egt_ptr};
    if (load_failed) {
        std::cerr << "EE: An attempt to load " << material_signature << "("
         << version_specifier << ") for " << egt_format.as_string()
         << " failed.\n";
        std::exit(EXIT_FAILURE);
    }
    the_egt_ptrs[material_signature] = std::move(egt_ptr);
}


void endgame_tables_type::load_or_warn(
    egt_format_type const & egt_format,
    material_signature_type const & material_signature
) {
    std::unique_ptr<endgame_table_type> egt_ptr{
     load(egt_format, material_signature)};
    bool const load_failed{nullptr == egt_ptr};
    if (load_failed) {
        std::cerr << "WW: An attempt to load " << material_signature << " for "
         << egt_format.as_string() << " failed.\n";
    } else {
        the_egt_ptrs[material_signature] = std::move(egt_ptr);
    }
}


void endgame_tables_type::load_or_warn(
    egt_format_type const & egt_format,
    material_signature_type const & material_signature,
    std::string_view const version_specifier
) {
    std::unique_ptr<endgame_table_type> egt_ptr{
     load(egt_format, material_signature, version_specifier)};
    bool const load_failed{nullptr == egt_ptr};
    if (load_failed) {
        std::cerr << "WW: An attempt to load " << material_signature << "("
         << version_specifier << ") for " << egt_format.as_string()
         << " failed.\n";
    } else {
        the_egt_ptrs[material_signature] = std::move(egt_ptr);
    }
}
