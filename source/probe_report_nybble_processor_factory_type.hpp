#if !defined (EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_FACTORY_TYPE_HPP)
#define EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_FACTORY_TYPE_HPP


#include <memory>

#include "representation/canonicalization_enum.hpp"
#include "representation/perspective_enum.hpp"
#include "representation/material_signature_type.hpp"

#include "probe_report_nybble_processor_enum.hpp"
#include "probe_report_nybble_processor_type.hpp"


class probe_report_nybble_processor_factory_type {

public:
    std::unique_ptr<probe_report_nybble_processor_type> create_ptr(
     probe_report_nybble_processor_enum processor,
     representation::canonicalization_enum canonicalization,
     representation::perspective_enum perspective,
     representation::material_signature_type const & material_signature,
     unsigned int cover_count_to_generate);
};


#endif // EGT_COMPARISON_PROBE_REPORT_NYBBLE_PROCESSOR_FACTORY_TYPE_HPP
