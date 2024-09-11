#if !defined(EGT_COMPARISON_ENDGAME_TABLES_TYPE_HPP)
#define EGT_COMPARISON_ENDGAME_TABLES_TYPE_HPP


#include <filesystem>
#include <map>
#include <memory>
#include <string_view>

#include "representation/enumerated_material_signatures_type.hpp"
#include "representation/material_signature_type.hpp"

#include "egt_format_type.hpp"
#include "endgame_table_type.hpp"


class endgame_tables_type {

public:
    endgame_tables_type & operator=(endgame_tables_type const &) = delete;
    endgame_tables_type & operator=(endgame_tables_type &&) = default;
    endgame_tables_type(endgame_tables_type const &) = delete;
    endgame_tables_type(endgame_tables_type &&) = default;
    ~endgame_tables_type() = default;
    endgame_tables_type(
        unsigned int highest_figurine_count,
        egt_format_type const & egt_format,
        bool exit_on_load_failure);
    endgame_tables_type(
        unsigned int highest_figurine_count,
        egt_format_type const & egt_format,
	std::string const & version_specifier,
        bool exit_on_load_failure);

    endgame_table_type const & egt(representation::material_signature_type);

    void load_or_exit(
        egt_format_type const & egt_format,
        representation::material_signature_type const & material_signature);
    void load_or_exit(
        egt_format_type const & egt_format,
        representation::material_signature_type const & material_signature,
        std::string_view const version_specifier);
    void load_or_warn(
        egt_format_type const & egt_format,
        representation::material_signature_type const & material_signature);
    void load_or_warn(
        egt_format_type const & egt_format,
        representation::material_signature_type const & material_signature,
        std::string_view const version_specifier);

    endgame_table_type const * get(
        representation::material_signature_type const & material_signature
    ) const;

private:
    // Returns nullptr iff an error occurred with most formats.  However, note
    // that the BDD loading code exits irregularly when an attempted load fails.
    std::unique_ptr<endgame_table_type> load(
        std::filesystem::path const & egt_path);
    std::unique_ptr<endgame_table_type> load(
        egt_format_type const & egt_format,
        representation::material_signature_type const & material_signature);
    std::unique_ptr<endgame_table_type> load(
        egt_format_type const & egt_format,
        representation::material_signature_type const & material_signature,
        std::string_view const version_specifier);

    representation::enumerated_material_signatures_type
     the_enumerated_material_signatures;

    std::map<representation::material_signature_type,
     std::unique_ptr<endgame_table_type>> the_egt_ptrs;
};


#endif // EGT_COMPARISON_ENDGAME_TABLES_TYPE_HPP
