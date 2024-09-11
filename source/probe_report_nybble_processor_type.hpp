#if !defined (EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_TYPE_HPP)
#define EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_TYPE_HPP


#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "probe_report_nybble_endgame_table_type.hpp"


class probe_report_nybble_processor_type {
public:
    virtual ~probe_report_nybble_processor_type() = default;
    probe_report_nybble_processor_type(
        representation::material_signature_type const & material_signature,
        unsigned int cover_count_to_generate);

    void process();

protected:
    virtual std::string name() const = 0;

    virtual std::filesystem::path output_path_prefix() const = 0;

    representation::material_signature_type material_signature() const {
        return the_material_signature;
    }

    unsigned int cover_count_to_generate() const {
        return the_cover_count_to_generate;
    }

    bool record_F() const { return 1 <= the_cover_count_to_generate; }
    bool record_R() const { return 2 <= the_cover_count_to_generate; }
    bool record_D() const { return 3 <= the_cover_count_to_generate; }

private:
    virtual void on_process_commencement() {}
    virtual void on_process_completion() {}

    virtual void record(
        representation::material_signature_type const & material_signature,
        std::size_t position_id,
        representation::egt_position_type const & egt_position,
        probe_report_type wpr_probe_report) = 0;

    virtual void vtable_pinning_method() const;

    representation::material_signature_type the_material_signature;

    // 1 = F, 2 = F and R, 3 = F, R, and D.
    unsigned int the_cover_count_to_generate;

    std::unique_ptr<probe_report_nybble_endgame_table_type> table_ptr;
};


#endif // EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_TYPE_HPP
