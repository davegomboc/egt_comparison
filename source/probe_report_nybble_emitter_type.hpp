#if !defined(EGT_COMPARISON_PROBE_REPORT_NYBBLE_EMITTER_TYPE_HPP)
#define EGT_COMPARISON_PROBE_REPORT_NYBBLE_EMITTER_TYPE_HPP


#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>

#include "representation/canonicalization_enum.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "representation/perspective_enum.hpp"

#include "paths.hpp"
#include "syzygy_position_processor_type.hpp"


class probe_report_nybble_emitter_type : public syzygy_position_processor_type {

public:
    virtual ~probe_report_nybble_emitter_type() override = default;
    probe_report_nybble_emitter_type() = default;

private:
    virtual std::string name() const override {
        return "probe_report_nybble_emitter";
    }

    virtual std::filesystem::path output_path_prefix(
        representation::perspective_enum /*perspective*/
    ) const override {
        return probe_report_nybble_path_prefix();
    }

    virtual void on_process_table_commencement(
        representation::canonicalization_enum canonicalize,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature
    ) override;

    virtual void record(
        representation::material_signature_type const & material_signature,
        std::size_t const position_id,
        representation::egt_position_type const & noncanonicalized_egt_position,
        probe_report_type const finalized_probe_report) override;

    virtual void on_process_table_completion(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature
    ) override;

    virtual void on_postprocess_table(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature
    ) override;

    virtual void vtable_pinning_method() const override;

    std::vector<std::byte> the_probe_reports;
    std::filesystem::path the_file_path;
    std::unique_ptr<std::ofstream> the_output_stream_ptr;
};


#endif // EGT_COMPARISON_PROBE_REPORT_NYBBLE_EMITTER_TYPE_HPP
