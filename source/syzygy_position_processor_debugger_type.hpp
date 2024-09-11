#if !defined(EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_DEBUGGER_TYPE_HPP)
#define EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_DEBUGGER_TYPE_HPP


#include <filesystem>
#include <string>
#include <vector>

#include "representation/canonicalization_enum.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "representation/perspective_enum.hpp"

#include "syzygy_position_processor_type.hpp"


class syzygy_position_processor_debugger_type
  : public syzygy_position_processor_type {

public:
    virtual ~syzygy_position_processor_debugger_type() override = default;
    syzygy_position_processor_debugger_type() = default;

private:
    virtual std::string name() const override {
        return "debugging_processor";
    }

    virtual std::filesystem::path output_path_prefix(
        representation::perspective_enum
    ) const override {
        return std::filesystem::temp_directory_path();
    }

    virtual void on_process_table_commencement(
        representation::canonicalization_enum /*canonicalization*/,
        representation::perspective_enum /*perspective*/,
        representation::material_signature_type const & /*material_signature*/
    ) override {}

    virtual void on_process_table_completion(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature
    ) override;

    virtual void on_postprocess_table(
        representation::canonicalization_enum canonicalization,
        representation::perspective_enum perspective,
        representation::material_signature_type const & material_signature
    ) override {
        std::clog << "II: Postprocessing of " << material_signature << " "
         << canonicalization << " " << perspective << " would occur now.\n";
    }

    virtual void record(
        representation::material_signature_type const & material_signature,
        std::size_t const position_id,
        representation::egt_position_type const & noncanonicalized_egt_position,
        probe_report_type probe_report) override;
};


#endif // EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_DEBUGGER_TYPE_HPP
