#if !defined(EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_TYPE_HPP)
#define EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_TYPE_HPP


#include <cstddef>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <vector>

#include "representation/bitboards_type.hpp"
#include "representation/canonicalization_enum.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/figurine_placements_type.hpp"
#include "representation/material_signature_type.hpp"
#include "representation/perspective_enum.hpp"

#include "probe_report_counting_type.hpp"


class syzygy_position_processor_type {
public:
    syzygy_position_processor_type & operator=(
     syzygy_position_processor_type const &) = delete;
    syzygy_position_processor_type & operator=(
     syzygy_position_processor_type &&) = default;
    syzygy_position_processor_type(syzygy_position_processor_type const &) = delete;
    syzygy_position_processor_type(syzygy_position_processor_type &&) = default;
    virtual ~syzygy_position_processor_type() = default;
    syzygy_position_processor_type() = default;

    void process_tables(
        representation::canonicalization_enum canonicalize,
        representation::perspective_enum perspective,
        std::vector<representation::material_signature_type> const &
         material_signatures,
        bool also_do_postprocessing);

protected:
    virtual std::filesystem::path output_path_prefix(
        representation::perspective_enum perspective) const = 0;

    // Returns the std::error_code from attempting to create the directories.
    std::error_code ensure_output_parent_path_exists(
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature
    ) const;

    void record_additional_metadata(
        std::vector<std::pair<std::string, std::string>> const & key_value_pairs
    ) const;

    void dump_probe_report_counters(std::ostream & out) const;

private:
    virtual std::string name() const = 0;

    virtual void on_process_table_commencement(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature) = 0;

    virtual void on_process_table_completion(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature) = 0;

    virtual void on_postprocess_table(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature) = 0;

    virtual void record(
        representation::material_signature_type const & material_signature,
        std::size_t const position_id,
        representation::egt_position_type const & noncanonicalized_egt_position,
        probe_report_type finalized_probe_report) = 0;

    virtual void vtable_pinning_method() const;

    void count_processed_egt_position(
        std::optional<representation::egt_position_type> const &
         canonicalized_egt_position_opt,
        std::optional<probe_report_type>
         probe_report_of_canonicalized_egt_position_opt,
        std::optional<representation::egt_position_type> const &
         noncanonicalized_egt_position_opt,
        std::optional<probe_report_type>
         probe_report_of_noncanonicalized_egt_position_opt);

    void create_metadata_file(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature);

    void process_egt_position(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature,
        std::size_t const position_id);

    void process_table(
        representation::canonicalization_enum canonicalize,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature);

    probe_report_type query_egt_position(
        representation::egt_position_type const & egt_position);

    void record_syzygy_position_processor_computed_metadata() const;

    std::chrono::seconds the_processing_duration;
    std::optional<probe_report_counting_type> the_counters_opt;
    std::unique_ptr<std::ofstream> the_metadata_ptr;
};


#endif // EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_TYPE_HPP
