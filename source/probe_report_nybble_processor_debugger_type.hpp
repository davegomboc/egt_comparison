#if !defined(EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_DEBUGGER_TYPE_HPP)
#define EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_DEBUGGER_TYPE_HPP


#include "probe_report_nybble_processor_type.hpp"


class probe_report_nybble_processor_debugger_type
: public probe_report_nybble_processor_type {

public:
    virtual ~probe_report_nybble_processor_debugger_type() override = default;
    probe_report_nybble_processor_debugger_type(
        representation::material_signature_type const & material_signature,
        unsigned int const cover_count_to_generate
    ) : probe_report_nybble_processor_type(material_signature,
        cover_count_to_generate) {}

private:
    virtual std::string name() const override {
        return "probe_report_nybble_processor_debugger";
    }

    virtual std::filesystem::path output_path_prefix() const override {
        return std::filesystem::path();
    }

    virtual void on_process_commencement() override;
    virtual void on_process_completion() override;

    virtual void record(
        representation::material_signature_type const & material_signature,
        std::size_t const position_id,
        representation::egt_position_type const & egt_position,
        probe_report_type wpr_probe_report) override;
};


#endif // EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_DEBUGGER_TYPE_HPP
