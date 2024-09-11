#if !defined(EGT_COMPARISON_ENDGAME_TABLE_TYPE_HPP)
#define EGT_COMPARISON_ENDGAME_TABLE_TYPE_HPP


#include <filesystem>
#include <iosfwd>

#include "representation/canonicalization_enum.hpp"
#include "representation/egt_position_type.hpp"
#include "probe_report_type.hpp"


class endgame_table_type {

public:
    virtual ~endgame_table_type() = default;

    virtual std::filesystem::path extension() const = 0;

    probe_report_type probe_after_canonicalizing(
        representation::egt_position_type const & position) const;

protected:
    endgame_table_type & operator=(endgame_table_type const &) = default;
    endgame_table_type & operator=(endgame_table_type &&) = default;
    endgame_table_type(endgame_table_type const &) = default;
    endgame_table_type(endgame_table_type &&) = default;
    endgame_table_type(
        representation::canonicalization_enum canonicalization_mode);

    representation::canonicalization_enum canonicalization_mode() const;

    virtual void vtable_pinning_method() const;

private:
    virtual probe_report_type probe_directly_impl(
        representation::egt_position_type const & position) const = 0;

    representation::canonicalization_enum the_canonicalization_mode;
};


std::ostream & operator<<(std::ostream & out, endgame_table_type const & endgame_table);


#endif // EGT_COMPARISON_ENDGAME_TABLE_TYPE_HPP
